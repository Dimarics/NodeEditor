#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cameracontroller.h"
#include "..\QtExtras\DataStream3d.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QDirectionalLight>

#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QTorusMesh>


#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QScreenRayCaster>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_rootEntity(new Qt3DCore::QEntity)
{
    ui->setupUi(this);
    connect(ui->globalSettingsVisible, &QAction::triggered, ui->globalSettings, &QWidget::setVisible);
    connect(ui->entityTreeVisible, &QAction::triggered, ui->entityTree, &QWidget::setVisible);
    connect(ui->propertyVision, &QAction::triggered, ui->properyTree, &QWidget::setVisible);
    connect(ui->entityTree, &EntityTree::entityChanged, ui->properyTree, &PropertyTree::setEntity);

    ui->ambient->setMinimum(0);
    ui->ambient->setMaximum(100);
    ui->ambient->setValue(ui->ambient->maximum());
    connect(ui->ambient, &Volume::valueChanged, this, [this](int value)
    {
        qreal k = value / 100.f;
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (!entityShell->hasMaterial()) continue;
            QColor ambient = entityShell->originalAmbient();
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
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);
    view->defaultFrameGraph()->setClearColor(QColor(33, 40, 48));
    view->setRootEntity(m_rootEntity);
    view->renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::PickMethod::TrianglePicking);
    //view->renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::PrimitivePicking);
    //view->renderSettings()->pickingSettings()->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);
    QWidget *container = QWidget::createWindowContainer(view);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->splitter->addWidget(container);

    Qt3DRender::QObjectPicker *objectPicker = new Qt3DRender::QObjectPicker(m_rootEntity);
    objectPicker->setDragEnabled(true);
    m_rootEntity->addComponent(objectPicker);
    connect(objectPicker, &Qt3DRender::QObjectPicker::pressed,
            this, [this](Qt3DRender::QPickEvent *pick)
    {
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (entityShell->entity() == pick->entity())
                ui->properyTree->setEntity(entityShell);
        }
    });

    // Настройки камеры
    Qt3DRender::QCamera *camera = view->camera();
    camera->setViewCenter(QVector3D(0, 0, 0));
    camera->setPosition(QVector3D(0, 0, 1000));
    camera->lens()->setPerspectiveProjection(20.f, qreal(view->width()) / qreal(view->height()), 1.f, 100000.f);
    //camera->setFarPlane(3.40282e+38);
    //Qt3DExtras::QOrbitCameraController *cameraController = new Qt3DExtras::QOrbitCameraController(m_rootEntity);
    CameraController *cameraController = new CameraController(m_rootEntity);
    cameraController->setCamera(camera);
    //cameraController->setLookSpeed(500);
    //cameraController->setLinearSpeed(500);

    // Свет
    Qt3DRender::QDirectionalLight *light = new Qt3DRender::QDirectionalLight(m_rootEntity);
    light->setWorldDirection(QVector3D(0, 0, -1));
    light->setIntensity(1);
    m_rootEntity->addComponent(light);
    connect(camera, &Qt3DRender::QCamera::viewVectorChanged, this, [light](const QVector3D &viewVector)
    {
        light->setWorldDirection(viewVector);
    });
    //m_light->setWorldDirection(m_camera->viewVector());

    // Загрузчик
    //sceneLoader->setSource(QUrl("qrc:/meshes/platform.obj"));

    //m_loader = new Qt3DRender::QSceneLoader(m_rootEntity);
    //m_rootEntity->addComponent(m_loader);

    Qt3DCore::QEntity *loaderEntity = new Qt3DCore::QEntity(m_rootEntity);
    //Qt3DCore::QEntity *loaderEntity = new Qt3DCore::QEntity;
    connect(ui->open, &QAction::triggered, this, [loaderEntity, this]
    {
        QString path = QFileDialog::getOpenFileName(this, tr("Открытие файла"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                    "Файлы OBJ (*.obj);;"
                                                    "Файлы DMG (*.dmg);;"
                                                    "Литография (*.stl);;"
                                                    "Блендер (*.blend)");
        if (path.isEmpty()) return;
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
                ui->entityTree->setRoot(root);
            });
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

    /*Qt3DCore::QEntity *torusEntity = new Qt3DCore::QEntity(m_rootEntity);
    torusEntity->setObjectName("torus");
    Qt3DExtras::QTorusMesh *torusMesh = new Qt3DExtras::QTorusMesh(torusEntity);
    torusMesh->setRadius(15);
    torusMesh->setMinorRadius(6);
    torusMesh->setSlices(16);
    torusMesh->setRings(32);
    Qt3DExtras::QDiffuseSpecularMaterial *torusMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(torusEntity);
    torusMaterial->setAmbient(Qt::black);
    torusMaterial->setDiffuse(QColor(0, 255, 0));
    Qt3DCore::QTransform *torusTransform = new Qt3DCore::QTransform(torusEntity);
    torusEntity->addComponent(torusMesh);
    torusEntity->addComponent(torusMaterial);
    torusEntity->addComponent(torusTransform);*/
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_rootEntity;
}
