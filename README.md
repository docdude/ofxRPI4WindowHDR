# ofxRPI4WindowHDR

### DESCRIPTION:   
This is an openFrameworks addon for the Raspberry Pi to allow rendering without X

HDR support added to take advantage of DRM linux kernel driver HDR metadata additions

Uses dual planes to allow on the fly switching between SDR overlay plane and HDR primary plane

Exposes HDR metadata and AVI infoframe structures to facilitate UI changes 

Extensive logging for debugging




### REQUIREMENTS:   
- openFrameworks 11, with patches to use float images and allow compiling on Biasilinux armv7l --> (ofx.patch)
- raspberry Pi linux 5.10, with patches to vc4, v3d, drm drivers --> (drm_vc4.patch)
- Raspberry Pi 4 or 3B+ (previous ones might work too)
- KMS Driver enabled, thru config.txt(see config.txt) 
- Newest Mesa libraries, with patches that add HDR colorspace attributes --> (mesa_hdr.patch)
- 
 #### Manual Option  
Change `openFrameworks/libs/openFrameworksCompiled/project/linuxarmv7l/config.linuxarmv7l.default.mk`   

```
    ifeq ($(USE_PI_LEGACY), 1)
    	PLATFORM_DEFINES += TARGET_RASPBERRY_PI_LEGACY
        $(info using legacy build)
    else
    	# comment this for older EGL windowing. Has no effect if USE_PI_LEGACY is enabled
    	# GLFW seems to provide a more robust window on newer Raspbian releases
	#USE_GLFW_WINDOW = 1
    endif
```
    
Comment out `ofSetupOpenGL` in 
https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/app/ofAppRunner.cpp#L31

#### Automatic Option  
Run `./patchOF.sh`

### USAGE:   
 
If needed, `sudo apt-get install libgbm-dev`.

### RUNNING
Use the classic oF way to Launch your application. (eg: `make RunDebug`)  
*Note: Quitting is not yet properly supported, use Ctrl+C to interrupt your ofApp.*

### CREDITS:   
derived from 
https://github.com/matusnovak/rpi-opengl-without-x

https://gitlab.freedesktop.org/mesa/kmscube/tree/master

https://github.com/jvcleave/ofxRPI4Window

https://github.com/popcornmix/xbmc/tree/gbm_matrix


