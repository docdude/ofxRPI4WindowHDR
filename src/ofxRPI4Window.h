#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofRectangle.h"
#include "ofGLProgrammableRenderer.h"

#include <stdio.h> // sprintf
#include <stdlib.h>  // malloc
#include <fcntl.h>  // open fcntl
#include <unistd.h> // read close
#include <string.h> // strlen
#include <sys/time.h>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>
#include <gbm.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>


#ifndef EGL_KHR_platform_gbm
#define EGL_KHR_platform_gbm 1
#define EGL_PLATFORM_GBM_KHR              0x31D7
#endif /* EGL_KHR_platform_gbm */
#ifndef EGL_EXT_gl_colorspace_bt2020_pq
#define EGL_EXT_gl_colorspace_bt2020_pq 1
#define EGL_GL_COLORSPACE_BT2020_PQ_EXT   0x3340
#endif /* EGL_EXT_gl_colorspace_bt2020_pq */

#ifndef DRM_FORMAT_NV15
#define DRM_FORMAT_NV15 fourcc_code('N', 'V', '1', '5')
#endif

#ifndef DRM_FORMAT_NV20
#define DRM_FORMAT_NV20 fourcc_code('N', 'V', '2', '0')
#endif

// P030 should be defined in drm_fourcc.h and hopefully will be sometime
// in the future but until then...
#ifndef DRM_FORMAT_P030
#define DRM_FORMAT_P030 fourcc_code('P', '0', '3', '0')
#endif

#ifndef DRM_FORMAT_NV15
#define DRM_FORMAT_NV15 fourcc_code('N', 'V', '1', '5')
#endif

#ifndef DRM_FORMAT_NV20
#define DRM_FORMAT_NV20 fourcc_code('N', 'V', '2', '0')
#endif

// V4L2_PIX_FMT_NV12_10_COL128 and V4L2_PIX_FMT_NV12_COL128 should be defined
// in drm_fourcc.h hopefully will be sometime in the future but until then...
#ifndef V4L2_PIX_FMT_NV12_10_COL128
#define V4L2_PIX_FMT_NV12_10_COL128 v4l2_fourcc('N', 'C', '3', '0')
#endif

#ifndef V4L2_PIX_FMT_NV12_COL128
#define V4L2_PIX_FMT_NV12_COL128 v4l2_fourcc('N', 'C', '1', '2') /* 12  Y/CbCr 4:2:0 128 pixel wide column */
#endif

#ifndef HAS_GBM_MODIFIERS
#define HAS_GBM_MODIFIERS
#endif

typedef EGLDisplay (EGLAPIENTRYP PFNEGLGETPLATFORMDISPLAYEXTPROC) (EGLenum platform, void *native_display, const EGLint *attrib_list);
//EGLAPI EGLDisplay EGLAPIENTRY eglGetPlatformDisplayEXT (EGLenum platform, void *native_display, const EGLint *attrib_list);

using namespace std;

/* HDR EDID parsing. */
#define CTA_EXTENSION_VERSION		0x03
#define HDR_STATIC_METADATA_BLOCK       0x06
#define USE_EXTENDED_TAG		0x07



// HDR definitions copied from linux/include/uapi/drm/drm_mode.h

#ifndef HAVE_DRM_HDR_OUTPUT_METADATA

struct drm_hdr_metadata_infoframe {
  uint8_t eotf;
  uint8_t metadata_type;
  struct {
    uint16_t x, y;
  } display_primaries[3];
  struct {
    uint16_t x, y;
  } white_point;
  uint16_t max_display_mastering_luminance;
  uint16_t min_display_mastering_luminance;
  uint16_t max_cll;
  uint16_t max_fall;
};

struct drm_hdr_output_metadata {
  uint32_t metadata_type;
  union {
    struct drm_hdr_metadata_infoframe hdmi_metadata_type1;
  };
};


#endif  // HAVE_DRM_HDR_OUTPUT_METADATA
/* DRM HDR definitions. Not in the UAPI header, unfortunately. */
enum hdmi_metadata_type {
	HDMI_STATIC_METADATA_TYPE1 = 0,
};

enum hdmi_eotf {
	HDMI_EOTF_TRADITIONAL_GAMMA_SDR,
	HDMI_EOTF_TRADITIONAL_GAMMA_HDR,
	HDMI_EOTF_SMPTE_ST2084,
    HDMI_EOTF_BT_2100_HLG,
};

struct drm_fb {
	struct gbm_bo *bo = nullptr;
	uint32_t fb_id;
    uint32_t format;
};

struct avi_infoframe {
	uint32_t colorspace;
    uint32_t max_bpc; 
	uint32_t output_format;
	uint32_t c_enc; 
	uint32_t c_range;
};




struct DisplayChromacities
{
	float RedX;
	float RedY;
	float GreenX;
	float GreenY;
	float BlueX;
	float BlueY;
	float WhiteX;
	float WhiteY;
};



static const DisplayChromacities DisplayChromacityList[] =
{
	{ 0.64000f, 0.33000f, 0.30000f, 0.60000f, 0.15000f, 0.06000f, 0.31270f, 0.32900f }, // Display Gamut Rec709
	{ 0.70800f, 0.29200f, 0.17000f, 0.79700f, 0.13100f, 0.04600f, 0.31270f, 0.32900f }, // Display Gamut Rec2020
	{ 0.68000f, 0.32000f, 0.26500f, 0.69000f, 0.15000f, 0.06000f, 0.31270f, 0.32900f }, // Display Gamut P3D65
	{ 0.68000f, 0.32000f, 0.26500f, 0.69000f, 0.15000f, 0.06000f, 0.31400f, 0.35100f }, // Display Gamut P3DCI(Theater)
	{ 0.68000f, 0.32000f, 0.26500f, 0.69000f, 0.15000f, 0.06000f, 0.32168f, 0.33767f }, // Display Gamut P3D60(ACES Cinema)
};

class ofxRPI4Window : public ofAppBaseGLESWindow
{
public:
    

    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
	EGLint SurfaceAttribs [10] = {
		EGL_SMPTE2086_DISPLAY_PRIMARY_RX_EXT,       
		EGL_SMPTE2086_DISPLAY_PRIMARY_RY_EXT,
		EGL_SMPTE2086_DISPLAY_PRIMARY_GX_EXT,
		EGL_SMPTE2086_DISPLAY_PRIMARY_GY_EXT,
		EGL_SMPTE2086_DISPLAY_PRIMARY_BX_EXT,
		EGL_SMPTE2086_DISPLAY_PRIMARY_BY_EXT,
		EGL_SMPTE2086_WHITE_POINT_X_EXT,
		EGL_SMPTE2086_WHITE_POINT_Y_EXT,
		EGL_SMPTE2086_MAX_LUMINANCE_EXT,
		EGL_SMPTE2086_MIN_LUMINANCE_EXT
};   
    int device;

    drmModeModeInfo mode;
    struct gbm_device* gbmDevice;
    struct gbm_surface* gbmSurface;
    drmModeCrtc *crtc;
	int crtc_index;
    uint32_t connectorId, HDRplaneId, SDRplaneId;
		bool ok;
		uint64_t colorspace, max_bpc, output_format, c_enc, c_range, in_formats;
	uint32_t prop_id;
				uint64_t crtc_id, fb_id, blob_id;
		drmModePropertyPtr prop;
		drmModeAtomicReq *req;
			drmModePlaneRes *res;
	drmModePlane *plane;
	unsigned int num_modifiers;
	uint64_t *modifiers;
	avi_infoframe property_id;
		uint32_t flags = DRM_MODE_ATOMIC_NONBLOCK | DRM_MODE_ATOMIC_ALLOW_MODESET;
	
		//	uint64_t output_format;
    gbm_bo *previousBo;
    uint32_t previousFb;
    
    ofRectangle currentWindowRect;
    ofOrientation orientation;
    bool bEnableSetupScreen;
    int glesVersion;
    ofWindowMode windowMode;


    static bool allowsMultiWindow(){ return false; }
    static bool doesLoop(){ return false; }
    static void loop(){};
    static bool needsPolling(){ return true; }
    static void pollEvents();
    

    ofxRPI4Window();
    ofxRPI4Window(const ofGLESWindowSettings & settings);

   
    int getWidth() override;
    int getHeight() override;
    glm::vec2 getWindowSize() override;
    glm::vec2 getWindowPosition() override;
    glm::vec2 getScreenSize()  override;
    ofOrientation getOrientation() { return orientation; }
    void enableSetupScreen() override;
    void disableSetupScreen() override;
    void setWindowShape(int w, int h) override;
    void setVerticalSync(bool enabled) override;
    
    void update() override;
    void draw() override;
void EGL_info();
bool drm_mode_get_property(int drm_fd, uint32_t object_id, uint32_t object_type,
		     const char *name, uint32_t *prop_id /* out */,
		     uint64_t *value /* out */,
		     drmModePropertyPtr *prop /* out */);
int last_req = 0;
int first_req = 0;
  bool flip = true;
int isHDR = 0;

void drm_mode_atomic_set_property(int drm_fd, drmModeAtomicReq *freq, const char *name /* in */, uint32_t object_id /* in */,
			uint32_t prop_id /* in */, uint64_t value /* in */, drmModePropertyPtr prop /* in */);
void get_format_modifiers(int fd, uint32_t blob_id, int format_index);

int find_device();
bool InitDRM();
int CreateFB_ID();
bool is_panel_hdr(int fd, int connector_id);
void in_formats_info(int fd, uint32_t blob_id);
bool cta_block(const char *edid_ext);
void FlipPage(bool flip, int isHDR, uint32_t fb_id);
void SetActivePlane(uint32_t plane_id, ofRectangle currentWindowRect, int fb_id);
void DisablePlane(uint32_t plane_id); 
int SetPlaneId(int isHDR);
bool updateHDR_Infoframe(enum hdmi_eotf, int idx);
bool updateAVI_Infoframe(uint32_t plane_id, struct avi_infoframe avi_infoframe);

	drm_fb * drm_fb_get_from_bo(struct gbm_bo *bo);
    void swapBuffers() override;
    
    void makeCurrent() override;
    void startRender() override;
    void finishRender() override;
    
    ofCoreEvents coreEvents;
    ofCoreEvents & events();
    std::shared_ptr<ofBaseRenderer> & renderer();

    void setup(const ofGLESWindowSettings & settings);
	void HDRWindowSetup();
    void SDRWindowSetup();   
	
    std::shared_ptr<ofBaseRenderer> currentRenderer;
    
  
    EGLDisplay getEGLDisplay() override;
    EGLContext getEGLContext() override;
    EGLSurface getEGLSurface() override;
    bool DestroyWindow();
	void DestroyContext();
	void DestroySurface();
    virtual ~ofxRPI4Window();
    bool skipRender;
    struct timeval t0;
    struct timeval t1;
    float lastFrameTimeMillis;
    string getInfo();
    void gbmClean();
};
