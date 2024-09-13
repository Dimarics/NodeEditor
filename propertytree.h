#ifndef PROPERTYTREE_H
#define PROPERTYTREE_H

#include "entityshell.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemDelegate>

class PropertyTree : public QTreeView
{
    Q_OBJECT
public:
    enum DataType
    {
        PropertyType = Qt::UserRole,
        ColorType,
        EditorType,
    };
    enum Editor
    {
        TextEditor = 2,
        ColorEditor,
        ByteEditor,
    };
    enum Property
    {
        Name = 2,
        AmbientColor,
        AmbientRed,
        AmbientGreen,
        AmbientBlue,
        DiffuseColor,
        DiffuseRed,
        DiffuseGreen,
        DiffuseBlue,
        SpecularColor,
        SpecularRed,
        SpecularGreen,
        SpecularBlue,
    };
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
    QStandardItem *m_transparency;

    //void setColorProperty(QStandardItem *root, const QString &name, const QColor &color, Property id);
    inline void setSectionHidden(QStandardItem *item, bool hide);

public slots:
    void setEntity(EntityShell *entity);
    void update();

signals:
    //void entityNameChanged(const QString&);
};

// Делегат
class PropertyTreeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    static void setItemColor(QStandardItem *item, const QColor &color);
    static void setItemColor(QStandardItem *item, const QVariant &var);
    static void setColorIcon(QStandardItem *item, const QColor &color);

protected:
    void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override;

signals:
    void editingFinished(QStandardItem*);
};

#endif // PROPERTYTREE_H
