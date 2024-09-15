#ifndef VIEW3D_H
#define VIEW3D_H

#include <QtWidgets/private/qwindowcontainer_p.h>
#include <Qt3DExtras/Qt3DWindow>

class View3D : public QWindowContainer
{
    Q_OBJECT
public:
    View3D(QWidget *parent = nullptr);
    QVector3D mapToWorld(qreal x, qreal y);
    Qt3DCore::QEntity *rootEntity();

private:
    Qt3DExtras::Qt3DWindow *m_view;
    Qt3DCore::QEntity *m_rootEntity;
};

#endif // VIEW3D_H
