LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg
LOCAL_SRC_FILES := libffmpeg.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ffmpeg
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := main
SDL_PATH := ../SDL
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
	video.c

#have to be +=, have to having SDL2 and ffmpeg, as we having 2 libs
LOCAL_SHARED_LIBRARIES += SDL2
LOCAL_SHARED_LIBRARIES += ffmpeg
LOCAL_LDLIBS := -lGLESv1_CM -llog
include $(BUILD_SHARED_LIBRARY)
