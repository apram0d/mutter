/*
 * Wayland Support
 *
 * Copyright (C) 2022 Robert Mader <robert.mader@posteo.de>
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
 *
 */

#ifndef META_WAYLAND_FRACTIONAL_SCALE_H
#define META_WAYLAND_FRACTIONAL_SCALE_H

#include "wayland/meta-wayland-types.h"

void meta_wayland_init_fractional_scale (MetaWaylandCompositor *compositor);

void meta_wayland_fractional_scale_maybe_send_preferred_scale (MetaWaylandSurface *surface,
                                                               double              scale);

#endif /* META_WAYLAND_FRACTIONAL_SCALE_H */
