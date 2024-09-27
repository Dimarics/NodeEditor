#ifndef ENTITYSHELL_H
#define ENTITYSHELL_H

#include <QStandardItem>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QDiffuseSpecularMaterial>

class EntityShell : public QStandardItem
{
public:
    EntityShell(Qt3DCore::QEntity *entity);
    void setShininess(float shininess);
    void setName(const QString &name);
    void setAmbient(const QColor &ambient);
    void setDiffuse(const QColor &diffuse);
    void setSpecular(const QColor &ambient);
    void setTransparency(quint8 alpha);

    bool hasMaterial() const;
    float shininess() const;
    float originalShininess() const;
    QString name() const;
    QColor ambient() const;
    QColor diffuse() const;
    QColor specular() const;
    QColor originalAmbient() const;
    QColor originalDiffuse() const;
    QColor originalSpecular() const;
    Qt3DCore::QEntity *entity();

    //
    void setOriginPoint(const QVector3D &p)
    {
        m_originPoint = p;
    }
    void rotate(const QQuaternion &rotation)
    {
        m_transform->setRotation(m_transform->rotation() * rotation);
    }

private:
    float m_originalShininess;
    QColor m_originalAmbient;
    QColor m_originalDiffuse;
    QColor m_originalSpecular;
    Qt3DCore::QEntity *m_entity;
    Qt3DCore::QTransform *m_transform;
    Qt3DRender::QGeometryRenderer *m_geometryRender;
    Qt3DExtras::QDiffuseSpecularMaterial *m_material;

    //
    QVector3D m_originPoint;
    QVector3D m_rotateAxis;
};

#endif // ENTITYSHELL_H
