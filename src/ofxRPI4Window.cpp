#include "ofxRPI4Window.h"
#include "igt_edid.h"
extern "C"
{
#include <libavutil/frame.h>
#include <libavutil/hwcontext_drm.h>
#include <libavutil/mastering_display_metadata.h>
}
#include <fcntl.h>
// O_CLOEXEC
//#include <asm-generic/fcntl.h>



#include <sys/ioctl.h>
#include <sys/mman.h>
#define __func__ __PRETTY_FUNCTION__

#define CASE_STR(x,y) case x: str = y; break

const char *format_str(uint32_t format)
{
	switch (format) {
	case DRM_FORMAT_INVALID:
		return "INVALID";
	case DRM_FORMAT_C8:
		return "C8";
	case DRM_FORMAT_R8:
		return "R8";
	case DRM_FORMAT_R16:
		return "R16";
	case DRM_FORMAT_RG88:
		return "RG88";
	case DRM_FORMAT_GR88:
		return "GR88";
	case DRM_FORMAT_RG1616:
		return "RG1616";
	case DRM_FORMAT_GR1616:
		return "GR1616";
	case DRM_FORMAT_RGB332:
		return "RGB332";
	case DRM_FORMAT_BGR233:
		return "BGR233";
	case DRM_FORMAT_XRGB4444:
		return "XRGB4444";
	case DRM_FORMAT_XBGR4444:
		return "XBGR4444";
	case DRM_FORMAT_RGBX4444:
		return "RGBX4444";
	case DRM_FORMAT_BGRX4444:
		return "BGRX4444";
	case DRM_FORMAT_ARGB4444:
		return "ARGB4444";
	case DRM_FORMAT_ABGR4444:
		return "ABGR4444";
	case DRM_FORMAT_RGBA4444:
		return "RGBA4444";
	case DRM_FORMAT_BGRA4444:
		return "BGRA4444";
	case DRM_FORMAT_XRGB1555:
		return "XRGB1555";
	case DRM_FORMAT_XBGR1555:
		return "XBGR1555";
	case DRM_FORMAT_RGBX5551:
		return "RGBX5551";
	case DRM_FORMAT_BGRX5551:
		return "BGRX5551";
	case DRM_FORMAT_ARGB1555:
		return "ARGB1555";
	case DRM_FORMAT_ABGR1555:
		return "ABGR1555";
	case DRM_FORMAT_RGBA5551:
		return "RGBA5551";
	case DRM_FORMAT_BGRA5551:
		return "BGRA5551";
	case DRM_FORMAT_RGB565:
		return "RGB565";
	case DRM_FORMAT_BGR565:
		return "BGR565";
	case DRM_FORMAT_RGB888:
		return "RGB888";
	case DRM_FORMAT_BGR888:
		return "BGR888";
	case DRM_FORMAT_XRGB8888:
		return "XRGB8888";
	case DRM_FORMAT_XBGR8888:
		return "XBGR8888";
	case DRM_FORMAT_RGBX8888:
		return "RGBX8888";
	case DRM_FORMAT_BGRX8888:
		return "BGRX8888";
	case DRM_FORMAT_ARGB8888:
		return "ARGB8888";
	case DRM_FORMAT_ABGR8888:
		return "ABGR8888";
	case DRM_FORMAT_RGBA8888:
		return "RGBA8888";
	case DRM_FORMAT_BGRA8888:
		return "BGRA8888";
	case DRM_FORMAT_XRGB2101010:
		return "XRGB2101010";
	case DRM_FORMAT_XBGR2101010:
		return "XBGR2101010";
	case DRM_FORMAT_RGBX1010102:
		return "RGBX1010102";
	case DRM_FORMAT_BGRX1010102:
		return "BGRX1010102";
	case DRM_FORMAT_ARGB2101010:
		return "ARGB2101010";
	case DRM_FORMAT_ABGR2101010:
		return "ABGR2101010";
	case DRM_FORMAT_RGBA1010102:
		return "RGBA1010102";
	case DRM_FORMAT_BGRA1010102:
		return "BGRA1010102";
	case DRM_FORMAT_XRGB16161616F:
		return "XRGB16161616F";
	case DRM_FORMAT_XBGR16161616F:
		return "XBGR16161616F";
	case DRM_FORMAT_ARGB16161616F:
		return "ARGB16161616F";
	case DRM_FORMAT_ABGR16161616F:
		return "ABGR16161616F";
	case DRM_FORMAT_AXBXGXRX106106106106:
		return "AXBXGXRX106106106106";
	case DRM_FORMAT_YUYV:
		return "YUYV";
	case DRM_FORMAT_YVYU:
		return "YVYU";
	case DRM_FORMAT_UYVY:
		return "UYVY";
	case DRM_FORMAT_VYUY:
		return "VYUY";
	case DRM_FORMAT_AYUV:
		return "AYUV";
	case DRM_FORMAT_XYUV8888:
		return "XYUV8888";
	case DRM_FORMAT_VUY888:
		return "VUY888";
	case DRM_FORMAT_VUY101010:
		return "VUY101010";
	case DRM_FORMAT_Y210:
		return "Y210";
	case DRM_FORMAT_Y212:
		return "Y212";
	case DRM_FORMAT_Y216:
		return "Y216";
	case DRM_FORMAT_Y410:
		return "Y410";
	case DRM_FORMAT_Y412:
		return "Y412";
	case DRM_FORMAT_Y416:
		return "Y416";
	case DRM_FORMAT_XVYU2101010:
		return "XVYU2101010";
	case DRM_FORMAT_XVYU12_16161616:
		return "XVYU12_16161616";
	case DRM_FORMAT_XVYU16161616:
		return "XVYU16161616";
	case DRM_FORMAT_Y0L0:
		return "Y0L0";
	case DRM_FORMAT_X0L0:
		return "X0L0";
	case DRM_FORMAT_Y0L2:
		return "Y0L2";
	case DRM_FORMAT_X0L2:
		return "X0L2";
	case DRM_FORMAT_YUV420_8BIT:
		return "YUV420_8BIT";
	case DRM_FORMAT_YUV420_10BIT:
		return "YUV420_10BIT";
	case DRM_FORMAT_XRGB8888_A8:
		return "XRGB8888_A8";
	case DRM_FORMAT_XBGR8888_A8:
		return "XBGR8888_A8";
	case DRM_FORMAT_RGBX8888_A8:
		return "RGBX8888_A8";
	case DRM_FORMAT_BGRX8888_A8:
		return "BGRX8888_A8";
	case DRM_FORMAT_RGB888_A8:
		return "RGB888_A8";
	case DRM_FORMAT_BGR888_A8:
		return "BGR888_A8";
	case DRM_FORMAT_RGB565_A8:
		return "RGB565_A8";
	case DRM_FORMAT_BGR565_A8:
		return "BGR565_A8";
	case DRM_FORMAT_NV12:
		return "NV12";
	case DRM_FORMAT_NV21:
		return "NV21";
	case DRM_FORMAT_NV16:
		return "NV16";
	case DRM_FORMAT_NV61:
		return "NV61";
	case DRM_FORMAT_NV24:
		return "NV24";
	case DRM_FORMAT_NV42:
		return "NV42";
	case DRM_FORMAT_NV15:
		return "NV15";
	case DRM_FORMAT_P210:
		return "P210";
	case DRM_FORMAT_P010:
		return "P010";
	case DRM_FORMAT_P012:
		return "P012";
	case DRM_FORMAT_P016:
		return "P016";
    case DRM_FORMAT_P030:
	    return "P030";
	case DRM_FORMAT_Q410:
		return "Q410";
	case DRM_FORMAT_Q401:
		return "Q401";
	case DRM_FORMAT_YUV410:
		return "YUV410";
	case DRM_FORMAT_YVU410:
		return "YVU410";
	case DRM_FORMAT_YUV411:
		return "YUV411";
	case DRM_FORMAT_YVU411:
		return "YVU411";
	case DRM_FORMAT_YUV420:
		return "YUV420";
	case DRM_FORMAT_YVU420:
		return "YVU420";
	case DRM_FORMAT_YUV422:
		return "YUV422";
	case DRM_FORMAT_YVU422:
		return "YVU422";
	case DRM_FORMAT_YUV444:
		return "YUV444";
	case DRM_FORMAT_YVU444:
		return "YVU444";
	default:
		return "Unknown";
	}
}


void hdr_output_metadata_info(int fd, uint32_t blob_id)
{
	drmModePropertyBlobRes *blob = drmModeGetPropertyBlob(fd, blob_id);
	if (!blob) {
		ofLogError() << "Could not get drmModeGetPropertyBlob";
	}

	struct drm_hdr_output_metadata *meta = static_cast<drm_hdr_output_metadata*>(blob->data);

	if (meta->metadata_type == 0 /*HDMI_STATIC_METADATA_TYPE1*/) {
 

		const struct drm_hdr_metadata_infoframe* info = &meta->hdmi_metadata_type1;
		ofLog() << "HDR_OUTPUT_METADATA currently set to:";
		ofLog() << "	metadata_type = " << static_cast<int>(info->metadata_type); 
		ofLog() << "	eotf = " << static_cast<int>(info->eotf);
		ofLog() << "	display_primaries_r_x = " << info->display_primaries[0].x;
		ofLog() << "	display_primaries_r_y = " << info->display_primaries[0].y;
		ofLog() << "	display_primaries_g_x = " << info->display_primaries[1].x;
		ofLog() << "	display_primaries_g_y = " << info->display_primaries[1].y;
		ofLog() << "	display_primaries_b_x = " << info->display_primaries[2].x;
		ofLog() << "	display_primaries_b_y = " << info->display_primaries[2].y;
		ofLog() << "	white_point_x = " << info->white_point.x;
		ofLog() << "	white_point_y = " << info->white_point.y;
		ofLog() << "	max_display_mastering_luminance = " << info->max_display_mastering_luminance;
		ofLog() << "	min_display_mastering_luminance = " << info->min_display_mastering_luminance;
		ofLog() << "	max_cll = " << info->max_cll;
		ofLog() << "	max_fall = " << info->max_fall;
	}

	drmModeFreePropertyBlob(blob);

}

void dovi_output_metadata_info(int fd, uint32_t blob_id)
{
	drmModePropertyBlobRes *blob = drmModeGetPropertyBlob(fd, blob_id);
	if (!blob) {
		ofLogError() << "Could not get drmModeGetPropertyBlob";
	}

	struct dovi_output_metadata *dovi = static_cast<dovi_output_metadata*>(blob->data);

 

		ofLog() << "DOVI_OUTPUT_METADATA currently set to:";
		ofLog() << "	DV Status = " << (dovi->dv_status ? "active" : "not active"); 
		ofLog() << "	DV Interface = " << (dovi->dv_interface ? "low latency" : "standard + low_latency"); //fix
		ofLog() << "	Backlight Metadata = " << (dovi->backlight_metadata ? "present" : "not present");
		ofLog() << "	Backlight Max Luminance = " << (dovi->backlight_max_luminance  ? "present" : "not present");
		ofLog() << "	Auxillary runmode = " << (dovi->aux_runmode  ? "present" : "not present");
		ofLog() << "	Auxillary version = " << (dovi->aux_version  ? "present" : "not present");
		ofLog() << "	Auxillary debug = " << (dovi->aux_debug  ? "present" : "not present");



	drmModeFreePropertyBlob(blob);

}

void ofxRPI4Window::in_formats_info(int fd, uint32_t blob_id)
{
	uint32_t fmt = 0;
	unsigned int mod_index = 0;

	drmModePropertyBlobRes *blob = drmModeGetPropertyBlob(fd, blob_id);
	if (!blob) {
		perror("drmModeGetPropertyBlob");

	}

	struct drm_format_modifier_blob *data = static_cast<drm_format_modifier_blob*>(blob->data);
	uint32_t fmts_arr[data->count_modifiers];
	uint32_t *fmts = (uint32_t *)
		((char *)data + data->formats_offset);

	struct drm_format_modifier *mods = (struct drm_format_modifier *)
		((char *)data + data->modifiers_offset);

	for (uint32_t i = 0; i < data->count_modifiers; ++i) {

		ofLog() << "modifier " << std::hex << mods[i].modifier;


		for (uint64_t j = 0; j < 64; ++j) {
			if (mods[i].formats & (1ull << j)) {
				fmt = fmts[j + mods[i].offset];

				ofLog() << "        " << format_str(fmt) << std::hex << " (0x" << fmt << ")"; 
			}
		}


	}


//	drmModeFreePropertyBlob(blob);


}

void ofxRPI4Window::get_format_modifiers(int fd, uint32_t blob_id, int format_index)
{


	drmModePropertyBlobRes *blob = drmModeGetPropertyBlob(fd, blob_id);
	if (!blob) {
		perror("drmModeGetPropertyBlob");

	}

	struct drm_format_modifier_blob *data = static_cast<drm_format_modifier_blob*>(blob->data);

	uint32_t *fmts = (uint32_t *)
		((char *)data + data->formats_offset);

	struct drm_format_modifier *mods = (struct drm_format_modifier *)
		((char *)data + data->modifiers_offset);

	for (uint32_t j = 0; j < data->count_modifiers; j++) {


			if (mods[j].formats & (1ull << format_index)) {


					num_modifiers++;

				

			}
		


	}
	modifiers = (uint64_t *)calloc(data->count_modifiers, sizeof(uint64_t));

	if (!modifiers) {
		ofLogError() << "DRM: -failed to allocate modifiers";
	}

	for (uint32_t j = 0; j < data->count_modifiers; j++) {
		if (mods[j].formats & (1ULL << format_index)) {
			modifiers[j-1] = mods[j].modifier;
	   /*
		* Some broadcom modifiers have parameters encoded which need to be
		* masked out before comparing with reported modifiers.
		*/
			if ((modifiers[j-1] >> 56) == DRM_FORMAT_MOD_VENDOR_BROADCOM)
				modifiers[j-1] = fourcc_mod_broadcom_mod(modifiers[j-1]);
		}
	}

	drmModeFreePropertyBlob(blob);


}


static string eglErrorString(EGLint err) {
    string str;
    switch (err) {
            CASE_STR(EGL_SUCCESS, "EGL_SUCCESS");
            CASE_STR(EGL_NOT_INITIALIZED, "EGL_NOT_INITIALIZED");
            CASE_STR(EGL_BAD_ACCESS, "EGL_BAD_ACCESS");
            CASE_STR(EGL_BAD_ALLOC, "EGL_BAD_ACCESS");
            CASE_STR(EGL_BAD_ATTRIBUTE, "EGL_BAD_ATTRIBUTE");
            CASE_STR(EGL_BAD_CONTEXT, "EGL_BAD_CONTEXT");
            CASE_STR(EGL_BAD_CONFIG, "EGL_BAD_CONFIG");
            CASE_STR(EGL_BAD_CURRENT_SURFACE, "EGL_BAD_CURRENT_SURFACE");
            CASE_STR(EGL_BAD_DISPLAY, "EGL_BAD_DISPLAY");
            CASE_STR(EGL_BAD_SURFACE, "EGL_BAD_SURFACE");
            CASE_STR(EGL_BAD_MATCH, "EGL_BAD_MATCH");
            CASE_STR(EGL_BAD_PARAMETER, "EGL_BAD_PARAMETER");
            CASE_STR(EGL_BAD_NATIVE_PIXMAP, "EGL_BAD_NATIVE_PIXMAP");
            CASE_STR(EGL_BAD_NATIVE_WINDOW, "EGL_BAD_NATIVE_WINDOW");
            CASE_STR(EGL_CONTEXT_LOST, "EGL_CONTEXT_LOST");
        default: str = "unknown error " + err; break;
    }
    return str;
}


ofxRPI4Window::ofxRPI4Window() {
    orientation = OF_ORIENTATION_DEFAULT;
    skipRender = false;
    previousBo = NULL;
    lastFrameTimeMillis = 0;
    gbmDevice = NULL;
    gbmSurface = NULL;
}
ofxRPI4Window::ofxRPI4Window(const ofGLESWindowSettings & settings) {
    ofLog() << "CTOR CALLED WITH settings";
    setup(settings);
}

int match_config_to_visual(EGLDisplay egl_display,
                           EGLint visual_id,
                           EGLConfig *configs,
                           int count)
{
    int i;
    
    for (i = 0; i < count; ++i) {
        EGLint id;
        
        if (!eglGetConfigAttrib(egl_display,
                                configs[i], EGL_NATIVE_VISUAL_ID,
                                &id))
            continue;
			
        if (id == visual_id)
            return i;
    }
 
    return -1;
}

void printConfigInfo(int n, EGLDisplay display, EGLConfig *config) {
 EGLint size;
EGLint visual_id;
    printf("Configuration %d is\n", n);

    eglGetConfigAttrib(display,
		       *config, EGL_RED_SIZE, &size);
    printf("  Red size is %d\n", size);
    eglGetConfigAttrib(display,
		       *config, EGL_GREEN_SIZE, &size);
    printf("  Green size is %d\n", size);
    eglGetConfigAttrib(display,
		       *config, EGL_BLUE_SIZE, &size);
    printf("  Blue size is %d\n", size);
	eglGetConfigAttrib(display,
		       *config, EGL_ALPHA_SIZE, &size);
    printf("  Alpha size is %d\n", size);
    eglGetConfigAttrib(display,
		       *config, EGL_BUFFER_SIZE, &size);
    printf("  Buffer size is %d\n", size);
	    eglGetConfigAttrib(display,
		       *config, EGL_NATIVE_VISUAL_ID, &visual_id);
    printf("  VisualID is %s (0x%08x)\n", format_str(visual_id),visual_id); 	

   eglGetConfigAttrib(display,
		       *config,  EGL_BIND_TO_TEXTURE_RGB , &size);
   if (size == EGL_TRUE)
       printf("  Can be bound to RGB texture\n");
   else
       printf("  Can't be bound to RGB texture\n");

   eglGetConfigAttrib(display,
		       *config,  EGL_BIND_TO_TEXTURE_RGBA , &size);
   if (size == EGL_TRUE)
       printf("  Can be bound to RGBA texture\n");
   else
       printf("  Can't be bound to RGBA texture\n");
}

/**
 * Print table of all available configurations.
 */
#define MAX_CONFIGS 1000
/* These are X visual types, so if you're running eglinfo under
 * something not X, they probably don't make sense. */
static const char *vnames[] = { "SG", "GS", "SC", "PC", "TC", "DC" };

static void
PrintConfigs(EGLDisplay d)
{
   EGLConfig configs[MAX_CONFIGS];
   EGLint numConfigs, i;

   eglGetConfigs(d, configs, MAX_CONFIGS, &numConfigs);

   printf("Configurations:\n");
   printf("     bf lv colorbuffer dp st  ms     vis                                 cav bi     renderable  supported\n");
   printf("  id sz  l  r  g  b  a th cl ns b     id                                 eat nd gl es es2 es3 vg surfaces \n");
   printf("----------------------------------------------------------------------------------------------------------\n");
   for (i = 0; i < numConfigs; i++) {
      EGLint id, size, level;
      EGLint red, green, blue, alpha;
      EGLint depth, stencil;
      EGLint renderable, surfaces;
      EGLint vid, vtype, caveat, bindRgb, bindRgba;
      EGLint samples, sampleBuffers;
	  EGLint cc_type;//, cc_type_fixed;
      char surfString[100] = "";

      eglGetConfigAttrib(d, configs[i], EGL_CONFIG_ID, &id);
      eglGetConfigAttrib(d, configs[i], EGL_BUFFER_SIZE, &size);
      eglGetConfigAttrib(d, configs[i], EGL_LEVEL, &level);

      eglGetConfigAttrib(d, configs[i], EGL_RED_SIZE, &red);
      eglGetConfigAttrib(d, configs[i], EGL_GREEN_SIZE, &green);
      eglGetConfigAttrib(d, configs[i], EGL_BLUE_SIZE, &blue);
      eglGetConfigAttrib(d, configs[i], EGL_ALPHA_SIZE, &alpha);
      eglGetConfigAttrib(d, configs[i], EGL_DEPTH_SIZE, &depth);
      eglGetConfigAttrib(d, configs[i], EGL_STENCIL_SIZE, &stencil);
      eglGetConfigAttrib(d, configs[i], EGL_NATIVE_VISUAL_ID, &vid);
      eglGetConfigAttrib(d, configs[i], EGL_NATIVE_VISUAL_TYPE, &vtype);

      eglGetConfigAttrib(d, configs[i], EGL_CONFIG_CAVEAT, &caveat);
      eglGetConfigAttrib(d, configs[i], EGL_BIND_TO_TEXTURE_RGB, &bindRgb);
      eglGetConfigAttrib(d, configs[i], EGL_BIND_TO_TEXTURE_RGBA, &bindRgba);
      eglGetConfigAttrib(d, configs[i], EGL_RENDERABLE_TYPE, &renderable);
      eglGetConfigAttrib(d, configs[i], EGL_SURFACE_TYPE, &surfaces);

      eglGetConfigAttrib(d, configs[i], EGL_SAMPLES, &samples);
      eglGetConfigAttrib(d, configs[i], EGL_SAMPLE_BUFFERS, &sampleBuffers);
	   eglGetConfigAttrib(d, configs[i],   EGL_COLOR_COMPONENT_TYPE_EXT,&cc_type);
 //  eglGetConfigAttrib(d, configs[i], EGL_PBUFFER_BIT, &cc_type_fixed);

      if (surfaces & EGL_WINDOW_BIT)
         strcat(surfString, "win,");
      if (surfaces & EGL_PBUFFER_BIT)
         strcat(surfString, "pb,");
      if (surfaces & EGL_PIXMAP_BIT)
         strcat(surfString, "pix,");
      if (surfaces & EGL_STREAM_BIT_KHR)
         strcat(surfString, "str,");
      if (strlen(surfString) > 0)
         surfString[strlen(surfString) - 1] = 0;

      printf("0x%02x %2d %2d %2d %2d %2d %2d %2d %2d %2d%2d %s(0x%02x)%s 0x%02x(%s)",
             id, size, level,
             red, green, blue, alpha,
             depth, stencil,
             samples, sampleBuffers, format_str(vid),vid, vtype < 6 ? vnames[vtype] : "--", cc_type, cc_type == 0x333A ? "Fixed" : "Float");
      printf("  %c  %c  %c  %c  %c   %c  %c %s\n",
             (caveat != EGL_NONE) ? 'y' : ' ',
             (bindRgba) ? 'a' : (bindRgb) ? 'y' : ' ',
             (renderable & EGL_OPENGL_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENGL_ES_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENGL_ES2_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENGL_ES3_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENVG_BIT) ? 'y' : ' ',
             surfString);
   }
}

static void check_extensions(void)
{

        const char *client_extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

        if (!client_extensions) {
            ofLogError() << "No client extensions string available\n";
            abort();
        }
        if (strstr(client_extensions, "EGL_KHR_platform_gbm")) {
			ofLog() << "EGL_KHR_platform_gbm available\n";
           // abort();
        }
		        if (strstr(client_extensions, "EGL_MESA_platform_gbm")) {
			ofLog() << "EGL_MESA_platform_gbm available\n";
           // abort();
        }
		        if (strstr(client_extensions, "EGL_EXT_platform_base")) {
			ofLog() << "EGL_EXT_platform_base available\n";
           // abort();
        }

}

static EGLDisplay
gbm_get_display (gbm_device* gbmDevice)
{
  EGLDisplay dpy = NULL;
        const char *client_extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

        if (!client_extensions) {
            ofLogError() << "No client extensions string available\n";
            abort();
        }
        if (!strstr(client_extensions, "EGL_KHR_platform_gbm")) {
			ofLogError() << "No EGL_KHR_platform_gbm available\n";
            abort();
        }
	  static const int MAX_DEVICES = 32;
  EGLDeviceEXT eglDevs[MAX_DEVICES];
  EGLint numDevices;

  PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT =
    (PFNEGLQUERYDEVICESEXTPROC)
    eglGetProcAddress("eglQueryDevicesEXT");

  eglQueryDevicesEXT(MAX_DEVICES, eglDevs, &numDevices);

  //printf("Detected %d devices\n", numDevices);
  
  if (strstr(client_extensions, "EGL_EXT_platform_base"))
    {
		ofLog() << "EGL_EXT_platform_base available\n";
      PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayEXT =
        (PFNEGLGETPLATFORMDISPLAYEXTPROC) eglGetProcAddress ("eglGetPlatformDisplayEXT");

      if (getPlatformDisplayEXT)
        dpy = getPlatformDisplayEXT (EGL_PLATFORM_GBM_KHR,
                               gbmDevice,
						      // eglDevs[2],
                                  NULL);
      if (dpy)
        return dpy;
    }

  return 0;//eglGetDisplay ((EGLNativeDisplayType) gbmDevice);
}
#if 1
void
ofxRPI4Window::drm_mode_atomic_set_property(int drm_fd, drmModeAtomicReq *freq, const char *name /* in */, uint32_t object_id /* in */,
			uint32_t prop_id /* in */, uint64_t value /* in */, drmModePropertyPtr prop /* in */)
{
	int success = 0;
    if (first_req) { req = drmModeAtomicAlloc(); first_req = 0;}
    uint32_t flags = DRM_MODE_ATOMIC_ALLOW_MODESET;	

	success = drmModeAtomicAddProperty(req, object_id, prop_id, value);						

	if (success < 0) {
		ofLogError() << "DRM: Unable to set " << name << " " << strerror(errno);
	} else {
					
		uint32_t type = prop->flags &
		(DRM_MODE_PROP_LEGACY_TYPE | DRM_MODE_PROP_EXTENDED_TYPE);
		

		switch (type) {
		case DRM_MODE_PROP_RANGE:
			ofLog() << "DRM: Setting " << name << ": range [" <<  prop->values[0] << ".." << prop->values[1] << "] = " << value;

			break;
		case DRM_MODE_PROP_ENUM:
			ofLog() << "DRM: Setting " << name << ": enum {" << prop->enums[value].name << "} = " << prop->enums[value].value;
			break;
		case DRM_MODE_PROP_BITMASK:
		    break;
		case DRM_MODE_PROP_OBJECT:
			ofLog() << "DRM: Setting " << name << ": object " << prop->name << " = " << value;
			break;
		case DRM_MODE_PROP_SIGNED_RANGE:
			ofLog() << "DRM: Setting " << name << ": range [" <<  prop->values[0] << ".." << prop->values[1] << "] = " << value;
			break;

		case DRM_MODE_PROP_BLOB:
			ofLog() << "DRM: Setting " << name << ": blob_id" << " = " << value;
			if (!value) {
				break;
			}
			if (strcmp(prop->name, "IN_FORMATS") == 0) {
			    in_formats_info(drm_fd, value);
			} else if (strcmp(prop->name, "MODE_ID") == 0) {
			//	mode_id_info(drm_fd, value);
			} else if (strcmp(prop->name, "HDR_OUTPUT_METADATA") == 0) {

				hdr_output_metadata_info(drm_fd, value);
			} else if (strcmp(prop->name, "DOVI_OUTPUT_METADATA") == 0) {

				dovi_output_metadata_info(drm_fd, value);
			} else if (strcmp(prop->name, "WRITEBACK_PIXEL_FORMATS") == 0) {
			//	writeback_pixel_formats_info(drm_fd, value);
			} else if (strcmp(prop->name, "PATH") == 0) {
				//path_info(drm_fd, value);
			}
			break;
		}		
	}
	if (last_req) {
		success = drmModeAtomicCommit(drm_fd, req, flags, NULL);
		if (success < 0) {
			ofLogError() << "DRM: failed to commit " << name << " property " << strerror(errno);
		} else {
			ofLog() << "DRM: property commit successful";
		}       
		drmModeAtomicFree(req);	
		last_req = 0;
	}
};

bool  
ofxRPI4Window::drm_mode_get_property(int drm_fd, uint32_t object_id, uint32_t object_type,
		     const char *name, uint32_t *prop_id /* out */,
		     uint64_t *value /* out */,
		     drmModePropertyPtr *prop /* out */)
{
	drmModeObjectPropertiesPtr proplist;
	drmModePropertyPtr _prop;
	bool found = false;
//	int i;

	proplist = drmModeObjectGetProperties(drm_fd, object_id, object_type);
	for (uint32_t i = 0; i < proplist->count_props; i++) {
		_prop = drmModeGetProperty(drm_fd, proplist->props[i]);
		if (!_prop)
			continue;

//		bool atomic = flags & DRM_MODE_PROP_ATOMIC;
//		bool immutable = flags & DRM_MODE_PROP_IMMUTABLE;
		uint64_t _value = proplist->prop_values[i];
		
		if (strcmp(_prop->name, name) == 0) {
			found = true;
			uint32_t flags = _prop->flags;
			uint32_t type = flags &
			(DRM_MODE_PROP_LEGACY_TYPE | DRM_MODE_PROP_EXTENDED_TYPE);
			

			switch (type) {
			case DRM_MODE_PROP_RANGE:
				ofLog() << name << ": range [" <<  _prop->values[0] << ".." << _prop->values[1] << "] = ";

				break;
			case DRM_MODE_PROP_ENUM:
			case DRM_MODE_PROP_BITMASK:

				for (int j = 0; j < _prop->count_enums; ++j) {;
				ofLog() << name << ": enum {" << _prop->enums[j].name << "} = " << _prop->enums[j].value;
				}
				break;
			case DRM_MODE_PROP_OBJECT:
				ofLog() << name << ": object " << _prop->name << " = " << _value;
				break;
			case DRM_MODE_PROP_SIGNED_RANGE:
				ofLog() << name << ": range [" <<  _prop->values[0] << ".." << _prop->values[1] << "] = ";
				break;
			}

			switch (type) {
			case DRM_MODE_PROP_RANGE:
				ofLog() << name << ": currently set to " << _value;
			    break;
			case DRM_MODE_PROP_ENUM:
			    ofLog() << name << ": currently set to enum {"<< _prop->enums[_value].name << "} = " << _value;
			    break;
			case DRM_MODE_PROP_BITMASK:
			    ofLog() << _value;
			    break;
			case DRM_MODE_PROP_OBJECT:
				if (!_value) {
					break;
				}
		//	    ofLog() << _value;
               
				break;
			case DRM_MODE_PROP_BLOB:
				// TODO: base64-encode blob contents
				ofLog() << name << ": currently set to blob_id " << _value;
				break;
			case DRM_MODE_PROP_SIGNED_RANGE:
				ofLog() << _value;
				break;
			}

			switch (type) {
			case DRM_MODE_PROP_BLOB:
				if (!_value) {
					break;
				}
				if (strcmp(_prop->name, "IN_FORMATS") == 0) {
				    in_formats_info(drm_fd, _value);
				} else if (strcmp(_prop->name, "MODE_ID") == 0) {
				//	mode_id_info(drm_fd, _value);
				ofLog() << "************";
				} else if (strcmp(_prop->name, "HDR_OUTPUT_METADATA") == 0) {
					hdr_output_metadata_info(drm_fd, _value);
				} else if (strcmp(_prop->name, "DOVI_OUTPUT_METADATA") == 0) {
					dovi_output_metadata_info(drm_fd, _value);
				} else if (strcmp(_prop->name, "WRITEBACK_PIXEL_FORMATS") == 0) {
				//	writeback_pixel_formats_info(drm_fd, _value);
				} else if (strcmp(_prop->name, "PATH") == 0) {
					//path_info(drm_fd, _value);
				}
				break;
			case DRM_MODE_PROP_RANGE:
				// This is a special case, as the SRC_* properties are
				// in 16.16 fixed point
				if (strncmp(_prop->name, "SRC_", 4) == 0) {
					ofLog() << (_value >> 16);
				}
				break;
			case DRM_MODE_PROP_OBJECT:
				if (!_value) {
					break;
				}
				if (strcmp(_prop->name, "FB_ID") == 0) {
					//data_obj = fb_info(fd, value);
				}
				break;
			}		
       		
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
	}

	drmModeFreeObjectProperties(proplist);
	return found;
}
#endif



static void
print_device_info(drmDevicePtr device, int i, bool print_revision)
{
    ofLog() << "device[" << i << "]";
    ofLog() << "+-> available_nodes " << hex << setw(4) <<  device->available_nodes;
    ofLog() << "+-> nodes";
    for (int j = 0; j < DRM_NODE_MAX; j++)
        if (device->available_nodes & 1 << j)
            ofLog() << "|   +-> nodes[" << j << "] " << device->nodes[j];
    ofLog() << "+-> bustype " << hex << setw(4) << device->bustype;
    if (device->bustype == DRM_BUS_PCI) {
        ofLog() << "|   +-> pci";
        ofLog() << "|       +-> domain " << hex << setw(4) << device->businfo.pci->domain;
        ofLog() << "|       +-> bus    " << hex << setw(2) <<  device->businfo.pci->bus;
        ofLog() << "|       +-> dev    " << hex << setw(2) <<  device->businfo.pci->dev;
        ofLog() << "|       +-> func   " << static_cast<unsigned long>(device->businfo.pci->func);
        ofLog() << "+-> deviceinfo";
        ofLog() << "    +-> pci";
        ofLog() << "        +-> vendor_id    " << hex << setw(4) <<  device->deviceinfo.pci->vendor_id;
        ofLog() << "        +-> device_id    " << hex << setw(4) <<  device->deviceinfo.pci->device_id;
        ofLog() << "        +-> subvendor_id  " << hex << setw(4) <<  device->deviceinfo.pci->subvendor_id;
        ofLog() << "        +-> subdevice_id  " << hex << setw(4) <<  device->deviceinfo.pci->subdevice_id;
        if (print_revision)
            ofLog() << "        +-> revision_id   " << hex << setw(2) <<  device->deviceinfo.pci->revision_id;
        else
            ofLog() <<"        +-> revision_id   IGNORED";
    } else if (device->bustype == DRM_BUS_USB) {
        ofLog() << "|   +-> usb";
        ofLog() << "|       +-> bus " << setw(3) << unsigned(device->businfo.usb->bus);
        ofLog() << "|       +-> dev " << setw(3) << unsigned(device->businfo.usb->dev);
        ofLog() << "+-> deviceinfo";
        ofLog() << "    +-> usb";
        ofLog() << "        +-> vendor  " << hex << setw(4) <<  device->deviceinfo.usb->vendor;
        ofLog() << "        +-> product " << hex << setw(4) <<  device->deviceinfo.usb->product;
    } else if (device->bustype == DRM_BUS_PLATFORM) {
        char **compatible = device->deviceinfo.platform->compatible;
        ofLog() << "|   +-> platform";
        ofLog() << "|       +-> fullname     " << device->businfo.platform->fullname;
        ofLog() << "+-> deviceinfo";
        ofLog() << "    +-> platform";
        ofLog() << "        +-> compatible";
        while (*compatible) {
            ofLog() << "                    " << *compatible;
            compatible++;
        }
    } else if (device->bustype == DRM_BUS_HOST1X) {
        char **compatible = device->deviceinfo.host1x->compatible;
        ofLog() << "|   +-> host1x";
        ofLog() << "|       +-> fullname     " << device->businfo.host1x->fullname;
        ofLog() << "+-> deviceinfo";
        ofLog() << "    +-> host1x";
        ofLog() << "        +-> compatible";
        while (*compatible) {
            ofLog() << "                    " << *compatible;
            compatible++;
        }
    } else {
        ofLog() << "Unknown/unhandled bustype";
    }

}

	const char* renderDevicePath{nullptr};
	int renderDevice;
int
ofxRPI4Window::find_device(void)
{
//    drmDevicePtr *devices;

    drmDevicePtr device;
    int fd, ret, max_devices;
    ofLog() << "--- Checking the number of DRM device available ---";
    max_devices = drmGetDevices2(0, NULL, 0);
    if (max_devices <= 0) {
        ofLogError() << "drmGetDevices2() has not found any devices " << strerror(errno) << " " << -max_devices;
        return 77;
    }
    ofLog() << "--- Devices reported " << max_devices << " ---";

 std::vector<drmDevicePtr> devices(max_devices);

    ofLog() << "--- Retrieving devices information (PCI device revision is ignored) ---";
    ret = drmGetDevices2(0, devices.data(), devices.size());//devices, max_devices);
    if (ret < 0) {
        ofLogError() << "drmGetDevices2() returned an error " << strerror(errno) << " " << ret;

        return -1;
    }

	for (const auto device : devices) {
		if (!(device->available_nodes & 1 << DRM_NODE_PRIMARY))
            continue;
		::close(fd);
		fd = open(device->nodes[DRM_NODE_PRIMARY], O_RDWR | O_CLOEXEC);
		if (fd < 0)
			continue;
		ofLog() << "DRM: opened device: " << device->nodes[DRM_NODE_PRIMARY];	
		print_device_info(devices[0], 0, false);
	
	    const char* renderPath = drmGetRenderDeviceNameFromFd(fd);

		if (!renderPath)
			renderPath = drmGetDeviceNameFromFd2(fd);

		if (!renderPath)
			renderPath = drmGetDeviceNameFromFd(fd);

		if (renderPath)
		{
			renderDevicePath = renderPath;
			renderDevice = open(renderPath, O_RDWR | O_CLOEXEC);
			if (renderDevice != 0)
				ofLog() << "DRM: - opened render node: " << renderPath;
		}
		drmFreeDevices(devices.data(), devices.size());//devices, ret);

		return fd;
	}
	drmFreeDevices(devices.data(), devices.size());//devices, ret);

    return 0;
}

bool ofxRPI4Window::cta_is_hdr_static_metadata_block(const char *edid_ext)
{
	unsigned int oui;
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

bool ofxRPI4Window::cta_is_dolby_video_block(const char *edid_ext)
{
	unsigned int oui;
	/*
	 * Byte 1: 0x07 indicates Extended Tag
	 * Byte 2: 0x01 indicates HDMI Dolby Vision VSDB
	 * Bytes 3-5: HDMI Dolby Laboratories OUI
	 */
	oui = edid_ext[4] << 16 | edid_ext[3] << 8 | edid_ext[2];
	if ((((edid_ext[0] & 0xe0) >> 5 == USE_EXTENDED_TAG) &&
	      (edid_ext[1] == DOLBY_VIDEO_DATA_BLOCK)) &&
	      (oui == HDMI_DOLBY_OUI))
			return true;

	return false;
}

/* Returns if panel supports HDR of HDR and Dolby Vision support */
int ofxRPI4Window::is_panel_hdr_dovi(int fd, int connector_id)
{
	bool ok;
	int i, j, offset;
	uint64_t edid_blob_id;
	drmModePropertyBlobRes *edid_blob;
	const struct edid_ext *edid_ext;
	const struct edid *edid;
	const struct edid_cea *edid_cea;
	const char *cea_data;
	int ret = 0;
	bool supportsHDR = false;
	bool supportsDoVi = false;
	
	ok = drm_mode_get_property(fd, connector_id,
			DRM_MODE_OBJECT_CONNECTOR, "EDID",
			NULL, &edid_blob_id, NULL);

	if (!ok || !edid_blob_id)
		return ret;

	edid_blob = drmModeGetPropertyBlob(fd, edid_blob_id);
	assert(edid_blob);

	edid = (const struct edid *) edid_blob->data;
	assert(edid);

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
			//	ret = cta_block(cea_data + j);
				
				if (cta_is_hdr_static_metadata_block(cea_data + j))
					supportsHDR = true;
					
				if (cta_is_dolby_video_block(cea_data + j))
					supportsDoVi = true;

			}
		}
	}
	if (supportsHDR && !supportsDoVi)
		ret = HDR_TYPE_HDR10;
	else if (supportsDoVi && supportsHDR)
		ret = HDR_TYPE_DOLBY_VISION;
	else 
		ret = 0; 
	
	return ret;
}

bool ofxRPI4Window::InitDRM()
{
	bool ok;
	device = find_device();

    /* Programmer!! Save your sanity!!
     * VERY important or we won't get all the available planes on drmGetPlaneResources()!
     * We also need to enable the ATOMIC cap to see the atomic properties in objects!! */
    int ret = drmSetClientCap(device, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
    if (ret)
       ofLogError() << "DRM: can't set UNIVERSAL PLANES cap";
    else
       ofLog() << "DRM: UNIVERSAL PLANES cap set";

    ret = drmSetClientCap(device, DRM_CLIENT_CAP_ATOMIC, 1);
    if (ret)
    {
       /*If this happens, check kernel support and kernel parameters
        * (add i915.nuclear_pageflip=y to the kernel boot line for example) */
       ofLogError() << "DRM: can't set ATOMIC caps: " <<  strerror(errno);
    }
    else
       ofLog() << "DRM: ATOMIC caps set";
 
    ret = drmSetClientCap(device, DRM_CLIENT_CAP_STEREO_3D, 1);
   if (ret)
   {
     ofLogError() << "DRM: failed to set stereo 3d capability: " << strerror(errno);
   }

#if defined(DRM_CLIENT_CAP_ASPECT_RATIO)
   ret = drmSetClientCap(device, DRM_CLIENT_CAP_ASPECT_RATIO, 0);
   if (ret != 0)
     ofLogError() << "DRM: aspect ratio capability is not supported: " << strerror(errno);
#endif
 
    drmModeRes* resources = drmModeGetResources(device);
    bool passed = false;
    if (resources == NULL)
    {
        ofLogError() << "DRM: Unable to get DRM resources";
    }
    
    drmModeConnector* connector = NULL;
    
    for (int i = 0; i < resources->count_connectors; i++)
    {
        drmModeConnector* modeConnector = drmModeGetConnector(device, resources->connectors[i]);
        if (modeConnector->connection == DRM_MODE_CONNECTED)
        {
            connector = modeConnector;
			ofLog() << "DRM: Using CONNECTOR_ID: " << connector->connector_id;
            break;
        }
        drmModeFreeConnector(connector);
    }
    
 
    if (connector == NULL)
    {
        ofLogError() << "DRM: Unable to get connector";
        drmModeFreeResources(resources);
    }
    
    connectorId = connector->connector_id;
    mode = connector->modes[0];
    
    currentWindowRect = ofRectangle(0, 0, mode.hdisplay, mode.vdisplay);
    ofLog() << "DRM: currentWindowRect: " << currentWindowRect;
    
    drmModeEncoder* encoder = NULL;
    
    if (connector->encoder_id)
    {
        encoder = drmModeGetEncoder(device, connector->encoder_id);
		ofLog() << "DRM: Using ENCODER_ID: " << connector->encoder_id;

    }
    
    
    if (encoder == NULL)
    {
        ofLogError() << "DRM: Unable to get encoder";
        
        drmModeFreeConnector(connector);
        drmModeFreeResources(resources);
    }//else
   // {
  //      passed = true;
   // }


		
        crtc = drmModeGetCrtc(device, encoder->crtc_id);
		for (int i = 0; i < resources->count_crtcs; i++) {
			if (resources->crtcs[i] == crtc->crtc_id) {
				crtc_index = i;
				break;
			}
		}
		
		
		
		
    res	= drmModeGetPlaneResources(device);
	if (!res) {
		ofLogError() << "DRM: Unable to get drmModeGetPlaneResources";

	}
int foundHDR=0;
	for (uint32_t i = 0; i < res->count_planes; i++) {
		plane = drmModeGetPlane(device, res->planes[i]);
		if (!plane) {
			ofLogError() << "DRM: Unable to get drmModeGetPlane";
			continue;
		}

		if (plane->possible_crtcs & (1 << crtc_index) && !foundHDR) {
			HDRplaneId = plane->plane_id;
			foundHDR = 1;

		}	

  
       		if (plane->possible_crtcs & (1 << crtc_index) && plane->plane_id != HDRplaneId && foundHDR) {
			SDRplaneId = plane->plane_id;
			break;

		}	



	
	}
				
			
	ofLog() << "DRM: Using FB_ID: " << crtc->buffer_id;					
	ofLog() << "DRM: Using CRTC_ID: " << crtc->crtc_id;
	ofLog() << "DRM: Using HDR PLANE_ID: " << HDRplaneId;
	ofLog() << "DRM: Using SDR PLANE_ID: " << SDRplaneId;	
		
    ret = drmSetMaster(device);
    if (ret < 0)
    {
      ofLogError() << "DRM: - failed to set drm master, will try to authorize instead: " << strerror(errno);

      drm_magic_t magic;

      ret = drmGetMagic(device, &magic);
      if (ret < 0)
      {
        ofLogError() << "DRM: - failed to get drm magic: " <<  strerror(errno);
	 //   passed = false; 
      } else {
        ofLog() << "DRM: - successfully got drm magic";		
	 //   passed = true; 
	  }
      ret = drmAuthMagic(device, magic);
      if (ret < 0)
      {
        ofLogError() << "DRM: - failed to authorize drm magic: " << strerror(errno);
	 	passed = false;
      } else {
	    ofLog() << "DRM: - successfully authorized drm magic";
	    passed = true;
      }
     } else {
	   ofLog() << "DRM: - successfully authorized drm master";
	   passed = true; 
	  
	}


		

	drmModeFreePlane(plane);

	drmModeFreePlaneResources(res);
    drmModeFreeEncoder(encoder);
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
	
	
	if(passed)
    { 
		ofLog() << "DRM: - initialized atomic DRM";
		return true;
	} else {
		return false;
	}
}

void ofxRPI4Window::FindModifiers(uint32_t format, uint32_t plane_id)
{
	bool ok;
	int format_index=0;

	plane = drmModeGetPlane(device, plane_id);
	if (!plane) {
		ofLogError() << "DRM: Unable to get drmModeGetPlane";
	}


	
	for (uint32_t i = 0; i < plane->count_formats; i++) {
		uint32_t fmt = plane->formats[i];

		if (fmt == format) {
			format_index = i;
			break;
		} 
	}
	ok = drm_mode_get_property(device, plane_id, 
			DRM_MODE_OBJECT_PLANE, "IN_FORMATS",
			NULL, &in_formats, NULL);
	if (!ok) 
		ofLogError() << "DRM: Unable to find IN_FORMATS";	
	
	get_format_modifiers(device, in_formats, format_index);		
	
	drmModeFreePlane(plane);
	
}	

	
void ofxRPI4Window::EGL_info()
{
	#if 1
            
            
            ofLog() << "-----EGL-----";
           // ofLog() << "EGL_VERSION_MAJOR = " << eglVersionMajor;
           // ofLog() << "EGL_VERSION_MINOR = " << eglVersionMinor;
            ofLog() << "EGL_CLIENT_APIS = " << eglQueryString(getEGLDisplay(), EGL_CLIENT_APIS);
            ofLog() << "EGL_VENDOR = "  << eglQueryString(getEGLDisplay(), EGL_VENDOR);
            ofLog() << "EGL_VERSION = " << eglQueryString(getEGLDisplay(), EGL_VERSION);
            ofLog() << "EGL_EXTENSIONS = " << eglQueryString(getEGLDisplay(), EGL_EXTENSIONS);
            ofLog() << "GL_SHADING_LANGUAGE_VERSION   = " << glGetString(GL_SHADING_LANGUAGE_VERSION);
            ofLog() << "GL_RENDERER = " << glGetString(GL_RENDERER);
            ofLog() << "GL_VERSION  = " << glGetString(GL_VERSION);
            ofLog() << "GL_VENDOR   = " << glGetString(GL_VENDOR);
            ofLog() << "-------------";
            
            auto gl_exts = (char *) glGetString(GL_EXTENSIONS);
            ofLog(OF_LOG_VERBOSE, "GL INFO");
            ofLog(OF_LOG_VERBOSE, "  version: \"%s\"", glGetString(GL_VERSION));
            ofLog(OF_LOG_VERBOSE, "  shading language version: \"%s\"", glGetString(GL_SHADING_LANGUAGE_VERSION));
            ofLog(OF_LOG_VERBOSE, "  vendor: \"%s\"", glGetString(GL_VENDOR));
            ofLog(OF_LOG_VERBOSE, "  renderer: \"%s\"", glGetString(GL_RENDERER));
            ofLog(OF_LOG_VERBOSE, "  extensions: \"%s\"", gl_exts);
            ofLog(OF_LOG_VERBOSE, "===================================\n");
            //get_proc_gl(GL_OES_EGL_image, glEGLImageTargetTexture2DOES);
       
#endif   
}

int ofxRPI4Window::isHDR = 0;
int ofxRPI4Window::isDolby = 0;

void ofxRPI4Window::setup(const ofGLESWindowSettings & settings)
{
	
//	 ofSetLogLevel(OF_LOG_VERBOSE);
    check_extensions();
    bEnableSetupScreen = true;
    windowMode = OF_WINDOW;
    glesVersion = settings.glesVersion;
    InitDRM();
    if (is_panel_hdr_dovi(device, connectorId) == HDR_TYPE_HDR10) {
		ofLog() << "DRM: panel is HDR capable";
		ofLog() << "DRM: setting up HDR window/surface"; 
		isHDR = 1;
		isDolby = 0;
		FindModifiers(DRM_FORMAT_ARGB2101010, HDRplaneId);
		HDRWindowSetup();
	} else if (is_panel_hdr_dovi(device, connectorId) == HDR_TYPE_DOLBY_VISION) {
		ofLog() << "DRM: panel is HDR and Dolby Vision capable";
		ofLog() << "DRM: setting up Dolby Vision window/surface"; 
		isHDR = 1;
		isDolby = 1;
		FindModifiers(DRM_FORMAT_ARGB2101010, HDRplaneId);
		DoViWindowSetup();
	} else {
		ofLog() << "DRM: panel is not HDR capable";
		ofLog() << "DRM: setting up SDR window/surface";
		isHDR = 0;
		isDolby =0;
		FindModifiers(DRM_FORMAT_ARGB8888, SDRplaneId);
		SDRWindowSetup();
	}

//	EGL_info();
        

    
}

void ofxRPI4Window::HDRWindowSetup()
{
	if (!DestroyWindow()) 
	{
		ofLogError() << "GBM: Failed to deinitialize GBM";
	}
	
    gbmDevice = gbm_create_device(device);
	  if (!gbmDevice)
	{
		ofLogError() << "GBM: - failed to create device: " << gbmDevice; 

	}
#if 1
#if defined(HAS_GBM_MODIFIERS)
	if (num_modifiers > 0)
	{
		gbmSurface = gbm_surface_create_with_modifiers(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB2101010, modifiers,
                                                num_modifiers);
	}
#endif
	if (!gbmSurface)
	{
		gbmSurface = gbm_surface_create(gbmDevice, mode.hdisplay, mode.vdisplay,GBM_FORMAT_ARGB2101010,
									GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
	}

	if (!gbmSurface)
	{
		ofLogError() << "GBM: - failed to create surface: " << strerror(errno);

	} else {

		ofLog() << "GBM: - created surface with size " << mode.hdisplay << "x" << mode.vdisplay << " and " << ((*modifiers >= 0) ? "modifier " : "no modifier ") << hex << ((*modifiers >= 0) ? *modifiers : NULL);
	}
#else
    gbmSurface = gbm_surface_create(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB2101010, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

	if (!gbmSurface)
	{
		ofLogError() << "GBM: - failed to create surface: " << strerror(errno);

	} else {

		ofLog() << "GBM: - created surface with size " << mode.hdisplay << "x" << mode.vdisplay;
	}
#endif		
	display = gbm_get_display(gbmDevice);
    if (!display)
    {
        auto error = eglGetError();
        ofLogError() << "display ERROR: " << eglErrorString(error);
    }
        
    int major, minor;
    //   eglInitialize(display, &major, &minor);
    //eglBindAPI(EGL_OPENGL_API);
    if (!eglInitialize(display, &major, &minor))
    {
        auto error = eglGetError();
        ofLogError() << "initialize ERROR: " << eglErrorString(error);
    }
    eglBindAPI(EGL_OPENGL_ES_API);
        
    EGLint count = 0;
    EGLint matched = 0;
    int config_index = -1;
        
    if (!eglGetConfigs(display, NULL, 0, &count) || count < 1)
    {
        ofLogError() << "No EGL configs to choose from";
    }
    printf("EGL has %d configs\n", count);


	EGLConfig *configs = (EGLConfig *)malloc(count * sizeof *configs);
  //      EGLConfig configs[count];

	EGLint configAttribs[] = {
		EGL_RED_SIZE,10,
		EGL_GREEN_SIZE,10,
		EGL_BLUE_SIZE,10,
		EGL_ALPHA_SIZE,2,
		EGL_DEPTH_SIZE,24,
		EGL_BUFFER_SIZE,32,
		EGL_STENCIL_SIZE,8,
		EGL_SAMPLES,0,
		EGL_SAMPLE_BUFFERS,0,
//		EGL_BIND_TO_TEXTURE_RGBA,EGL_TRUE,
//		EGL_BIND_TO_TEXTURE_RGB,EGL_FALSE,
//		EGL_CONFIG_CAVEAT,EGL_NON_CONFORMANT_CONFIG,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR, //| EGL_OPENGL_ES3_BIT,
		EGL_COLOR_COMPONENT_TYPE_EXT, EGL_COLOR_COMPONENT_TYPE_FIXED_EXT, //EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT, 
		EGL_NONE
	};
		//EGLint visualId = GBM_FORMAT_RGBX1010102; //??HDR  
	EGLint visualId = GBM_FORMAT_ARGB2101010;


  //  eglGetConfigs(display, configs, count, &count);
    
//    int i;
//    for (i = 0; i < count; i++) {
//	printConfigInfo(i, display, &configs[i]);
 //   }
    PrintConfigs(display);
	 
    EGLConfig config = NULL;
        
        
        
        
    if (!eglChooseConfig(display, configAttribs, configs, count, &matched) || !matched)
	{
        printf("No EGL configs with appropriate attributes.\n");
    }
       
    if (config_index == -1)
    {
        config_index = match_config_to_visual(display,
                                              visualId,
                                              configs,
                                              matched);
    }
        
    if (config_index != -1)
    {
        config = configs[config_index];
    }
        
        
        
 //       const EGLint contextAttribs[] = {
  //         EGL_CONTEXT_CLIENT_VERSION, 2,
  //          EGL_NONE};
    const EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,  //update to version 3.0, previously 2
		EGL_CONTEXT_MINOR_VERSION, 1,
        EGL_NONE
	};
			
    if(config)
    {
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (!context)
        {
            auto error = eglGetError();
            ofLogError() << "context ERROR: " << eglErrorString(error);
        }
	    const char *client_extensions = eglQueryString(display, EGL_EXTENSIONS);
				  
	    if (strstr(client_extensions, "EGL_EXT_gl_colorspace_bt2020_pq"))
		{
			ofLog() << "EGL_GL_COLORSPACE_BT2020_PQ_EXT available\n";
				  
		} else {
			ofLogError() << "EGL_GL_COLORSPACE_BT2020_PQ_EXT not available\n";
		}
		if (strstr(client_extensions, "EGL_KHR_gl_colorspace")) {
			ofLog() << "EGL_GL_COLORSPACE_KHR  available\n";
		} else {
			ofLogError() << "EGL_GL_COLORSPACE_KHR not available\n";
		}
		EGLint attribs[] = {EGL_GL_COLORSPACE_KHR,EGL_GL_COLORSPACE_BT2020_PQ_EXT,EGL_NONE };         

	
		PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceEXT = nullptr;
		const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
		if (extensions && (strstr(extensions, "EGL_KHR_platform_gbm") || strstr(extensions, "EGL_MESA_platform_gbm"))) {
			createPlatformWindowSurfaceEXT = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)
			eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
		}
		if (createPlatformWindowSurfaceEXT) {
			surface = createPlatformWindowSurfaceEXT(display, config, gbmSurface, attribs);
		} else {
			ofLog() << "No eglCreatePlatformWindowSurface for GBM, falling back to eglCreateWindowSurface\n" ;
			surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)gbmSurface, NULL);
		}



//for (uint32_t i = 0; i < 8; i++)
//{
//    eglSurfaceAttrib(display,surface, SurfaceAttribs[i],EGLint(DisplayChromacityList[1].ChromaVals[i] * EGL_METADATA_SCALING_EXT));
//}
#if 1
		eglSurfaceAttrib(display,surface, SurfaceAttribs[0],EGLint(DisplayChromacityList[1].RedX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[1],EGLint(DisplayChromacityList[1].RedY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[2],EGLint(DisplayChromacityList[1].GreenX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[3],EGLint(DisplayChromacityList[1].GreenY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[4],EGLint(DisplayChromacityList[1].BlueX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[5],EGLint(DisplayChromacityList[1].BlueY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[6],EGLint(DisplayChromacityList[1].WhiteX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[7],EGLint(DisplayChromacityList[1].WhiteY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[8],EGLint(10000.0f * 10000.0f));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[9],EGLint(0.001f    * 10000.0f));
#endif

	

        if (!surface)
        {
            auto error = eglGetError();
            ofLogError() << "surface ERROR: " << eglErrorString(error);
        }
            
        currentRenderer = make_shared<ofGLProgrammableRenderer>(this);
        makeCurrent();
        static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(3,1);
#if 0
		updateHDR_Infoframe(HDMI_EOTF_SMPTE_ST2084, 1);
		struct avi_infoframe avi_infoframe;
		avi_infoframe.colorspace = 9;
		avi_infoframe.output_format = 1;
		avi_infoframe.max_bpc = 10;
		avi_infoframe.c_enc = 2;
		avi_infoframe.c_range = 1;
		updateAVI_Infoframe(HDRplaneId, avi_infoframe);	
#endif 


EGL_info();	
		ofLog() << "GBM: - initialized GBM";	
			
	} else {
        ofLog() << "RIP";
    }
	
	
}

void ofxRPI4Window::DoViWindowSetup()
{
	if (!DestroyWindow()) 
	{
		ofLogError() << "GBM: Failed to deinitialize GBM";
	}
	
    gbmDevice = gbm_create_device(device);
	  if (!gbmDevice)
	{
		ofLogError() << "GBM: - failed to create device: " << gbmDevice; 

	}
#if 1
#if defined(HAS_GBM_MODIFIERS)
	if (num_modifiers > 0)
	{
		gbmSurface = gbm_surface_create_with_modifiers(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB2101010, modifiers,
                                                num_modifiers);
	}
#endif
	if (!gbmSurface)
	{
		gbmSurface = gbm_surface_create(gbmDevice, mode.hdisplay, mode.vdisplay,GBM_FORMAT_ARGB2101010,
									GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
	}

	if (!gbmSurface)
	{
		ofLogError() << "GBM: - failed to create surface: " << strerror(errno);

	} else {

		ofLog() << "GBM: - created surface with size " << mode.hdisplay << "x" << mode.vdisplay << " and " << ((*modifiers >= 0) ? "modifier " : "no modifier ") << hex << ((*modifiers >= 0) ? *modifiers : NULL);
	}
#else
    gbmSurface = gbm_surface_create(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB2101010, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

	if (!gbmSurface)
	{
		ofLogError() << "GBM: - failed to create surface: " << strerror(errno);

	} else {

		ofLog() << "GBM: - created surface with size " << mode.hdisplay << "x" << mode.vdisplay;
	}
#endif		
	display = gbm_get_display(gbmDevice);
    if (!display)
    {
        auto error = eglGetError();
        ofLogError() << "display ERROR: " << eglErrorString(error);
    }
        
    int major, minor;
    //   eglInitialize(display, &major, &minor);
    //eglBindAPI(EGL_OPENGL_API);
    if (!eglInitialize(display, &major, &minor))
    {
        auto error = eglGetError();
        ofLogError() << "initialize ERROR: " << eglErrorString(error);
    }
    eglBindAPI(EGL_OPENGL_ES_API);
        
    EGLint count = 0;
    EGLint matched = 0;
    int config_index = -1;
        
    if (!eglGetConfigs(display, NULL, 0, &count) || count < 1)
    {
        ofLogError() << "No EGL configs to choose from";
    }
    printf("EGL has %d configs\n", count);


	EGLConfig *configs = (EGLConfig *)malloc(count * sizeof *configs);
  //      EGLConfig configs[count];

	EGLint configAttribs[] = {
		EGL_RED_SIZE,10,
		EGL_GREEN_SIZE,10,
		EGL_BLUE_SIZE,10,
		EGL_ALPHA_SIZE,2,
		EGL_DEPTH_SIZE,24,
		EGL_BUFFER_SIZE,32,
		EGL_STENCIL_SIZE,8,
		EGL_SAMPLES,0,
		EGL_SAMPLE_BUFFERS,0,
//		EGL_BIND_TO_TEXTURE_RGBA,EGL_TRUE,
//		EGL_BIND_TO_TEXTURE_RGB,EGL_FALSE,
//		EGL_CONFIG_CAVEAT,EGL_NON_CONFORMANT_CONFIG,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR, //| EGL_OPENGL_ES3_BIT,
		EGL_COLOR_COMPONENT_TYPE_EXT, EGL_COLOR_COMPONENT_TYPE_FIXED_EXT, //EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT, 
		EGL_NONE
	};
		//EGLint visualId = GBM_FORMAT_RGBX1010102; //??HDR  
	EGLint visualId = GBM_FORMAT_ARGB2101010;


  //  eglGetConfigs(display, configs, count, &count);
    
//    int i;
//    for (i = 0; i < count; i++) {
//	printConfigInfo(i, display, &configs[i]);
 //   }
    PrintConfigs(display);
	 
    EGLConfig config = NULL;
        
        
        
        
    if (!eglChooseConfig(display, configAttribs, configs, count, &matched) || !matched)
	{
        printf("No EGL configs with appropriate attributes.\n");
    }
       
    if (config_index == -1)
    {
        config_index = match_config_to_visual(display,
                                              visualId,
                                              configs,
                                              matched);
    }
        
    if (config_index != -1)
    {
        config = configs[config_index];
    }
        
        
        
 //       const EGLint contextAttribs[] = {
  //         EGL_CONTEXT_CLIENT_VERSION, 2,
  //          EGL_NONE};
    const EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,  //update to version 3.0, previously 2
		EGL_CONTEXT_MINOR_VERSION, 1,
        EGL_NONE
	};
			
    if(config)
    {
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (!context)
        {
            auto error = eglGetError();
            ofLogError() << "context ERROR: " << eglErrorString(error);
        }
	    const char *client_extensions = eglQueryString(display, EGL_EXTENSIONS);
				  
	    if (strstr(client_extensions, "EGL_EXT_gl_colorspace_bt2020_pq"))
		{
			ofLog() << "EGL_GL_COLORSPACE_BT2020_PQ_EXT available\n";
				  
		} else {
			ofLogError() << "EGL_GL_COLORSPACE_BT2020_PQ_EXT not available\n";
		}
		if (strstr(client_extensions, "EGL_KHR_gl_colorspace")) {
			ofLog() << "EGL_GL_COLORSPACE_KHR  available\n";
		} else {
			ofLogError() << "EGL_GL_COLORSPACE_KHR not available\n";
		}
		EGLint attribs[] = {EGL_GL_COLORSPACE_KHR,EGL_GL_COLORSPACE_BT2020_PQ_EXT,EGL_NONE };         

	
		PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceEXT = nullptr;
		const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
		if (extensions && (strstr(extensions, "EGL_KHR_platform_gbm") || strstr(extensions, "EGL_MESA_platform_gbm"))) {
			createPlatformWindowSurfaceEXT = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)
			eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
		}
		if (createPlatformWindowSurfaceEXT) {
			surface = createPlatformWindowSurfaceEXT(display, config, gbmSurface, attribs);
		} else {
			ofLog() << "No eglCreatePlatformWindowSurface for GBM, falling back to eglCreateWindowSurface\n" ;
			surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)gbmSurface, NULL);
		}



//for (uint32_t i = 0; i < 8; i++)
//{
//    eglSurfaceAttrib(display,surface, SurfaceAttribs[i],EGLint(DisplayChromacityList[1].ChromaVals[i] * EGL_METADATA_SCALING_EXT));
//}
#if 1
		eglSurfaceAttrib(display,surface, SurfaceAttribs[0],EGLint(DisplayChromacityList[2].RedX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[1],EGLint(DisplayChromacityList[2].RedY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[2],EGLint(DisplayChromacityList[2].GreenX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[3],EGLint(DisplayChromacityList[2].GreenY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[4],EGLint(DisplayChromacityList[2].BlueX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[5],EGLint(DisplayChromacityList[2].BlueY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[6],EGLint(DisplayChromacityList[2].WhiteX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[7],EGLint(DisplayChromacityList[2].WhiteY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[8],EGLint(10000.0f * 10000.0f));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[9],EGLint(0.001f    * 10000.0f));
#endif

	

        if (!surface)
        {
            auto error = eglGetError();
            ofLogError() << "surface ERROR: " << eglErrorString(error);
        }
            
        currentRenderer = make_shared<ofGLProgrammableRenderer>(this);
        makeCurrent();
        static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(3,1);



EGL_info();	
		ofLog() << "GBM: - initialized GBM";	
			
	} else {
        ofLog() << "RIP";
    }
	
	
}

void ofxRPI4Window::SDRWindowSetup()
{
	


int ret;

	if (!DestroyWindow()) 
	{
		ofLogError() << "GBM: Failed to deinitialize GBM";
	}
	
    gbmDevice = gbm_create_device(device);
	
	if (!gbmDevice)
	{
		ofLogError() << "GBM: - failed to create device: " << gbmDevice; 

	}
#if 1
#if defined(HAS_GBM_MODIFIERS)
	if (num_modifiers > 0)
	{
		gbmSurface = gbm_surface_create_with_modifiers(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB8888, modifiers,
                                                num_modifiers);
	}
#endif
	if (!gbmSurface)
	{
		gbmSurface = gbm_surface_create(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB8888,
									GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
	}

	if (!gbmSurface)
	{
		ofLogError() << "GBM: - failed to create surface: " << strerror(errno);

	} else {

		ofLog() << "GBM: - created surface with size " << mode.hdisplay << "x" << mode.vdisplay << " and " << ((*modifiers >= 0) ? "modifier " : "no modifier ") << hex << ((*modifiers >= 0) ? *modifiers : NULL);
	}
#else
    gbmSurface = gbm_surface_create(gbmDevice, mode.hdisplay, mode.vdisplay, GBM_FORMAT_ARGB8888 , GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

	if (!gbmSurface)
	{
		ofLogError() << "GBM: - failed to create surface: " << strerror(errno);

	} else {

		ofLog() << "GBM: - created surface with size " << mode.hdisplay << "x" << mode.vdisplay;
	}
#endif	
	display = gbm_get_display(gbmDevice);
    if (!display)
    {
       auto error = eglGetError();
       ofLogError() << "display ERROR: " << eglErrorString(error);
    }
        
    int major, minor;
    eglInitialize(display, &major, &minor);
    //eglBindAPI(EGL_OPENGL_API);
    eglBindAPI(EGL_OPENGL_ES_API);
        
    EGLint count = 0;
    EGLint matched = 0;
    int config_index = -1;
        
    if (!eglGetConfigs(display, NULL, 0, &count) || count < 1)
    {
        ofLogError() << "No EGL configs to choose from";
    }
    printf("EGL has %d configs\n", count);


	EGLConfig *configs = (EGLConfig *)malloc(count * sizeof *configs);
  //      EGLConfig configs[count];

	EGLint configAttribs[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
		EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_NONE
	};
	EGLint visualId = GBM_FORMAT_ARGB8888;

      

    PrintConfigs(display);
	 
    EGLConfig config = NULL;
        
        
        
        
        
    if (!eglChooseConfig(display, configAttribs, configs,
                             count, &matched) || !matched) {
        printf("No EGL configs with appropriate attributes.\n");
    }
        
    if (config_index == -1)
    {
        config_index = match_config_to_visual(display,
                                              visualId,
                                              configs,
                                              matched); 
	}
        
    if (config_index != -1)
	{
		config = configs[config_index];
    }
        
        
        
 //       const EGLint contextAttribs[] = {
  //         EGL_CONTEXT_CLIENT_VERSION, 2,
  //          EGL_NONE};
    const EGLint contextAttribs[] = {
		EGL_CONTEXT_MAJOR_VERSION, 3,  //update to version 3.0, previously 2
		EGL_CONTEXT_MINOR_VERSION, 1,
		EGL_NONE
	};
			
    if(config)
    {
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (!context)
        {
            auto error = eglGetError();
            ofLogError() << "context ERROR: " << eglErrorString(error);
        }
		const char *client_extensions = eglQueryString(display, EGL_EXTENSIONS);
				  

		if (strstr(client_extensions, "EGL_KHR_gl_colorspace")) {
			ofLog() << "EGL_GL_COLORSPACE_KHR  available\n";
		} else {
			ofLogError() << "EGL_GL_COLORSPACE_KHR not available\n";
		}
		EGLint attribs[] = {EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_SRGB_KHR, EGL_NONE };         
	
		PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceEXT = nullptr;
		const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
		if (extensions && (strstr(extensions, "EGL_KHR_platform_gbm") || strstr(extensions, "EGL_MESA_platform_gbm")))
		{
			createPlatformWindowSurfaceEXT = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)
			eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
		}
		if (createPlatformWindowSurfaceEXT) {
			surface = createPlatformWindowSurfaceEXT(display, config, gbmSurface, attribs);
		} else {
			ofLog() << "No eglCreatePlatformWindowSurface for GBM, falling back to eglCreateWindowSurface\n" ;
			surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)gbmSurface, NULL);
		}


//for (uint32_t i = 0; i < 8; i++)
//{
//    eglSurfaceAttrib(display,surface, SurfaceAttribs[i],EGLint(DisplayChromacityList[1].ChromaVals[i] * EGL_METADATA_SCALING_EXT));
//}
#if 1
		eglSurfaceAttrib(display,surface, SurfaceAttribs[0],EGLint(DisplayChromacityList[0].RedX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[1],EGLint(DisplayChromacityList[0].RedY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[2],EGLint(DisplayChromacityList[0].GreenX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[3],EGLint(DisplayChromacityList[0].GreenY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[4],EGLint(DisplayChromacityList[0].BlueX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[5],EGLint(DisplayChromacityList[0].BlueY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[6],EGLint(DisplayChromacityList[0].WhiteX * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[7],EGLint(DisplayChromacityList[0].WhiteY * EGL_METADATA_SCALING_EXT));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[8],EGLint(10000.0f * 10000.0f));
		eglSurfaceAttrib(display,surface, SurfaceAttribs[9],EGLint(0.001f    * 10000.0f));
#endif

	

        if (!surface)
        {
            auto error = eglGetError();
            ofLogError() << "surface ERROR: " << eglErrorString(error);
        }
            
        currentRenderer = make_shared<ofGLProgrammableRenderer>(this);
        makeCurrent();
        static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(3,1);
#if 0
		updateHDR_Infoframe(HDMI_EOTF_TRADITIONAL_GAMMA_SDR, 0);
		struct avi_infoframe avi_infoframe;
		avi_infoframe.colorspace = 0;
		avi_infoframe.output_format = 2;
		avi_infoframe.max_bpc = 8;
		avi_infoframe.c_enc = 1;
		avi_infoframe.c_range = 0;
		updateAVI_Infoframe(SDRplaneId, avi_infoframe);	
#endif
   EGL_info();  
		ofLog() << "GBM: - initialized GBM";

        }else
        {
            ofLog() << "RIP";
        }
        
        
        
        
 //   }
    
    
    
    
    
    
    
}







void ofxRPI4Window::makeCurrent()
{
    eglMakeCurrent(display, surface, surface, context);
    
}

void ofxRPI4Window::update()
{
    //ofLog() << "update";
    coreEvents.notifyUpdate();
	if (flip) {
		if (isHDR && !isDolby)
		{ 
			HDRWindowSetup();
		} else if (isHDR && isDolby) {
			DoViWindowSetup();	
		} else {
			SDRWindowSetup();
		}
	}
}

int ofxRPI4Window::getWidth()
{
    //ofLog() << __func__ << currentWindowRect.width;
    return currentWindowRect.width;
}

int ofxRPI4Window::getHeight()
{
    //ofLog() << __func__ << currentWindowRect.height;
    
    return currentWindowRect.height;
}

glm::vec2 ofxRPI4Window::getScreenSize()
{
    
    //ofLog() << __func__;
    return {currentWindowRect.getWidth(), currentWindowRect.getHeight()};
}

glm::vec2 ofxRPI4Window::getWindowSize()
{
    //ofLog() << __func__;
    return {currentWindowRect.width, currentWindowRect.height};
}

//------------------------------------------------------------
glm::vec2 ofxRPI4Window::getWindowPosition(){
    //ofLog() << __func__;
    return glm::vec2(currentWindowRect.getPosition());
}



static void drm_fb_destroy_callback(struct gbm_bo *bo, void *data)
{
	drm_fb *fb = static_cast<drm_fb*>(data);

	if (fb->fb_id > 0)
	{
	   int drm_fd = gbm_device_get_fd(gbm_bo_get_device(bo));
	   drmModeRmFB(drm_fd, fb->fb_id);
	}
	free(fb);
}

drm_fb * ofxRPI4Window::drm_fb_get_from_bo(struct gbm_bo *bo)
{
	int drm_fd = gbm_device_get_fd(gbm_bo_get_device(bo));
  //{
//	struct drm_fb *fb = static_cast<drm_fb*>(gbm_bo_get_user_data(bo));

  //  if(fb)
  //  {
   //   if (gbm_bo_get_format(bo) == fb->format)
 //       return fb;
   //   else
   //     drm_fb_destroy_callback(bo, gbm_bo_get_user_data(bo));
  //  }
 // }
 struct drm_fb *fb = new drm_fb;
  fb->bo = bo;
  fb->format = gbm_bo_get_format(bo);

  uint32_t width,
           height,
           handles[4] = {0},
           strides[4] = {0},
           offsets[4] = {0};

  uint64_t modifiers[4] = {0};
  
   
	width = gbm_bo_get_width(bo);
	height = gbm_bo_get_height(bo);

#if defined(HAS_GBM_MODIFIERS)
  for (int i = 0; i < gbm_bo_get_plane_count(bo); i++)
  {
    handles[i] = gbm_bo_get_handle_for_plane(bo, i).u32;
    strides[i] = gbm_bo_get_stride_for_plane(bo, i);
    offsets[i] = gbm_bo_get_offset(bo, i);
    modifiers[i] = gbm_bo_get_modifier(bo);
  }
#else
  handles[0] = gbm_bo_get_handle(bo).u32;
  strides[0] = gbm_bo_get_stride(bo);
  memset(offsets, 0, 16);
#endif

  uint32_t flags = 0;

	  if (modifiers[0] && modifiers[0] != DRM_FORMAT_MOD_INVALID)
  {
    flags |= DRM_MODE_FB_MODIFIERS;
   printf("%s - using modifier: {:%llx}\n", __func__, modifiers[0]);
  }
	
 int ret = drmModeAddFB2WithModifiers(drm_fd,
                                       width,
                                       height,
                                       fb->format,
                                       handles,
                                       strides,
                                       offsets,
                                       modifiers,
                                       &fb->fb_id,
                                       flags);

  if(ret < 0)
  {
	if (flags)
			ofLogError() << "DRM: Modifiers failed!";
		
    ret = drmModeAddFB2(drm_fd,
                        width,
                        height,
                        fb->format,
                        handles,
                        strides,
                        offsets,
                        &fb->fb_id,
                        flags);

 if (ret < 0)
    {
      delete (fb);
      ofLogError() << "DRM: - failed to add framebuffer " <<  strerror(errno) << "  " << errno;
      return nullptr;
    }
  }

  gbm_bo_set_user_data(bo, fb, drm_fb_destroy_callback);

  return fb;
}

void ofxRPI4Window::swapBuffers()
{
    // ofLog() << __func__;
  
    EGLBoolean success = eglSwapBuffers(display, surface);
    if(!success) {
        GLint error = eglGetError();
        ofLog() << "eglSwapBuffers failed: " << eglErrorString(error);
    }
    struct gbm_bo *bo = gbm_surface_lock_front_buffer(gbmSurface);
	if (!bo) {
			ofLogError() << "GBM: Failed to lock frontbuffer";

	}

	struct drm_fb *fb = drm_fb_get_from_bo(bo);
	if (!fb) {
		ofLogError() << "DRM: Failed to get a new framebuffer BO";
	}

    		
  //  drmModeSetCrtc(device, crtc->crtc_id, fb->fb_id, 0, 0, &connectorId, 1, &mode);   
     FlipPage(flip, isHDR, isDolby, fb->fb_id);
     		flip = false;  //change to flags 
					/* Allow a modeset change for the first commit only. */
		//flags &= ~(DRM_MODE_ATOMIC_ALLOW_MODESET);

    if (previousBo)
    {
        drmModeRmFB(device, previousFb);
        gbm_surface_release_buffer(gbmSurface, previousBo);

    }
    previousBo = bo;
    previousFb = fb->fb_id;
    
}


void ofxRPI4Window::startRender()
{
   //ofLog() << __func__;
    glEnable(GL_DEPTH_TEST);
	if (!isHDR) 
		glEnable(GL_FRAMEBUFFER_SRGB_EXT);
    renderer()->startRender();
}

void ofxRPI4Window::finishRender()
{
    //ofLog() << __func__;
    renderer()->finishRender();
}


float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int ofxRPI4Window::CreateFB_ID()
{
int ret;
	/* Request a dumb buffer */
	struct drm_mode_create_dumb create_request = {
		mode.vdisplay, //height
		mode.hdisplay, //width
		32		//bpp
	};
	ret = ioctl(device, DRM_IOCTL_MODE_CREATE_DUMB, &create_request);

	/* Bail out if we could not allocate a dumb buffer */
	if (ret) {
		printf(
			"Dumb Buffer Object Allocation request of %ux%u@%u failed : %s\n",
			create_request.width, create_request.height,
			create_request.bpp,
			strerror(ret)
		);
	//	goto could_not_allocate_buffer;
	}


ret = drmModeRmFB(device, crtc->buffer_id);

  if (ret) {
    printf("drmModeRmFB failed for fb_id %d with error %d\n", crtc->buffer_id, ret);
  }
	/* create framebuffer object for the dumb-buffer */
	uint32_t bo_handles[4] = { create_request.handle };
	uint32_t pitches[4] = { create_request.pitch };
	uint32_t offsets[4] = { 0 };
	 uint32_t flags = 0;
uint64_t modifier[4] = { modifiers[1], modifiers[1] };
	uint32_t frame_buffer_id;
	
	if (modifiers[0] && modifiers[0] != DRM_FORMAT_MOD_INVALID) 
	{
		uint32_t flags = DRM_MODE_FB_MODIFIERS;
		// modifier[4]  = 
	}

	ret = drmModeAddFB2WithModifiers(
		device,
		mode.hdisplay, mode.vdisplay,
		DRM_FORMAT_ARGB8888, bo_handles,
		pitches, offsets, modifier, &frame_buffer_id, flags
	);

	/* Without framebuffer, we won't do anything so bail out ! */
	if (ret) {
		printf(
			"Could not add a framebuffer using drmModeAddFB2 : %s\n",
			strerror(ret)
		);
	//	goto could_not_add_frame_buffer;
	}

	/* We assume that the currently chosen encoder CRTC ID is the current
	 * one.
	 */
	uint32_t current_crtc_id = crtc->crtc_id;

	if (!current_crtc_id) {
		printf("The retrieved encoder has no CRTC attached... ?\n");
		//goto could_not_retrieve_current_crtc;
	}

	/* Backup the informations of the CRTC to restore when we're done.
	 * The most important piece seems to currently be the buffer ID.
	 */
	drmModeCrtc * __restrict crtc_to_restore =
		drmModeGetCrtc(device, current_crtc_id);

	if (!crtc_to_restore) {
		printf("Could not retrieve the current CRTC with a valid ID !\n");
	//	goto could_not_retrieve_current_crtc;
	}

	/* Set the CRTC so that it uses our new framebuffer */
	ret = drmModeSetCrtc(
		device, current_crtc_id, frame_buffer_id,
		0, 0,
		&connectorId,
		1,
		&mode);

	/* For this test only : Export our dumb buffer using PRIME */
	/* This will provide us a PRIME File Descriptor that we'll use to
	 * map the represented buffer. This could be also be used to reimport
	 * the GEM buffer into another GPU */
	struct drm_prime_handle prime_request = {
		create_request.handle, //handle
		DRM_CLOEXEC | DRM_RDWR, //flags
		-1 //fd
	};

	ret = ioctl(device, DRM_IOCTL_PRIME_HANDLE_TO_FD, &prime_request);
	int const dma_buf_fd = prime_request.fd;

	/* If we could not export the buffer, bail out since that's the
	 * purpose of our test */
	if (ret || dma_buf_fd < 0) {
		printf(
			"Could not export buffer : %s (%d) - FD : %d\n",
			strerror(ret), ret,
			dma_buf_fd
 		);
	//	goto could_not_export_buffer;
	}

	/* Map the exported buffer, using the PRIME File descriptor */
	/* That ONLY works if the DRM driver implements gem_prime_mmap.
	 * This function is not implemented in most of the DRM drivers for 
	 * GPU with discrete memory. Meaning that it will surely fail with
	 * Radeon, AMDGPU and Nouveau drivers for desktop cards ! */
	uint8_t * primed_framebuffer = static_cast<uint8_t*>(mmap(
		0, create_request.size,	PROT_READ | PROT_WRITE, MAP_SHARED,
		dma_buf_fd, 0));
	ret = errno;

	/* Bail out if we could not map the framebuffer using this method */
	if (primed_framebuffer == NULL || primed_framebuffer == MAP_FAILED) {
		printf(
			"Could not map buffer exported through PRIME : %s (%d)\n"
			"Buffer : %p\n",
			strerror(ret), ret,
			primed_framebuffer
		);
		//goto could_not_map_buffer;
	}
	

	printf("Buffer mapped !\n");
return frame_buffer_id;
}

void ofxRPI4Window::DisablePlane(uint32_t plane_id)
{
  bool ok;

 			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "CRTC_ID",
					&prop_id, NULL, &prop);
	first_req = 1;
 	drm_mode_atomic_set_property(device, req, "CRTC_ID" /* in */, plane_id/* in */,
					prop_id /* in */, 0 /* in */, prop /* in */);
					
			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "FB_ID",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "FB_ID" /* in */, plane_id/* in */,
					prop_id /* in */, 0/* in */, prop /* in */);  //value can also be crtc->buffer_id ** FB id to connect to


  //disable Colorspace

		ok = drm_mode_get_property(device, connectorId,
				DRM_MODE_OBJECT_CONNECTOR, "Colorspace",
				&prop_id, &colorspace, &prop);

		if (!ok || !(colorspace >= 0)) {
			ofLogError() << "Unable to find Colorspace";
		} else {
		    /* set colorspace to Default = 9 */
			colorspace = 0; //TODO: Set this from web interface
 //      last_req = 1;
	drm_mode_atomic_set_property(device, req, "Colorspace" /* in */, connectorId/* in */,
			prop_id /* in */, colorspace /* in */, prop /* in */);

        }	
		
	int ret;

  //disable HDR Metadata
	ok = drm_mode_get_property(device, connectorId,
			DRM_MODE_OBJECT_CONNECTOR, "HDR_OUTPUT_METADATA",
			&prop_id, &blob_id, &prop);
	if (!ok || !blob_id)
		ofLogError() << "Unable find or HDR_OUTPUT_METADATA not set";


    if (blob_id) {
      drmModeDestroyPropertyBlob(device, blob_id);
      blob_id = 0;
    }

	drm_mode_atomic_set_property(device, req, "HDR_OUTPUT_METADATA" /* in */, connectorId/* in */,
			prop_id /* in */, blob_id /* in */, prop /* in */);
			
  //disable DOVI Metadata
	ok = drm_mode_get_property(device, connectorId,
			DRM_MODE_OBJECT_CONNECTOR, "DOVI_OUTPUT_METADATA",
			&prop_id, &blob_id, &prop);
	if (!ok || !blob_id)
		ofLogError() << "Unable find or DOVI_OUTPUT_METADATA not set";
					

    if (blob_id) {
      drmModeDestroyPropertyBlob(device, blob_id);
      blob_id = 0;
    }
last_req = 1;
	drm_mode_atomic_set_property(device, req, "DOVI_OUTPUT_METADATA" /* in */, connectorId/* in */,
			prop_id /* in */, blob_id /* in */, prop /* in */);
}

int ofxRPI4Window::SetPlaneId(int isHDR)
{
	if (isHDR)
		return HDRplaneId;
	else
		return SDRplaneId; 
}
 
void ofxRPI4Window::FlipPage(bool flip, int isHDR, int isDolby, uint32_t fb_id)
{

	if (flip) {
		if (isHDR && !isDolby)
		{  
			// disable plane when working layer no longer is active
			DisablePlane(SDRplaneId);
			SetActivePlane(HDRplaneId, currentWindowRect, fb_id);
			updateHDR_Infoframe(HDMI_EOTF_SMPTE_ST2084, 1); // Display Gamut Rec2020
			struct avi_infoframe avi_infoframe;
			avi_infoframe.colorspace = 9; //BT2020_RGB
			avi_infoframe.output_format = 2; //YCrCb422
			avi_infoframe.max_bpc = 12; //12 bit
			avi_infoframe.c_enc = 2; //ITU-R BT.2020 YCbCr
			avi_infoframe.c_range = 1; //YCbCr full range
			updateAVI_Infoframe(HDRplaneId, avi_infoframe);	
#if 1
		} else if (isHDR && isDolby) {
			DisablePlane(SDRplaneId);
			SetActivePlane(HDRplaneId, currentWindowRect, fb_id);
						updateHDR_Infoframe(HDMI_EOTF_SMPTE_ST2084, 2); // Display Gamut P3D65
			updateDoVi_Infoframe(HDMI_EOTF_SMPTE_ST2084, 2); // Display Gamut P3D65
			struct avi_infoframe avi_infoframe;
			avi_infoframe.colorspace = 10; //BT2020_YCC
			avi_infoframe.output_format = 2; //YCrCb422, doesnt work with YCrCb420
			avi_infoframe.max_bpc = 12; // 12 bit
			avi_infoframe.c_enc = 2; //ITU-R BT.2020 YCbCr
			avi_infoframe.c_range = 1; //YCbCr full range
			updateAVI_Infoframe(HDRplaneId, avi_infoframe);	
#endif
		} else {
			DisablePlane(HDRplaneId);
			SetActivePlane(SDRplaneId, currentWindowRect, fb_id);
			updateHDR_Infoframe(HDMI_EOTF_TRADITIONAL_GAMMA_SDR, 0); // Display Gamut Rec709
			struct avi_infoframe avi_infoframe;
			avi_infoframe.colorspace = 0; //Default
			avi_infoframe.output_format = 2; //YCrCb422
			avi_infoframe.max_bpc = 8; //8 bit
			avi_infoframe.c_enc = 1; //ITU-R BT.709 YCbCr
			avi_infoframe.c_range = 0; //YCbCr limited range
			updateAVI_Infoframe(SDRplaneId, avi_infoframe);	
		}
		

	}

	if (drmModeSetPlane(device, SetPlaneId(isHDR), crtc->crtc_id,
		    fb_id, 0, crtc->x, crtc->y,
		    crtc->width, crtc->height, 0, 0,
		    ((int)currentWindowRect.width << 16), ((int)currentWindowRect.height << 16)))
	{
		ofLogError() << "DRM: -failed to enable plane";
	
	}

}

void ofxRPI4Window::SetActivePlane(uint32_t plane_id, ofRectangle currentWindowRect, int fb_id)
{
	bool ok;
    uint64_t blob_id, value;

		ok = drm_mode_get_property(device, connectorId,
				DRM_MODE_OBJECT_CONNECTOR, "CRTC_ID",
				&prop_id, &value, &prop);

		if (!ok || !value)
			ofLogError() << "DRM: Unable find CRTC_ID";
				//	output_format = avi_infoframe.output_format;


	first_req = 1;
	drm_mode_atomic_set_property(device, req, "CRTC_ID" /* in */, connectorId/* in */,
			prop_id /* in */, value /* in */, prop /* in */);
			
drmModeCreatePropertyBlob(device, &mode, sizeof(mode),
					      (uint32_t*)&blob_id);		
			
		ok = drm_mode_get_property(device, crtc->crtc_id,
				DRM_MODE_OBJECT_CRTC, "MODE_ID",
				&prop_id, &blob_id, &prop);

		if (!ok || !blob_id)
			ofLogError() << "DRM: Unable find MODE_ID";


	drm_mode_atomic_set_property(device, req, "MODE_ID" /* in */, crtc->crtc_id/* in */,
			prop_id /* in */, blob_id /* in */, prop /* in */);
			
		ok = drm_mode_get_property(device, crtc->crtc_id,
				DRM_MODE_OBJECT_CRTC, "ACTIVE",
				&prop_id, NULL, &prop);

		if (!ok)
			ofLogError() << "DRM: Unable find ACTIVE";



	drm_mode_atomic_set_property(device, req, "ACTIVE" /* in */, crtc->crtc_id/* in */,
			prop_id /* in */, 1 /* in */, prop /* in */);

 
 			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "CRTC_ID",
					&prop_id, NULL, &prop);

 	drm_mode_atomic_set_property(device, req, "CRTC_ID" /* in */, plane_id/* in */,
					prop_id /* in */, crtc->crtc_id /* in */, prop /* in */);
					
			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "FB_ID",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "FB_ID" /* in */, plane_id/* in */,
					prop_id /* in */, fb_id /* in */, prop /* in */);  //value can also be crtc->buffer_id ** FB id to connect to

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "SRC_X",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "SRC_X" /* in */, plane_id/* in */,
					prop_id /* in */, 0 /* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "SRC_Y",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "SRC_Y" /* in */, plane_id/* in */,
					prop_id /* in */, 0 /* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "SRC_W",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "SRC_W" /* in */, plane_id/* in */,
					prop_id /* in */, ((int)currentWindowRect.width << 16) /* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "SRC_H",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "SRC_H" /* in */, plane_id/* in */,
					prop_id /* in */, ((int)currentWindowRect.height << 16) /* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "CRTC_X",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "CRTC_X" /* in */, plane_id/* in */,
					prop_id /* in */, crtc->x/* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "CRTC_Y",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "CRTC_Y" /* in */, plane_id/* in */,
					prop_id /* in */, crtc->y/* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "CRTC_W",
					&prop_id, NULL, &prop);
	drm_mode_atomic_set_property(device, req, "CRTC_W" /* in */, plane_id/* in */,
					prop_id /* in */, crtc->width /* in */, prop /* in */);

			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "CRTC_H",
					&prop_id, NULL, &prop);
	last_req = 1;
	drm_mode_atomic_set_property(device, req, "CRTC_H" /* in */, plane_id/* in */,
					prop_id /* in */, crtc->height /* in */, prop /* in */);

}
void ofxRPI4Window::updateHDR_Infoframe(hdmi_eotf eotf, int idx)
{
	bool ok;
	int ret;
	drmModePropertyBlobRes *blob;


	ok = drm_mode_get_property(device, connectorId,
			DRM_MODE_OBJECT_CONNECTOR, "HDR_OUTPUT_METADATA",
			&prop_id, &blob_id, &prop);
	if (!ok || !blob_id)
		ofLogError() << "Unable find or HDR_OUTPUT_METADATA not set";
#if 0	
	if (blob_id)
	{
		blob = drmModeGetPropertyBlob(device, blob_id);
		if (!blob) {
			ofLogError() << "Could not get drmModeGetPropertyBlob";

		}
    }
#endif	
	if (blob_id)
      drmModeDestroyPropertyBlob(device, blob_id);
    blob_id = 0;
	
	struct drm_hdr_output_metadata meta; //= static_cast<drm_hdr_output_metadata*>(blob->data);
						//memset(meta, 0, sizeof(*meta));



	//					if (meta->metadata_type == 0 /*HDMI_STATIC_METADATA_TYPE1*/) {
						//	struct drm_hdr_metadata_infoframe* info = &meta->hdmi_metadata_type1;

	meta.metadata_type = static_cast<int>(HDMI_STATIC_METADATA_TYPE1);
	meta.hdmi_metadata_type1.eotf = eotf;
	meta.hdmi_metadata_type1.display_primaries[0].x = (DisplayChromacityList[idx].RedX * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.display_primaries[0].y = (DisplayChromacityList[idx].RedY * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.display_primaries[1].x = (DisplayChromacityList[idx].GreenX * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.display_primaries[1].y = (DisplayChromacityList[idx].GreenY * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.display_primaries[2].x = (DisplayChromacityList[idx].BlueX * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.display_primaries[2].y = (DisplayChromacityList[idx].BlueY * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.white_point.x = (DisplayChromacityList[idx].WhiteX * EGL_METADATA_SCALING_EXT);
	meta.hdmi_metadata_type1.white_point.y = (DisplayChromacityList[idx].WhiteY * EGL_METADATA_SCALING_EXT);
//	meta.hdmi_metadata_type1.max_display_mastering_luminance = std::round(av_q2d(10000.0f * 10000.0f));
//	meta.hdmi_metadata_type1.min_display_mastering_luminance = std::round(av_q2d((0.001f    * 10000.0f));
	meta.hdmi_metadata_type1.max_display_mastering_luminance = (uint16_t)(10000.0f * 10000.0f);
	meta.hdmi_metadata_type1.min_display_mastering_luminance = (uint16_t)(0.001f    * 10000.0f);
		
	meta.hdmi_metadata_type1.max_fall = 400.0f;
	meta.hdmi_metadata_type1.max_cll = 10000.0f;
	drmModeCreatePropertyBlob(device, &meta, sizeof(meta), (uint32_t*)&blob_id); 
			
	
#if 0
	ret = drmModeConnectorSetProperty(
		device,
		connectorId,
		prop_id, blob_id );

#endif

first_req = 1;
last_req = 1;
//	drm_mode_atomic_set_property(device, req, "DOVI_OUTPUT_METADATA" /* in */, connectorId/* in */,
//			prop_id /* in */, blob_id /* in */, prop /* in */);
	drm_mode_atomic_set_property(device, req, "HDR_OUTPUT_METADATA" /* in */, connectorId/* in */,
			prop_id /* in */, blob_id /* in */, prop /* in */);
#if 0
	if (ret != 0) {
		ofLogError() << "Unable to set HDR_OUTPUT_METADATA";
	//	return false;
	} else {
//		hdr_output_metadata_info(device,blob_id);
		dovi_output_metadata_info(device,blob_id);

	//	return true;
	}
#endif
//	drmModeFreePropertyBlob(blob);	 	


}

void ofxRPI4Window::updateDoVi_Infoframe(hdmi_eotf eotf, int idx) //
{
	bool ok;
	int ret;
	drmModePropertyBlobRes *blob;

#if 0
	ok = drm_mode_get_property(device, connectorId,
			DRM_MODE_OBJECT_CONNECTOR, "HDR_OUTPUT_METADATA",
			&prop_id, &blob_id, &prop);
	if (!ok || !blob_id)
		ofLogError() << "Unable find or HDR_OUTPUT_METADATA not set";

	if (blob_id)
      drmModeDestroyPropertyBlob(device, blob_id);
    blob_id = 0;
#endif

	ok = drm_mode_get_property(device, connectorId,
			DRM_MODE_OBJECT_CONNECTOR, "DOVI_OUTPUT_METADATA",
			&prop_id, &blob_id, &prop);
	if (!ok || !blob_id)
		ofLogError() << "Unable find or DOVI_OUTPUT_METADATA not set";
	
	if (blob_id)
      drmModeDestroyPropertyBlob(device, blob_id);
    blob_id = 0;	
	
	struct dovi_output_metadata dovi;
	 
	dovi.dv_status = 1;
	dovi.dv_interface = 1; 
	dovi.backlight_metadata = 0;
	dovi.backlight_max_luminance = 0;
	dovi.aux_runmode = 0;
	dovi.aux_version = 0;
	dovi.aux_debug = 0;
		drmModeCreatePropertyBlob(device, &dovi, sizeof(dovi), (uint32_t*)&blob_id); 
first_req = 1;
last_req = 1;
	drm_mode_atomic_set_property(device, req, "DOVI_OUTPUT_METADATA" /* in */, connectorId/* in */,
			prop_id /* in */, blob_id /* in */, prop /* in */);
//	drm_mode_atomic_set_property(device, req, "HDR_OUTPUT_METADATA" /* in */, connectorId/* in */,
//			prop_id /* in */, blob_id /* in */, prop /* in */);

//	drmModeFreePropertyBlob(blob);	 	


}
bool ofxRPI4Window::updateAVI_Infoframe(uint32_t plane_id, struct avi_infoframe avi_infoframe)
{
	bool ok;
		ok = drm_mode_get_property(device, connectorId,
				DRM_MODE_OBJECT_CONNECTOR, "Colorspace",
				&prop_id, &colorspace, &prop);

		if (!ok || !(colorspace >= 0)) {
			ofLogError() << "Unable to find Colorspace";
		} else {
		    /* set colorspace to BT2020_RGB = 9 */
			colorspace = avi_infoframe.colorspace; //TODO: Set this from web interface
first_req = 1;
	drm_mode_atomic_set_property(device, req, "Colorspace" /* in */, connectorId/* in */,
			prop_id /* in */, colorspace /* in */, prop /* in */);

        }			
			



		ok = drm_mode_get_property(device, connectorId,
				DRM_MODE_OBJECT_CONNECTOR, "output format",
				&prop_id, &output_format, &prop);

		if (!ok || !output_format)
			ofLogError() << "DRM: Unable find OUTPUT FORMAT";
					output_format = avi_infoframe.output_format;



	drm_mode_atomic_set_property(device, req, "output format" /* in */, connectorId/* in */,
			prop_id /* in */, output_format /* in */, prop /* in */);

		ok = drm_mode_get_property(device, connectorId,
				DRM_MODE_OBJECT_CONNECTOR, "max bpc",
				&prop_id, &max_bpc, &prop);

		if (!ok || !max_bpc) {
			ofLogError() << "DRM: Unable find MAX_BPC";
		} else {
		
			max_bpc = avi_infoframe.max_bpc;

		drm_mode_atomic_set_property(device, req, "max bpc" /* in */, connectorId/* in */,
			prop_id /* in */, max_bpc /* in */, prop /* in */);
		}
	

			
#if 0

    res	= drmModeGetPlaneResources(device);
	if (!res) {
		ofLogError() << "DRM: Unable to get drmModeGetPlaneResources";
	//	return NULL;
	}

	for (uint32_t i = 0; i < res->count_planes; i++) {
		plane = drmModeGetPlane(device, res->planes[i]);
		if (!plane) {
			ofLogError() << "DRM: Unable to get drmModeGetPlane";
			continue;
		}


	if (plane->possible_crtcs & (1 << crtc_index)) {
#endif 		    /* set COLOR_ENCODING plane property */
			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "COLOR_ENCODING",
					&prop_id, &c_enc, &prop);

			if (!ok || !(c_enc >= 0)) {
				ofLogError() << "DRM: Unable find COLOR_ENCODING";
			} else {
				c_enc = avi_infoframe.c_enc; //set to ITU-R BT.2020 YCbCr

				drm_mode_atomic_set_property(device, req, "COLOR_ENCODING" /* in */, plane_id/* in */,
					prop_id /* in */, c_enc /* in */, prop /* in */);
			}
			/* set COLOR_RANGE plane property */
			ok = drm_mode_get_property(device, plane_id,
					DRM_MODE_OBJECT_PLANE, "COLOR_RANGE",
					&prop_id, &c_range, &prop);
			if (!ok || !(c_range >= 0)) {
				ofLogError() << "DRM: Unable find COLOR_RANGE";
			} else {
				c_range = avi_infoframe.c_range; //set to YCbCr full range	
last_req = 1;
				drm_mode_atomic_set_property(device, req, "COLOR_RANGE" /* in */, plane_id/* in */,
				prop_id /* in */, c_range /* in */, prop /* in */);
			}

	


	return true;
}

void ofxRPI4Window::draw()
{
    
//ofLog() << getInfo();
    
    int waiting_for_flip = 1;
    auto startFrame = ofGetElapsedTimeMillis();
    gettimeofday(&t0, 0);
  //TODO: Add user input to change HDR infoframe and AVI infoframe prior to drawing.    
	   
    if(skipRender)
    {
        
        coreEvents.notifyDraw();
        
        swapBuffers();
        
    }else
    {
        currentRenderer->startRender();
        if( bEnableSetupScreen )
        {
            currentRenderer->setupScreen();
            //bEnableSetupScreen = false;
        }
        
        
        coreEvents.notifyDraw();
        currentRenderer->finishRender();
        swapBuffers();
    }

 
    gettimeofday(&t1, 0);
    
    lastFrameTimeMillis = timedifference_msec(t0, t1);
    
    //printf("Code executed in %f milliseconds.\n", elapsed);
    
    
    
    
}

void ofxRPI4Window::setWindowShape(int w, int h)
{
    currentWindowRect = ofRectangle(currentWindowRect.x,currentWindowRect.y, w, h);
}

void ofxRPI4Window::pollEvents()
{
    //ofLog() << "pollEvents";
    
}

ofCoreEvents & ofxRPI4Window::events(){
    return coreEvents;
}


void ofxRPI4Window::enableSetupScreen(){
    bEnableSetupScreen = true;
}

//------------------------------------------------------------
void ofxRPI4Window::disableSetupScreen(){
    bEnableSetupScreen = false;
}

void ofxRPI4Window::setVerticalSync(bool enabled)
{
    eglSwapInterval(display, enabled ? 1 : 0);
}

EGLDisplay ofxRPI4Window::getEGLDisplay()
{
    //ofLog() << __func__;
    return display;
}

EGLContext ofxRPI4Window::getEGLContext()
{
    //ofLog() << __func__;
    
    return context;
}

EGLSurface ofxRPI4Window::getEGLSurface()
{
    //ofLog() << __func__;
    
    return surface;
}

shared_ptr<ofBaseRenderer> & ofxRPI4Window::renderer(){
    
    //ofLog() << __func__;
    
    return currentRenderer;
}

string ofxRPI4Window::getInfo()
{
    
    
    stringstream info;
    
    info << "ofGetFrameRate(): " << ofGetFrameRate() << endl;
    info << "ofGetLastFrameTime(): " << ofGetLastFrameTime() << endl;
    info << "lastFrameTimeMillis: " << lastFrameTimeMillis << endl;

    info << "ofGetWidth(): " << ofGetWidth() << endl;
    info << "ofGetHeight(): " << ofGetHeight()<< endl;
    info << "ofGetScreenHeight(): " << ofGetScreenHeight()<< endl;
    info << "ofGetScreenWidth(): " << ofGetScreenWidth()<< endl;
    info << "ofGetWindowWidth(): " << ofGetWindowWidth()<< endl;
    info << "ofGetWindowHeight(): " << ofGetWindowHeight()<< endl;
    info << "ofGetWindowPositionX(): " << ofGetWindowPositionX()<< endl;
    info << "ofGetWindowPositionY(): " << ofGetWindowPositionY()<< endl;
    info << "ofGetWindowRect(): " << ofGetWindowRect()<< endl;
    
    return info.str();
}

bool ofxRPI4Window::DestroyWindow()
{
//gbmClean();
  DestroyContext();
  DestroySurface();

  if (display != EGL_NO_DISPLAY)
  {
    eglTerminate(display);
    display = EGL_NO_DISPLAY;
  }
  ofLog() << "GBM: - deinitialized GBM";
  return true;
}

void ofxRPI4Window::DestroyContext()
{
  if (context != EGL_NO_CONTEXT)
  {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    context = EGL_NO_CONTEXT;
  }
}

void ofxRPI4Window::DestroySurface()
{
  if (surface != EGL_NO_SURFACE)
  {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(display, surface);
    surface = EGL_NO_SURFACE;
  }
}
void ofxRPI4Window::gbmClean()
{
    // set the previous crtc
    drmModeSetCrtc(device, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y, &connectorId, 1, &crtc->mode);
    drmModeFreeCrtc(crtc);
#if 0
	drm_mode_atomic_set_property(device, req, "max bpc" /* in */, connectorId/* in */,
			property_id.max_bpc /* in */, 8 /* in */, prop /* in */);    

	drm_mode_atomic_set_property(device, req, "Colorspace" /* in */, connectorId/* in */,
			property_id.colorspace /* in */, 0 /* in */, prop /* in */);

	drm_mode_atomic_set_property(device, req, "output format" /* in */, connectorId/* in */,
			property_id.output_format /* in */, 0 /* in */, prop /* in */);
			

	drm_mode_atomic_set_property(device, req, "COLOR_ENCODING" /* in */, plane->plane_id/* in */,
			property_id.c_enc /* in */, 0 /* in */, prop /* in */);

	drm_mode_atomic_set_property(device, req, "COLOR_RANGE" /* in */, plane->plane_id/* in */,
			property_id.c_range /* in */, 0 /* in */, prop /* in */);
#endif			
    if (previousBo)
    {
        drmModeRmFB(device, previousFb);
        gbm_surface_release_buffer(gbmSurface, previousBo);
    }
    
    gbm_surface_destroy(gbmSurface);
    gbm_device_destroy(gbmDevice);
}


ofxRPI4Window::~ofxRPI4Window()
{
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
    gbmClean();
    if (drmAuthMagic(device, 0) == EINVAL)
    drmDropMaster(device);
    ::close(device); 
}



