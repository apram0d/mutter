/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2012  Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:
 *   Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>
 */

#ifndef __CLUTTER_ROTATE_ACTION_H__
#define __CLUTTER_ROTATE_ACTION_H__

#if !defined(__CLUTTER_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <clutter/clutter.h> can be included directly."
#endif

#include <clutter/clutter-gesture-action.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_ROTATE_ACTION               (clutter_rotate_action_get_type ())
#define CLUTTER_ROTATE_ACTION(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_ROTATE_ACTION, ClutterRotateAction))
#define CLUTTER_IS_ROTATE_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_ROTATE_ACTION))
#define CLUTTER_ROTATE_ACTION_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_ROTATE_ACTION, ClutterRotateActionClass))
#define CLUTTER_IS_ROTATE_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_ROTATE_ACTION))
#define CLUTTER_ROTATE_ACTION_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_ROTATE_ACTION, ClutterRotateActionClass))

typedef struct _ClutterRotateAction              ClutterRotateAction;
typedef struct _ClutterRotateActionPrivate       ClutterRotateActionPrivate;
typedef struct _ClutterRotateActionClass         ClutterRotateActionClass;

struct _ClutterRotateAction
{
  /*< private >*/
  ClutterGestureAction parent_instance;

  ClutterRotateActionPrivate *priv;
};

/**
 * ClutterRotateActionClass:
 * @rotate: class handler for the #ClutterRotateAction::rotate signal
 *
 * The #ClutterRotateActionClass structure contains
 * only private data.
 */
struct _ClutterRotateActionClass
{
  /*< private >*/
  ClutterGestureActionClass parent_class;
};

CLUTTER_EXPORT
GType clutter_rotate_action_get_type (void) G_GNUC_CONST;

CLUTTER_EXPORT
ClutterAction *clutter_rotate_action_new        (void);

G_END_DECLS

#endif /* __CLUTTER_ROTATE_ACTION_H__ */
