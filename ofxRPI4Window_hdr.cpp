/* HDR EDID parsing. */
#define CTA_EXTENSION_VERSION		0x03
#define HDR_STATIC_METADATA_BLOCK       0x06
#define USE_EXTENDED_TAG		0x07

/* DRM HDR definitions. Not in the UAPI header, unfortunately. */
enum hdmi_metadata_type {
	HDMI_STATIC_METADATA_TYPE1 = 0,
};

enum hdmi_eotf {
	HDMI_EOTF_TRADITIONAL_GAMMA_SDR,
	HDMI_EOTF_TRADITIONAL_GAMMA_HDR,
	HDMI_EOTF_SMPTE_ST2084,
};



/**
 * igt_output_replace_prop_blob:
 * @output: output to set property on.
 * @prop: property for which the blob will be replaced.
 * @ptr: Pointer to contents for the property.
 * @length: Length of contents.
 *
 * This function will destroy the old property blob for the given property,
 * and will create a new property blob with the values passed to this function.
 *
 * The new property blob will be committed when you call igt_display_commit(),
 * igt_display_commit2() or igt_display_commit_atomic().
 */
void
igt_output_replace_prop_blob(igt_output_t *output, enum igt_atomic_connector_properties prop, const void *ptr, size_t length)
{
	igt_display_t *display = output->display;
	uint64_t *blob = &output->values[prop];
	uint32_t blob_id = 0;

	if (*blob != 0)
		igt_assert(drmModeDestroyPropertyBlob(display->drm_fd,
						      *blob) == 0);

	if (length > 0)
		igt_assert(drmModeCreatePropertyBlob(display->drm_fd,
						     ptr, length, &blob_id) == 0);

	*blob = blob_id;
	igt_output_set_prop_changed(output, prop);
}


/**
 * kmstest_get_property:
 * @drm_fd: drm file descriptor
 * @object_id: object whose properties we're going to get
 * @object_type: type of obj_id (DRM_MODE_OBJECT_*)
 * @name: name of the property we're going to get
 * @prop_id: if not NULL, returns the property id
 * @value: if not NULL, returns the property value
 * @prop: if not NULL, returns the property, and the caller will have to free
 *        it manually.
 *
 * Finds a property with the given name on the given object.
 *
 * Returns: true in case we found something.
 */
bool
kmstest_get_property(int drm_fd, uint32_t object_id, uint32_t object_type,
		     const char *name, uint32_t *prop_id /* out */,
		     uint64_t *value /* out */,
		     drmModePropertyPtr *prop /* out */)
{
	drmModeObjectPropertiesPtr proplist;
	drmModePropertyPtr _prop;
	bool found = false;
	int i;

	proplist = drmModeObjectGetProperties(drm_fd, object_id, object_type);
	for (i = 0; i < proplist->count_props; i++) {
		_prop = drmModeGetProperty(drm_fd, proplist->props[i]);
		if (!_prop)
			continue;

		if (strcmp(_prop->name, name) == 0) {
			found = true;
			if (prop_id)
				*prop_id = proplist->props[i];
			if (value)
				*value = proplist->prop_values[i];
			if (prop)
				*prop = _prop;
			else
				drmModeFreeProperty(_prop);

			break;
		}
		drmModeFreeProperty(_prop);
	}

	drmModeFreeObjectProperties(proplist);
	return found;
}

static bool cta_block(const char *edid_ext)
{
	/*
	 * Byte 1: 0x07 indicates Extended Tag
	 * Byte 2: 0x06 indicates HDMI Static Metadata Block
	 * Byte 3: bits 0 to 5 identify EOTF functions supported by sink
	 *	       where ET_0: Traditional Gamma - SDR Luminance Range
	 *	             ET_1: Traditional Gamma - HDR Luminance Range
	 *	             ET_2: SMPTE ST 2084
	 *	             ET_3: Hybrid Log-Gamma (HLG)
	 *	             ET_4 to ET_5: Reserved for future use
	 */

	if ((((edid_ext[0] & 0xe0) >> 5 == USE_EXTENDED_TAG) &&
	      (edid_ext[1] == HDR_STATIC_METADATA_BLOCK)) &&
	     ((edid_ext[2] & HDMI_EOTF_TRADITIONAL_GAMMA_HDR) ||
	      (edid_ext[2] & HDMI_EOTF_SMPTE_ST2084)))
			return true;

	return false;
}

/* Returns true if panel supports HDR. */
static bool is_panel_hdr(data_t *data, igt_output_t *output)
{
	bool ok;
	int i, j, offset;
	uint64_t edid_blob_id;
	drmModePropertyBlobRes *edid_blob;
	const struct edid_ext *edid_ext;
	const struct edid *edid;
	const struct edid_cea *edid_cea;
	const char *cea_data;
	bool ret = false;

	ok = kmstest_get_property(data->fd, output->id,
			DRM_MODE_OBJECT_CONNECTOR, "EDID",
			NULL, &edid_blob_id, NULL);

	if (!ok || !edid_blob_id)
		return ret;

	edid_blob = drmModeGetPropertyBlob(data->fd, edid_blob_id);
	igt_assert(edid_blob);

	edid = (const struct edid *) edid_blob->data;
	igt_assert(edid);

	drmModeFreePropertyBlob(edid_blob);

	for (i = 0; i < edid->extensions_len; i++) {
		edid_ext = &edid->extensions[i];
		edid_cea = &edid_ext->data.cea;

		/* HDR not defined in CTA Extension Version < 3. */
		if ((edid_ext->tag != EDID_EXT_CEA) ||
		    (edid_cea->revision != CTA_EXTENSION_VERSION))
				continue;
		else {
			offset = edid_cea->dtd_start;
			cea_data = edid_cea->data;

			for (j = 0; j < offset; j += (cea_data[j] & 0x1f) + 1) {
				ret = cta_block(cea_data + j);

				if (ret)
					break;
			}
		}
	}

	return ret;
}



/* Sets the HDR output metadata prop. */
static void set_hdr_output_metadata(data_t *data,
				    struct hdr_output_metadata const *meta)
{
	igt_output_replace_prop_blob(data->output,
				     IGT_CONNECTOR_HDR_OUTPUT_METADATA, meta,
				     meta ? sizeof(*meta) : 0);
}

/* Converts a double to 861-G spec FP format. */
static uint16_t calc_hdr_float(double val)
{
	return (uint16_t)(val * 50000.0);
}





/* Fills some test values for HDR metadata targeting SDR. */
static void fill_hdr_output_metadata_sdr(struct hdr_output_metadata *meta)
{
	memset(meta, 0, sizeof(*meta));

	meta->metadata_type = HDMI_STATIC_METADATA_TYPE1;
	meta->hdmi_metadata_type1.eotf = HDMI_EOTF_TRADITIONAL_GAMMA_SDR;

	/* Rec. 709 */
	meta->hdmi_metadata_type1.display_primaries[0].x =
		calc_hdr_float(0.640); /* Red */
	meta->hdmi_metadata_type1.display_primaries[0].y =
		calc_hdr_float(0.330);
	meta->hdmi_metadata_type1.display_primaries[1].x =
		calc_hdr_float(0.300); /* Green */
	meta->hdmi_metadata_type1.display_primaries[1].y =
		calc_hdr_float(0.600);
	meta->hdmi_metadata_type1.display_primaries[2].x =
		calc_hdr_float(0.150); /* Blue */
	meta->hdmi_metadata_type1.display_primaries[2].y =
		calc_hdr_float(0.006);
	meta->hdmi_metadata_type1.white_point.x = calc_hdr_float(0.3127);
	meta->hdmi_metadata_type1.white_point.y = calc_hdr_float(0.3290);

	meta->hdmi_metadata_type1.max_display_mastering_luminance = 0;
	meta->hdmi_metadata_type1.min_display_mastering_luminance = 0;
	meta->hdmi_metadata_type1.max_fall = 0;
	meta->hdmi_metadata_type1.max_cll = 0;
}

















/* Fills some test values for ST2048 HDR output metadata.
 *
 * Note: there isn't really a standard for what the metadata is supposed
 * to do on the display side of things. The display is free to ignore it
 * and clip the output, use it to help tonemap to the content range,
 * or do anything they want, really.
 */
static void fill_hdr_output_metadata_st2048(struct hdr_output_metadata *meta)
{
	memset(meta, 0, sizeof(*meta));

	meta->metadata_type = HDMI_STATIC_METADATA_TYPE1;
	meta->hdmi_metadata_type1.eotf = HDMI_EOTF_SMPTE_ST2084;

	/* Rec. 2020 */
	meta->hdmi_metadata_type1.display_primaries[0].x =
		calc_hdr_float(0.708); /* Red */
	meta->hdmi_metadata_type1.display_primaries[0].y =
		calc_hdr_float(0.292);
	meta->hdmi_metadata_type1.display_primaries[1].x =
		calc_hdr_float(0.170); /* Green */
	meta->hdmi_metadata_type1.display_primaries[1].y =
		calc_hdr_float(0.797);
	meta->hdmi_metadata_type1.display_primaries[2].x =
		calc_hdr_float(0.131); /* Blue */
	meta->hdmi_metadata_type1.display_primaries[2].y =
		calc_hdr_float(0.046);
	meta->hdmi_metadata_type1.white_point.x = calc_hdr_float(0.3127);
	meta->hdmi_metadata_type1.white_point.y = calc_hdr_float(0.3290);

	meta->hdmi_metadata_type1.max_display_mastering_luminance =
		1000; /* 1000 nits */
	meta->hdmi_metadata_type1.min_display_mastering_luminance =
		500;				   /* 0.05 nits */
	meta->hdmi_metadata_type1.max_fall = 1000; /* 1000 nits */
	meta->hdmi_metadata_type1.max_cll = 500;   /* 500 nits */
}
