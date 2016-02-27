LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Locate OpenCL SDK root folder
INTELOCLSDKROOT="/Users/wing/Sdk/Mali_OpenCL_SDK_v1.1.0"

LOCAL_CFLAGS += -I$(INTELOCLSDKROOT)/include
LOCAL_MODULE    := LdpcInAndroid
LOCAL_SRC_FILES := MyOpenCL.cpp MyLdpc.cpp test.cpp
LOCAL_SHARED_LIBRARIES	:= OpenCL
LOCAL_LDFLAGS += -llog -ljnigraphics -L $(INTELOCLSDKROOT)/lib -lOpenCL
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
