#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../QtExtras/DataStream3d.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPlaneMesh>

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QScreenRayCaster>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
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
    Qt3DRender::QObjectPicker *objectPicker = new Qt3DRender::QObjectPicker(ui->viewPort->rootEntity());
    objectPicker->setDragEnabled(true);
    ui->viewPort->rootEntity()->addComponent(objectPicker);
    Qt3DRender::QScreenRayCaster *rayCaster = new Qt3DRender::QScreenRayCaster(ui->viewPort->rootEntity());
    ui->viewPort->rootEntity()->addComponent(rayCaster);
    connect(objectPicker, &Qt3DRender::QObjectPicker::pressed,
            this, [rayCaster, this](Qt3DRender::QPickEvent *pick)
    {
        rayCaster->setPosition(ui->viewPort->mapToGlobal(pick->position().toPoint()));
        //drawLine(QVector3D(0, 0, 0), pick->worldIntersection(), Qt::red, m_rootEntity);
        for (Qt3DRender::QRayCasterHit &hit : rayCaster->hits())
        {
            qDebug() << hit.entity();
        }
        for (EntityShell *entityShell : ui->entityTree->entities())
        {
            if (entityShell->entity() == pick->entity())
                ui->properyTree->setEntity(entityShell);
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

    drawLine(QVector3D(-500, 0, 0), QVector3D(500, 0, 0), QColor(255, 0, 0), ui->viewPort->rootEntity());
    ui->viewPort->mapToWorld(qreal(width()) / 2.f, qreal(height()) / 2.f);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawLine(const QVector3D& p1, const QVector3D& p2, const QColor& color, Qt3DCore::QEntity *root)
{
    Qt3DCore::QGeometry *geometry = new Qt3DCore::QGeometry(root);

    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float));
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = p1.x();
    *positions++ = p1.y();
    *positions++ = p1.z();
    *positions++ = p2.x();
    *positions++ = p2.y();
    *positions++ = p2.z();

    Qt3DCore::QBuffer *positionBuffer = new Qt3DCore::QBuffer(geometry);
    positionBuffer->setData(bufferBytes);

    Qt3DCore::QAttribute *positionAttribute = new Qt3DCore::QAttribute(geometry);
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(positionBuffer);
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute);

    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(uint));
    uint *indices = reinterpret_cast<uint*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    Qt3DCore::QBuffer *indexBuffer = new Qt3DCore::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    Qt3DCore::QAttribute *indexAttribute = new Qt3DCore::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setByteStride(sizeof(uint));
    indexAttribute->setCount(2);
    geometry->addAttribute(indexAttribute);

    // mesh
    Qt3DRender::QGeometryRenderer *line = new Qt3DRender::QGeometryRenderer(root);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    Qt3DExtras::QDiffuseSpecularMaterial *material = new Qt3DExtras::QDiffuseSpecularMaterial(root);
    material->setAmbient(color);

    // entity
    Qt3DCore::QEntity *lineEntity = new Qt3DCore::QEntity(root);
    lineEntity->addComponent(material);
    lineEntity->addComponent(line);
}
