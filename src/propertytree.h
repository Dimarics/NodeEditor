#ifndef PROPERTYTREE_H
#define PROPERTYTREE_H

#include "entityshell.h"

#include <QTreeView>
#include <QStandardItemModel>

class PropertyTree : public QTreeView
{
    Q_OBJECT
public:
    PropertyTree(QWidget *parent = nullptr);
    void clear();

private:
    EntityShell *m_entityShell;
    QStandardItemModel *m_model;
    QStandardItem *m_entityName;
    QStandardItem *m_hide;
    QStandardItem *m_materialSection;
    QStandardItem *m_ambientColor;
    QStandardItem *m_ambientRed;
    QStandardItem *m_ambientGreen;
    QStandardItem *m_ambientBlue;
    QStandardItem *m_diffuseColor;
    QStandardItem *m_diffuseRed;
    QStandardItem *m_diffuseGreen;
    QStandardItem *m_diffuseBlue;
    QStandardItem *m_specularColor;
    QStandardItem *m_specularRed;
    QStandardItem *m_specularGreen;
    QStandardItem *m_specularBlue;
    QStandardItem *m_shininess;
    QStandardItem *m_transparency;

    //void setColorProperty(QStandardItem *root, const QString &name, const QColor &color, Property id);
    inline void setSectionHidden(QStandardItem *item, bool hide);

public slots:
    void setEntity(EntityShell *entity);
    void update();

signals:
    //void entityNameChanged(const QString&);
};

#endif // PROPERTYTREE_H
