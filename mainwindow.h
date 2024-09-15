#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "view3d.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemDelegate>

#include <Qt3DRender/QSceneLoader>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawLine(const QVector3D& p1, const QVector3D& p2, const QColor& color, Qt3DCore::QEntity *root);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
