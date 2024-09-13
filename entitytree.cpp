#include "entitytree.h"

#include <QHeaderView>
#include <Qt3DExtras/QPhongMaterial>

EntityTree::EntityTree(QWidget *parent) : QTreeView(parent),
    m_model(new QStandardItemModel)
{
    setModel(m_model);
    m_model->setHorizontalHeaderLabels({"Древо сущностей"});
    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex &index)
    {
        m_currentItem = static_cast<EntityShell*>(m_model->itemFromIndex(index));
        emit entityChanged(m_currentItem);
    });
}

void EntityTree::setRoot(Qt3DCore::QEntity *rootEntity)
{
    EntityShell *root = new EntityShell(rootEntity);
    m_model->appendRow(root);
    root->setText(rootEntity->objectName());
    buildBrunch(root);
}

void EntityTree::buildBrunch(EntityShell *root)
{
    for (Qt3DCore::QNode *childNode : root->entity()->childNodes())
    {
        if (!qobject_cast<Qt3DCore::QEntity*>(childNode)) continue;
        for (Qt3DCore::QComponent *component : qobject_cast<Qt3DCore::QEntity*>(childNode)->components())
        {
            if (qobject_cast<Qt3DExtras::QPhongMaterial*>(component))
            {
                Qt3DExtras::QPhongMaterial *phongMaterial = qobject_cast<Qt3DExtras::QPhongMaterial*>(component);
                Qt3DExtras::QDiffuseSpecularMaterial *diffuseSpecularMaterial = new Qt3DExtras::QDiffuseSpecularMaterial;
                diffuseSpecularMaterial->setAmbient(phongMaterial->ambient());
                diffuseSpecularMaterial->setDiffuse(phongMaterial->diffuse());
                diffuseSpecularMaterial->setSpecular(phongMaterial->specular());
                diffuseSpecularMaterial->setShininess(phongMaterial->shininess());
                Qt3DCore::QEntity *entity = qobject_cast<Qt3DCore::QEntity*>(childNode);
                entity->removeComponent(phongMaterial);
                entity->addComponent(diffuseSpecularMaterial);
            }
        }
        EntityShell *childTreeItem = new EntityShell(qobject_cast<Qt3DCore::QEntity*>(childNode));
        root->appendRow(childTreeItem);
        buildBrunch(childTreeItem);
    }
    /*if (!treeItem->data(Qt::UserRole).canConvert<Qt3DCore::QNode*>()) return;
    Qt3DCore::QNode *node = treeItem->data(Qt::UserRole).value<Qt3DCore::QNode*>();
    for (Qt3DCore::QNode *childNode : node->childNodes())
    {
        if (!qobject_cast<Qt3DCore::QEntity*>(childNode)) continue;

        QStandardItem *childTreeItem = new QStandardItem;
        treeItem->appendRow(childTreeItem);
        QString name = childNode->objectName();
        if (name.isEmpty()) name = "<без имени>";
        childTreeItem->setText(name);
        for (Qt3DCore::QComponent *component : qobject_cast<Qt3DCore::QEntity*>(childNode)->components())
        {
            if (qobject_cast<Qt3DExtras::QPhongMaterial*>(component))
            {
                Qt3DExtras::QPhongMaterial *phongMaterial = qobject_cast<Qt3DExtras::QPhongMaterial*>(component);
                Qt3DExtras::QDiffuseSpecularMaterial *diffuseSpecularMaterial = new Qt3DExtras::QDiffuseSpecularMaterial;
                diffuseSpecularMaterial->setAmbient(phongMaterial->ambient());
                diffuseSpecularMaterial->setDiffuse(phongMaterial->diffuse());
                diffuseSpecularMaterial->setSpecular(phongMaterial->specular());
                diffuseSpecularMaterial->setShininess(phongMaterial->shininess());
                Qt3DCore::QEntity *entity = qobject_cast<Qt3DCore::QEntity*>(childNode);
                entity->removeComponent(phongMaterial);
                entity->addComponent(diffuseSpecularMaterial);
            }
        }

        QVariant data;
        data.setValue(childNode);
        childTreeItem->setData(data, Qt::UserRole);

        addElements(childTreeItem);
    }*/
}

/*void EntityTree::renameCurrent(const QString &name)
{
    m_currentItem->setText(name);
}*/

void EntityTree::clear()
{
    m_model->removeRows(0, m_model->rowCount());
    //initHeader();
}

EntityShell *EntityTree::currentItem()
{
    return m_currentItem;
}

QList<EntityShell*> EntityTree::entities() const
{
    QList<EntityShell*> entities;
    std::function<void(EntityShell*)> addEntity = [&](EntityShell *root)
    {
        for (int row = 0; row < root->rowCount(); ++row)
        {
            EntityShell *entity = static_cast<EntityShell*>(root->child(row));
            entities << entity;
            addEntity(entity);
        }
    };
    for (int row = 0; row < m_model->rowCount(); ++row)
    {
        EntityShell *entity = static_cast<EntityShell*>(m_model->item(row));
        entities << entity;
        addEntity(entity);
    }
    return entities;
}
