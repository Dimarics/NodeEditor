#include "mainwindow.h"
#include <QApplication>

#if defined(_DEBUG)
creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qputenv("QT3D_RENDERER", "opengl");
    //qputenv("QT3D_RENDERER", "rhi");
    //qputenv("QSG_RHI_BACKEND", "vulkan");

    MainWindow window;
    window.showMaximized();

    return app.exec();
}
