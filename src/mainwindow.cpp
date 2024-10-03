#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datastream3d.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPlaneMesh>

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setStretchFactor(2, 1);
    ui->splitter->setStretchFactor(3, 12);
    //ui->splitter->setSizes({150, 140, 210, 1000});
    connect(ui->globalSettingsVisible, &QAction::triggered, ui->globalSettings, &QWidget::setVisible);
    connect(ui->entityTreeVisible, &QAction::triggered, ui->entityTree, &QWidget::setVisible);
    connect(ui->propertyVision, &QAction::triggered, ui->properyTree, &QWidget::setVisible);
    connect(ui->entityTree, &EntityTree::entityChanged, ui->properyTree, &PropertyTree::setEntity);

    ui->ambient->setMinimum(0);
    ui->ambient->setMaximum(100);
    ui->ambient->setValue(10);
    connect(ui->ambient, &Volume::valueChanged, this, [this](int value)
    {
        qreal k = value / 100.f;
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (!entityShell->hasMaterial()) continue;
            QColor ambient = entityShell->diffuse();
            ambient.setRed(ambient.red() * k);
            ambient.setGreen(ambient.green() * k);
            ambient.setBlue(ambient.blue() * k);
            entityShell->setAmbient(ambient);
        }
    });

    ui->diffuse->setMinimum(0);
    ui->diffuse->setMaximum(100);
    ui->diffuse->setValue(ui->diffuse->maximum());
    connect(ui->diffuse, &Volume::valueChanged, this, [this](int value)
    {
        qreal k = value / 100.f;
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (!entityShell->hasMaterial()) continue;
            QColor diffuse = entityShell->originalAmbient();
            diffuse.setRed(diffuse.red() * k);
            diffuse.setGreen(diffuse.green() * k);
            diffuse.setBlue(diffuse.blue() * k);
            entityShell->setDiffuse(diffuse);
        }
    });

    ui->specular->setMinimum(0);
    ui->specular->setMaximum(100);
    ui->specular->setValue(ui->specular->maximum());
    connect(ui->specular, &Volume::valueChanged, this, [this](int value)
    {
        qreal k = value / 100.f;
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (!entityShell->hasMaterial()) continue;
            QColor specular = entityShell->originalAmbient();
            specular.setRed(specular.red() * k);
            specular.setGreen(specular.green() * k);
            specular.setBlue(specular.blue() * k);
            entityShell->setSpecular(specular);
        }
    });

    ui->shininess->setMinimum(0);
    ui->shininess->setMaximum(100);
    ui->shininess->setValue(ui->shininess->maximum());
    connect(ui->shininess, &Volume::valueChanged, this, [this](int value)
    {
        qreal k = value / 100.f;
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (!entityShell->hasMaterial()) continue;
            QColor shininess = entityShell->originalAmbient();
            shininess.setRed(shininess.red() * k);
            shininess.setGreen(shininess.green() * k);
            shininess.setBlue(shininess.blue() * k);
            entityShell->setSpecular(shininess);
        }
    });

    // Сцена
    /*Qt3DRender::QObjectPicker *objectPicker = new Qt3DRender::QObjectPicker(ui->viewPort->rootEntity());
    objectPicker->setDragEnabled(true);*/

    connect(ui->viewPort, &View3D::objectChanged, this, [this](Qt3DRender::QPickEvent *pick)
    {
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (entityShell->entity() == pick->entity())
            {
                ui->properyTree->setEntity(entityShell);
                for (Qt3DCore::QAttribute *attrib : entityShell->geometryRender()->geometry()->attributes())
                {
                    if (attrib->name() == Qt3DCore::QAttribute::defaultPositionAttributeName())
                    {
                        float *positions = reinterpret_cast<float*>(attrib->buffer()->data().data());
                        for (uint i = 0; i < attrib->count() / 3; ++i)
                        {
                            QVector3D p1, p2, p3;
                            p1.setX(*positions++);
                            p1.setY(*positions++);
                            p1.setZ(*positions++);

                            positions += 3;

                            p2.setX(*positions++);
                            p2.setY(*positions++);
                            p2.setZ(*positions++);

                            positions += 3;

                            p3.setX(*positions++);
                            p3.setY(*positions++);
                            p3.setZ(*positions++);

                            positions += 3;
                        }
                    }
                }
            }
        }
    });

    // Загрузчик
    //sceneLoader->setSource(QUrl("qrc:/meshes/platform.obj"));

    //m_loader = new Qt3DRender::QSceneLoader(m_rootEntity);
    //m_rootEntity->addComponent(m_loader);

    Qt3DCore::QEntity *loaderEntity = new Qt3DCore::QEntity(ui->viewPort->rootEntity());
    //Qt3DCore::QEntity *loaderEntity = new Qt3DCore::QEntity;
    connect(ui->open, &QAction::triggered, this, [loaderEntity, this]
    {
        QStringList paths = QFileDialog::getOpenFileNames(this, tr("Открытие файла"),
                                                          QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                          "Файлы OBJ (*.obj);;"
                                                          "Файлы DMG (*.dmg);;"
                                                          "Литография (*.stl);;"
                                                          "Блендер (*.blend)");
        if (paths.isEmpty()) return;
        for (QString &path : paths)
        {
            if (QFileInfo(path).suffix() == "dmg")
            {
                DataStream3D::readDMG(path, loaderEntity);
                Qt3DCore::QEntity *root = qobject_cast<Qt3DCore::QEntity*>(loaderEntity->childNodes().last());
                ui->entityTree->setRoot(root);
            }
            else
            {
                //m_loader->setSource(QUrl::fromLocalFile(path));
                Qt3DRender::QSceneLoader *loader = new Qt3DRender::QSceneLoader;
                loaderEntity->addComponent(loader);
                loader->setSource(QUrl::fromLocalFile(path));

                connect(loader, &Qt3DRender::QSceneLoader::statusChanged,
                        this, [loader, loaderEntity, this](Qt3DRender::QSceneLoader::Status status)
                {
                    if (status != Qt3DRender::QSceneLoader::Ready) return;
                    Qt3DCore::QEntity *root = qobject_cast<Qt3DCore::QEntity*>(loaderEntity->childNodes().last());
                    root->setObjectName(loader->source().fileName());
                    new Joint(root);
                    ui->entityTree->setRoot(root);
                    emit ui->ambient->valueChanged(ui->ambient->value());

                    if (loader->entity("Твердое тело2:54"))
                    {
                        Joint *joint = new Joint(loader->entity("Твердое тело2:54"));
                        joint->setRotationAxis(QVector3D(0, 1, 0));
                    }
                });
            }
        }
    });
    connect(ui->saveAs, &QAction::triggered, this, [loaderEntity, this]
    {
        QFileDialog fileDialog(this);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        QString path = fileDialog.getSaveFileName(this, tr("Сохранить файл"),
                                                  QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                  "Файлы DMG (*.dmg)");
        if (path.isEmpty()) return;
        DataStream3D::writeDMG(path, loaderEntity);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
