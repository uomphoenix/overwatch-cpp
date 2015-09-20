SRC_DIR = src
INC_DIR = include

# Use pkgconfig to link opencv libs & give include path,
# `pkg-config --cflags --libs opencv`
CONFIG += debug link_pkgconfig c++11
PKGCONFIG += opencv
TEMPLATE = app
TARGET = overwatch
DEPENDPATH += .
INCLUDEPATH += $${INC_DIR}
OBJECTS_DIR = obj
QMAKE_CXXFLAGS += -std=c++11

SOURCES += $${SRC_DIR}/main.cpp \
    $${SRC_DIR}/LeptonCamera.cpp \
    $${SRC_DIR}/CameraContainer.cpp \
    $${SRC_DIR}/LeptonCameraContainer.cpp \
    $${SRC_DIR}/PiCameraContainer.cpp \
    $${SRC_DIR}/AuthenticationClient.cpp \
    $${SRC_DIR}/VideoFeedClient.cpp \
    $${SRC_DIR}/SocketClient.cpp \
    $${SRC_DIR}/util.cpp

HEADERS += $${INC_DIR}/SocketClient.h \
    $${INC_DIR}/AuthenticationClient.h \
    $${INC_DIR}/VideoFeedClient.h \
    $${INC_DIR}/LeptonCamera.h \
    $${INC_DIR}/CameraContainer.h \
    $${INC_DIR}/LeptonCameraContainer.h \
    $${INC_DIR}/PiCameraContainer.h \
    $${INC_DIR}/Exception.h \
    $${INC_DIR}/util.h

# CFLAGS += -std=c++0x
# Need raspicam & mmal libs for reading from Pi cam
# -lmmal -lmmal_core
LIBS += -lraspicam -lraspicam_cv
