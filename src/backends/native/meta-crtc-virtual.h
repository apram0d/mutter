/*
 * Copyright (C) 2021 Red Hat
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

#ifndef META_CRTC_VIRTUAL_H
#define META_CRTC_VIRTUAL_H

#include "backends/native/meta-crtc-native.h"

#define META_TYPE_CRTC_VIRTUAL (meta_crtc_virtual_get_type ())
G_DECLARE_FINAL_TYPE (MetaCrtcVirtual, meta_crtc_virtual,
                      META, CRTC_VIRTUAL,
                      MetaCrtcNative)

MetaCrtcVirtual * meta_crtc_virtual_new (MetaBackend *backend,
                                         uint64_t     id);

#endif /* META_CRTC_VIRTUAL_H */
