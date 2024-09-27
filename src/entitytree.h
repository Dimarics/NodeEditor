#ifndef ENTITYTREE_H
#define ENTITYTREE_H

#include "entityshell.h"

#include <QTreeView>
#include <QStandardItemModel>

class EntityTree : public QTreeView
{
    Q_OBJECT
public:
    EntityTree(QWidget *parent = nullptr);
    void setRoot(Qt3DCore::QEntity *rootEntity);
    void buildBrunch(EntityShell *root);
    void clear();
    EntityShell *currentItem();
    QList<EntityShell*> entities() const;

private:
    QStandardItemModel *m_model;
    EntityShell *m_currentItem;

//public slots:
//    void renameCurrent(const QString &name);

signals:
    void entityChanged(EntityShell*);
};

#endif // ENTITYTREE_H
