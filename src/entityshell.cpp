#include "entityshell.h"

#include <QFile>


EntityShell::EntityShell(Qt3DCore::QEntity *entity) :
    m_originalShininess(0),
    m_entity(entity),
    m_transform(nullptr),
    m_geometryRender(nullptr),
    m_material(nullptr)
{
    setText(m_entity->objectName().isEmpty() ? "<без имени>" : m_entity->objectName());
    for (Qt3DCore::QComponent *component : m_entity->components())
    {
        if (qobject_cast<Qt3DExtras::QDiffuseSpecularMaterial*>(component))
        {
            m_material = qobject_cast<Qt3DExtras::QDiffuseSpecularMaterial*>(component);
            m_originalShininess = m_material->shininess();
            m_originalAmbient = m_material->ambient();
            m_originalDiffuse = m_material->diffuse().value<QColor>();
            m_originalSpecular = m_material->specular().value<QColor>();
        }
        else if (qobject_cast<Qt3DCore::QTransform*>(component))
        {
            m_transform = qobject_cast<Qt3DCore::QTransform*>(component);
        }
        else if (qobject_cast<Qt3DRender::QGeometryRenderer*>(component))
        {
            m_geometryRender = qobject_cast<Qt3DRender::QGeometryRenderer*>(component);
        }
    }

    /*QFile file("D:/test.3d");
    file.open(QIODevice::WriteOnly);
    DataStream3D stream(&file);
    stream << m_entity;
    file.close();

    Qt3DCore::QEntity *entity_data = new Qt3DCore::QEntity;
    file.open(QIODevice::ReadOnly);
    stream >> entity_data;*/
}

void EntityShell::setName(const QString &name)
{
    m_entity->setObjectName(name);
}

void EntityShell::setAmbient(const QColor &ambient)
{
    m_material->setAmbient(ambient);
}

void EntityShell::setDiffuse(const QColor &diffuse)
{
    m_material->setDiffuse(diffuse);
}

void EntityShell::setSpecular(const QColor &specular)
{
    m_material->setSpecular(specular);
}

void EntityShell::setShininess(float shininess)
{
    m_material->setShininess(shininess);
}

void EntityShell::setTransparency(quint8 alpha)
{
    if (alpha < 255)
    {
        m_material->setAlphaBlendingEnabled(true);
        QColor color = m_material->diffuse().value<QColor>();
        color.setAlpha(alpha);
        m_material->setDiffuse(color);
    }
    else m_material->setAlphaBlendingEnabled(false);
}

bool EntityShell::hasMaterial() const
{
    return m_material != nullptr;
}

float EntityShell::shininess() const
{
    return m_material->shininess();
}

float EntityShell::originalShininess() const
{
    return m_originalShininess;
}

QString EntityShell::name() const
{
    return m_entity->objectName();
}

QColor EntityShell::ambient() const
{
    return m_material->ambient();
}

QColor EntityShell::diffuse() const
{
    return m_material->diffuse().value<QColor>();
}

QColor EntityShell::specular() const
{
    return m_material->specular().value<QColor>();
}

QColor EntityShell::originalAmbient() const
{
    return m_originalAmbient;
}

QColor EntityShell::originalDiffuse() const
{
    return m_originalDiffuse;
}

QColor EntityShell::originalSpecular() const
{
    return m_originalSpecular;
}

Qt3DCore::QEntity *EntityShell::entity()
{
    return m_entity;
}

Qt3DCore::QTransform *EntityShell::transform()
{
    return m_transform;
}

Qt3DRender::QGeometryRenderer *EntityShell::geometryRender()
{
    return m_geometryRender;
}

Qt3DExtras::QDiffuseSpecularMaterial *EntityShell::material()
{
    return m_material;
}
