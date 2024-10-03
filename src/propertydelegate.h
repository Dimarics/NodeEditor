#ifndef PROPERTYDELEGATE_H
#define PROPERTYDELEGATE_H

#include <QItemDelegate>
#include <QStandardItem>

enum DataType
{
    PropertyType = 256,
    ColorType,
    EditorType,
};
enum Editor
{
    TextEditor = 2,
    ColorEditor,
    ByteEditor,
    IntEditor,
    SliderEditor,
};

class PropertyDelegate : public QItemDelegate
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

#endif // PROPERTYDELEGATE_H
