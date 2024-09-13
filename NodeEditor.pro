QT += core \
    widgets \
    gui 3dcore \
    3drender \
    3dinput \
    3dlogic \
    3dextras

CONFIG += c++17
#CONFIG += resources_big

SOURCES += \
    ../QtExtras/datastream3d.cpp \
    ../QtExtras/volume.cpp \
    cameracontroller.cpp \
    entityshell.cpp \
    entitytree.cpp \
    main.cpp \
    mainwindow.cpp \
    propertytree.cpp

HEADERS += \
    ../QtExtras/datastream3d.h \
    ../QtExtras/volume.h \
    cameracontroller.h \
    entityshell.h \
    entitytree.h \
    mainwindow.h \
    propertytree.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
