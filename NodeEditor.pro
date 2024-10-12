include (QtExtras.pri)
include (src/src.pri)

CONFIG += c++20

QT += \
    core \
    widgets \
    gui \
    3dcore \
    3drender \
    3dinput \
    3dlogic \
    3dextras


CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d
}
else {
    BUILD_FLAG = release
}

#win32: OS_SUFFIX = win32
#linux-g++: OS_SUFFIX = linux

#LIBS_PATH = $${PWD}/lib.$${OS_SUFFIX}/
BIN_PATH = $${PWD}/bin/$${BUILD_FLAG}/
#BUILD_PATH = $${PWD}/build/$${BUILD_FLAG}/$${TARGET}/

TEMPLATE = app

HEADERS +=
