/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* Mutter utilities */

/*
 * Copyright (C) 2001 Havoc Pennington
 * Copyright (C) 2005 Elijah Newren
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef META_UTIL_PRIVATE_H
#define META_UTIL_PRIVATE_H

#include <glib/gi18n-lib.h>

#include "meta/util.h"
#include "meta/common.h"

/* META_EXPORT_TEST should be used to export symbols that are exported only
 * for testability purposes */
#define META_EXPORT_TEST META_EXPORT

void     meta_set_verbose (gboolean setting);
void     meta_set_debugging (gboolean setting);

void     meta_set_is_wayland_compositor (gboolean setting);

char *   meta_generate_random_id (GRand *rand,
                                  int    length);

void meta_init_debug_utils (void);

#define META_POINT_IN_RECT(xcoord, ycoord, rect) \
  ((xcoord) >= (rect).x &&                   \
   (xcoord) <  ((rect).x + (rect).width) &&  \
   (ycoord) >= (rect).y &&                   \
   (ycoord) <  ((rect).y + (rect).height))

#define META_CONTAINER_OF(ptr, type, member) \
  (type *) ((uint8_t *) (ptr) - G_STRUCT_OFFSET (type, member))

#endif
