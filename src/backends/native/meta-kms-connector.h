/*
 * Copyright (C) 2019 Red Hat
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

#ifndef META_KMS_CONNECTOR_H
#define META_KMS_CONNECTOR_H

#include <glib-object.h>
#include <stdint.h>
#include <xf86drmMode.h>

#include "backends/meta-output.h"
#include "backends/native/meta-kms-types.h"

#define META_TYPE_KMS_CONNECTOR (meta_kms_connector_get_type ())
META_EXPORT_TEST
G_DECLARE_FINAL_TYPE (MetaKmsConnector, meta_kms_connector,
                      META, KMS_CONNECTOR, GObject)

typedef struct _MetaKmsConnectorState
{
  uint32_t current_crtc_id;

  uint32_t common_possible_crtcs;
  uint32_t common_possible_clones;
  uint32_t encoder_device_idxs;

  GList *modes;

  uint32_t width_mm;
  uint32_t height_mm;

  MetaTileInfo tile_info;
  GBytes *edid_data;

  gboolean has_scaling;
  gboolean non_desktop;
  MetaPrivacyScreenState privacy_screen_state;

  CoglSubpixelOrder subpixel_order;

  int suggested_x;
  int suggested_y;
  gboolean hotplug_mode_update;

  MetaMonitorTransform panel_orientation_transform;

  MetaKmsRange max_bpc;

  struct {
    MetaOutputColorspace value;
    uint64_t supported;
  } colorspace;

  struct {
    MetaOutputHdrMetadata value;
    gboolean supported;
    gboolean unknown;
  } hdr;
} MetaKmsConnectorState;

META_EXPORT_TEST
MetaKmsDevice * meta_kms_connector_get_device (MetaKmsConnector *connector);

uint32_t meta_kms_connector_get_connector_type (MetaKmsConnector *connector);

uint32_t meta_kms_connector_get_id (MetaKmsConnector *connector);

const char * meta_kms_connector_get_name (MetaKmsConnector *connector);

gboolean meta_kms_connector_can_clone (MetaKmsConnector *connector,
                                       MetaKmsConnector *other_connector);

META_EXPORT_TEST
MetaKmsMode * meta_kms_connector_get_preferred_mode (MetaKmsConnector *connector);

META_EXPORT_TEST
const MetaKmsConnectorState * meta_kms_connector_get_current_state (MetaKmsConnector *connector);

gboolean meta_kms_connector_is_underscanning_supported (MetaKmsConnector *connector);

gboolean meta_kms_connector_is_privacy_screen_supported (MetaKmsConnector *connector);

const MetaKmsRange * meta_kms_connector_get_max_bpc (MetaKmsConnector *connector);

gboolean meta_kms_connector_is_color_space_supported (MetaKmsConnector     *connector,
                                                      MetaOutputColorspace  color_space);

gboolean meta_kms_connector_is_hdr_metadata_supported (MetaKmsConnector *connector);

#endif /* META_KMS_CONNECTOR_H */
