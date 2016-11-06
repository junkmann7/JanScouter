# ------------------------------------ #
#  JanScouterUtil                      #
# ------------------------------------ #
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)



LOCAL_C_INCLUDES :=    \
                   $(JNI_H_INCLUDE)

LOCAL_MODULE    :=     \
                   JanScouterUtil

LOCAL_SRC_FILES :=     \
                   JanScouterUtil.cpp \
                   preparation.cpp\
                   matching.cpp\
                   keisan.cpp\
                   mjscore.cpp \
                   hosei.cpp \
#				   main.cpp \


LOCAL_CPPFLAGS   :=

LOCAL_LDLIBS := -llog
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

