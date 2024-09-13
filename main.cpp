#include "mainwindow.h"
#include <QApplication>

#if defined(_DEBUG)
creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.showMaximized();

    return app.exec();
}
