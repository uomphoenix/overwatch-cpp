SRC_DIR = src
INC_DIR = include

CONFIG = debug
TEMPLATE = app
TARGET = overwatch
DEPENDPATH += .
INCLUDEPATH += $${INC_DIR}
OBJECTS_DIR = obj

SOURCES += $${SRC_DIR}/main.cpp \
    $${SRC_DIR}/AuthenticationClient.cpp \
    $${SRC_DIR}/VideoFeedClient.cpp \
    $${SRC_DIR}/SocketClient.cpp \
    $${SRC_DIR}/util.cpp

HEADERS += $${INC_DIR}/SocketClient.h \
    $${INC_DIR}/AuthenticationClient.h \
    $${INC_DIR}/VideoFeedClient.h \
    $${INC_DIR}/util.h

# LIBS +=
