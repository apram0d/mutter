/*
 * Copyright (C) 2011,2013 Intel Corporation.
 * Copyrigth (C) 2020 Red Hat
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "cogl-config.h"

#include "winsys/cogl-onscreen-xlib.h"

#include "cogl-context-private.h"
#include "cogl-renderer-private.h"
#include "cogl-xlib-renderer-private.h"
#include "winsys/cogl-onscreen-egl.h"
#include "winsys/cogl-winsys-egl-x11-private.h"

struct _CoglOnscreenXlib
{
  CoglOnscreenEgl parent;

  Window xwin;

  gboolean pending_resize_notify;
};

G_DEFINE_TYPE (CoglOnscreenXlib, cogl_onscreen_xlib,
               COGL_TYPE_ONSCREEN_EGL)

#define COGL_ONSCREEN_X11_EVENT_MASK (StructureNotifyMask | ExposureMask)


static Window
create_xwindow (CoglOnscreenXlib  *onscreen_xlib,
                EGLConfig          egl_config,
                GError           **error)
{
  CoglOnscreen *onscreen = COGL_ONSCREEN (onscreen_xlib);
  CoglFramebuffer *framebuffer = COGL_FRAMEBUFFER (onscreen);
  CoglContext *context = cogl_framebuffer_get_context (framebuffer);
  CoglDisplay *display = context->display;
  CoglRenderer *renderer = display->renderer;
  CoglXlibRenderer *xlib_renderer =
    _cogl_xlib_renderer_get_data (renderer);
  Window xwin;
  int width;
  int height;
  CoglXlibTrapState state;
  XVisualInfo *xvisinfo;
  XSetWindowAttributes xattr;
  unsigned long mask;
  int xerror;

  width = cogl_framebuffer_get_width (framebuffer);
  height = cogl_framebuffer_get_height (framebuffer);

  _cogl_xlib_renderer_trap_errors (display->renderer, &state);

  xvisinfo = cogl_display_xlib_get_visual_info (display, egl_config);
  if (xvisinfo == NULL)
    {
      g_set_error (error, COGL_WINSYS_ERROR,
                   COGL_WINSYS_ERROR_CREATE_ONSCREEN,
                   "Unable to retrieve the X11 visual of context's "
                   "fbconfig");
      return None;
    }

  /* window attributes */
  xattr.background_pixel =
    WhitePixel (xlib_renderer->xdpy,
                DefaultScreen (xlib_renderer->xdpy));
  xattr.border_pixel = 0;
  /* XXX: is this an X resource that we are leaking‽... */
  xattr.colormap =
    XCreateColormap (xlib_renderer->xdpy,
                     DefaultRootWindow (xlib_renderer->xdpy),
                     xvisinfo->visual,
                     AllocNone);
  xattr.event_mask = COGL_ONSCREEN_X11_EVENT_MASK;

  mask = CWBorderPixel | CWColormap | CWEventMask;

  xwin = XCreateWindow (xlib_renderer->xdpy,
                        DefaultRootWindow (xlib_renderer->xdpy),
                        0, 0,
                        width, height,
                        0,
                        xvisinfo->depth,
                        InputOutput,
                        xvisinfo->visual,
                        mask, &xattr);

  XFree (xvisinfo);

  XSync (xlib_renderer->xdpy, False);
  xerror =
    _cogl_xlib_renderer_untrap_errors (display->renderer, &state);
  if (xerror)
    {
      char message[1000];
      XGetErrorText (xlib_renderer->xdpy, xerror,
                     message, sizeof (message));
      g_set_error (error, COGL_WINSYS_ERROR,
                   COGL_WINSYS_ERROR_CREATE_ONSCREEN,
                   "X error while creating Window for CoglOnscreen: %s",
                   message);
      return None;
    }

  return xwin;
}

static gboolean
cogl_onscreen_xlib_allocate (CoglFramebuffer  *framebuffer,
                             GError          **error)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (framebuffer);
  CoglOnscreenEgl *onscreen_egl = COGL_ONSCREEN_EGL (framebuffer);
  CoglContext *context = cogl_framebuffer_get_context (framebuffer);
  CoglDisplay *display = context->display;
  CoglRenderer *renderer = display->renderer;
  CoglRendererEGL *egl_renderer = renderer->winsys;
  EGLConfig egl_config;
  Window xwin;
  EGLSurface egl_surface;
  CoglFramebufferClass *parent_class;

  if (!cogl_onscreen_egl_choose_config (onscreen_egl, &egl_config, error))
    return FALSE;

  xwin = create_xwindow (onscreen_xlib, egl_config, error);
  if (xwin == None)
    return FALSE;

  onscreen_xlib->xwin = xwin;

  egl_surface =
    eglCreateWindowSurface (egl_renderer->edpy,
                            egl_config,
                            (EGLNativeWindowType) onscreen_xlib->xwin,
                            NULL);
  cogl_onscreen_egl_set_egl_surface (onscreen_egl,
                                     egl_surface);

  parent_class = COGL_FRAMEBUFFER_CLASS (cogl_onscreen_xlib_parent_class);
  return parent_class->allocate (framebuffer, error);
}

static void
cogl_onscreen_xlib_dispose (GObject *object)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (object);

  G_OBJECT_CLASS (cogl_onscreen_xlib_parent_class)->dispose (object);

  if (onscreen_xlib->xwin != None)
    {
      CoglFramebuffer *framebuffer = COGL_FRAMEBUFFER (object);
      CoglContext *context = cogl_framebuffer_get_context (framebuffer);
      CoglRenderer *renderer = context->display->renderer;
      CoglXlibRenderer *xlib_renderer =
        _cogl_xlib_renderer_get_data (renderer);
      CoglXlibTrapState old_state;

      _cogl_xlib_renderer_trap_errors (renderer, &old_state);

      XDestroyWindow (xlib_renderer->xdpy, onscreen_xlib->xwin);
      onscreen_xlib->xwin = None;
      XSync (xlib_renderer->xdpy, False);

      if (_cogl_xlib_renderer_untrap_errors (renderer,
                                             &old_state) != Success)
        g_warning ("X Error while destroying X window");

      onscreen_xlib->xwin = None;
    }
}

void
_cogl_winsys_onscreen_xlib_set_visibility (CoglOnscreen *onscreen,
                                           gboolean      visibility)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (onscreen);
  CoglFramebuffer *framebuffer = COGL_FRAMEBUFFER (onscreen);
  CoglContext *context = cogl_framebuffer_get_context (framebuffer);
  CoglRenderer *renderer = context->display->renderer;
  CoglXlibRenderer *xlib_renderer =
    _cogl_xlib_renderer_get_data (renderer);

  if (visibility)
    XMapWindow (xlib_renderer->xdpy, onscreen_xlib->xwin);
  else
    XUnmapWindow (xlib_renderer->xdpy, onscreen_xlib->xwin);
}

void
_cogl_winsys_onscreen_xlib_set_resizable (CoglOnscreen *onscreen,
                                          gboolean      resizable)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (onscreen);
  CoglFramebuffer *framebuffer = COGL_FRAMEBUFFER (onscreen);
  CoglContext *context = cogl_framebuffer_get_context (framebuffer);
  CoglXlibRenderer *xlib_renderer =
    _cogl_xlib_renderer_get_data (context->display->renderer);

  XSizeHints *size_hints = XAllocSizeHints ();

  if (resizable)
    {
      /* TODO: Add cogl_onscreen_request_minimum_size () */
      size_hints->min_width = 1;
      size_hints->min_height = 1;

      size_hints->max_width = INT_MAX;
      size_hints->max_height = INT_MAX;
    }
  else
    {
      int width = cogl_framebuffer_get_width (framebuffer);
      int height = cogl_framebuffer_get_height (framebuffer);

      size_hints->min_width = width;
      size_hints->min_height = height;

      size_hints->max_width = width;
      size_hints->max_height = height;
    }

  XSetWMNormalHints (xlib_renderer->xdpy, onscreen_xlib->xwin, size_hints);

  XFree (size_hints);
}

uint32_t
_cogl_winsys_onscreen_xlib_get_window_xid (CoglOnscreen *onscreen)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (onscreen);

  return onscreen_xlib->xwin;
}

gboolean
cogl_onscreen_xlib_is_for_window (CoglOnscreen *onscreen,
                                  Window        window)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (onscreen);

  return onscreen_xlib->xwin == window;
}

static void
flush_pending_resize_notifications_cb (void *data,
                                       void *user_data)
{
  CoglFramebuffer *framebuffer = data;

  if (COGL_IS_ONSCREEN (framebuffer))
    {
      CoglOnscreen *onscreen = COGL_ONSCREEN (framebuffer);
      CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (onscreen);

      if (onscreen_xlib->pending_resize_notify)
        {
          _cogl_onscreen_notify_resize (onscreen);
          onscreen_xlib->pending_resize_notify = FALSE;
        }
    }
}

static void
flush_pending_resize_notifications_idle (void *user_data)
{
  CoglContext *context = user_data;
  CoglRenderer *renderer = context->display->renderer;
  CoglRendererEGL *egl_renderer = renderer->winsys;

  /* This needs to be disconnected before invoking the callbacks in
   * case the callbacks cause it to be queued again */
  _cogl_closure_disconnect (egl_renderer->resize_notify_idle);
  egl_renderer->resize_notify_idle = NULL;

  g_list_foreach (context->framebuffers,
                  flush_pending_resize_notifications_cb,
                  NULL);
}

void
cogl_onscreen_xlib_resize (CoglOnscreen *onscreen,
                           int           width,
                           int           height)
{
  CoglOnscreenXlib *onscreen_xlib = COGL_ONSCREEN_XLIB (onscreen);
  CoglFramebuffer *framebuffer = COGL_FRAMEBUFFER (onscreen);
  CoglContext *context = cogl_framebuffer_get_context (framebuffer);
  CoglRenderer *renderer = context->display->renderer;
  CoglRendererEGL *egl_renderer = renderer->winsys;


  _cogl_framebuffer_winsys_update_size (framebuffer, width, height);

  /* We only want to notify that a resize happened when the
   * application calls cogl_context_dispatch so instead of immediately
   * notifying we queue an idle callback */
  if (!egl_renderer->resize_notify_idle)
    {
      egl_renderer->resize_notify_idle =
        _cogl_poll_renderer_add_idle (renderer,
                                      flush_pending_resize_notifications_idle,
                                      context,
                                      NULL);
    }

  onscreen_xlib->pending_resize_notify = TRUE;
}

CoglOnscreenXlib *
cogl_onscreen_xlib_new (CoglContext *context,
                        int          width,
                        int          height)
{
  CoglFramebufferDriverConfig driver_config;

  driver_config = (CoglFramebufferDriverConfig) {
    .type = COGL_FRAMEBUFFER_DRIVER_TYPE_BACK,
  };
  return g_object_new (COGL_TYPE_ONSCREEN_XLIB,
                       "context", context,
                       "driver-config", &driver_config,
                       "width", width,
                       "height", height,
                       NULL);
}

static void
cogl_onscreen_xlib_init (CoglOnscreenXlib *onscreen_xlib)
{
}

static void
cogl_onscreen_xlib_class_init (CoglOnscreenXlibClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  CoglFramebufferClass *framebuffer_class = COGL_FRAMEBUFFER_CLASS (klass);

  object_class->dispose = cogl_onscreen_xlib_dispose;

  framebuffer_class->allocate = cogl_onscreen_xlib_allocate;
}