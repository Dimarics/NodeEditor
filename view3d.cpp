#include "view3d.h"
#include "cameracontroller.h"

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QRenderSettings>
//#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QDirectionalLight>

#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QLogicalDevice>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DRender/QCamera>
#include <Qt3DLogic/QFrameAction>

View3D::View3D(QWidget *parent) : QWindowContainer(new Qt3DExtras::Qt3DWindow, parent),
    m_view(qobject_cast<Qt3DExtras::Qt3DWindow*>(containedWindow())),
    m_rootEntity(new Qt3DCore::QEntity)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_view->renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);
    m_view->defaultFrameGraph()->setClearColor(QColor(33, 40, 48));
    m_view->renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::PickMethod::TrianglePicking);
    //view->renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::PrimitivePicking);
    //view->renderSettings()->pickingSettings()->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);
    m_view->setRootEntity(m_rootEntity);

    // Настройки камеры
    Qt3DRender::QCamera *camera = m_view->camera();
    camera->setViewCenter(QVector3D(0, 0, 0));
    camera->setPosition(QVector3D(0, 0, 1000));
    camera->lens()->setPerspectiveProjection(20.f, qreal(m_view->width()) / qreal(m_view->height()), 1.f, 100000.f);
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
            { light->setWorldDirection(viewVector); });


    Qt3DInput::QMouseDevice *mouseDevice = new Qt3DInput::QMouseDevice(m_rootEntity);

    Qt3DInput::QActionInput *actionInput = new Qt3DInput::QActionInput(m_rootEntity);
    actionInput->setButtons({Qt::LeftButton});
    actionInput->setSourceDevice(mouseDevice);

    Qt3DInput::QAnalogAxisInput *axisXInput = new Qt3DInput::QAnalogAxisInput(m_rootEntity);
    axisXInput->setAxis(Qt3DInput::QMouseDevice::X);
    Qt3DInput::QAxis *xAxis = new Qt3DInput::QAxis(m_rootEntity);
    xAxis->addInput(axisXInput);
    connect(xAxis, &Qt3DInput::QAxis::valueChanged, this, [this](float value)
            {
            });

    Qt3DInput::QAnalogAxisInput *axisYInput = new Qt3DInput::QAnalogAxisInput(m_rootEntity);
    axisYInput->setAxis(Qt3DInput::QMouseDevice::Y);
    Qt3DInput::QAxis *yAxis = new Qt3DInput::QAxis(m_rootEntity);
    yAxis->addInput(axisYInput);

    Qt3DInput::QAction *action = new Qt3DInput::QAction(m_rootEntity);
    action->addInput(actionInput);
    connect(action, &Qt3DInput::QAction::activeChanged, this, [this](bool active)
            {
                qDebug() << active;
            });

    Qt3DInput::QLogicalDevice *logicalDevice = new Qt3DInput::QLogicalDevice(m_rootEntity);
    logicalDevice->addAction(action);
}

QVector3D View3D::mapToWorld(qreal x, qreal y)
{
    QVector4D v1(2.f * x / width() - 1.f, -2.f * y / height() + 1.f, -1.f, 1.f);
    QVector4D v2((m_view->camera()->projectionMatrix().inverted() * v1).toVector2D(), -1.f, 0.f);
    QVector3D dir((m_view->camera()->viewMatrix().inverted() * v2).normalized());
    QVector3D pos(m_view->camera()->viewMatrix().inverted() * QVector4D(0, 0, 0, 1.f));

    // Уравнение плоскости: Ax + By + Cz + D = 0
    // Нормаль: N = (A, B, C) -
    // Расстояние до центра: d = P0 * N, где - P0 - любая точка плоскости
    // P * N + d
    // Уравнение луча: O + D * t, t - число
    // t = (P0 * N - O * N) / (D * N)
    // result = O + D + t

    QVector3D normal(0.f, 1.f, 0.f);
    qreal t = -QVector3D::dotProduct(pos, normal) / QVector3D::dotProduct(dir, normal);
    QVector3D result = pos + dir * t;

    return result;
}

Qt3DCore::QEntity *View3D::rootEntity()
{
    return m_rootEntity;
}
