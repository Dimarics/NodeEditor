#include "propertydelegate.h"

#include <QPainter>
#include <QLineEdit>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QSpinBox>
#include <QSlider>
#include <QMetaProperty>

// Делегат
void PropertyDelegate::setItemColor(QStandardItem *item, const QColor &color)
{
    if (color.isValid())
    {
        item->setData(color, ColorType);
        item->setText(QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()));
        setColorIcon(item, color);
    }
}

void PropertyDelegate::setItemColor(QStandardItem *item, const QVariant &var)
{
    setItemColor(item, var.value<QColor>());
}

void PropertyDelegate::setColorIcon(QStandardItem *item, const QColor &color)
{
    QPixmap pixmap(14, 14);
    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    painter.setBrush(color);
    painter.drawRect(0, 0, pixmap.width() - 1, pixmap.height() - 1);
    item->setIcon(pixmap);
}

void PropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem item_option(option);
    if (item_option.state & QStyle::State_HasFocus)
        item_option.state = item_option.state ^ QStyle::State_HasFocus;
    QItemDelegate::paint(painter, item_option, index);
}

void PropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QByteArray n = editor->metaObject()->userProperty().name();
    if (!n.isEmpty()) model->setData(index, editor->property(n), Qt::EditRole);
    emit const_cast<PropertyDelegate*>(this)->
            editingFinished(qobject_cast<QStandardItemModel*>(model)->itemFromIndex(index));
}

QWidget* PropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const
{
    if (!index.data(EditorType).canConvert<uint>())
        return nullptr;
    switch (index.data(EditorType).toUInt())
    {
    case TextEditor:
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        return lineEdit;
    }
        break;

    case ColorEditor:
    {
        QWidget *colorCell = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout(colorCell);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addStretch();
        colorCell->setLayout(layout);

        QPushButton *dialogButton = new QPushButton("...", colorCell);
        //dialogButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        dialogButton->setFixedWidth(20);
        layout->addWidget(dialogButton);

        QStandardItem *item = qobject_cast<const QStandardItemModel*>(index.model())->itemFromIndex(index);

        connect(dialogButton, &QPushButton::clicked, this, [this, item]
        {
            QColor color = QColorDialog::getColor(item->data(ColorType).value<QColor>());
            if (color.isValid())
            {
                setItemColor(item, color);
                emit const_cast<PropertyDelegate*>(this)->editingFinished(item);
            }
        });
        return colorCell;
    }
        break;

    case ByteEditor:
    {
        QSpinBox *comboBox = new QSpinBox(parent);
        comboBox->setMinimum(0);
        comboBox->setMaximum(255);
        return comboBox;
    }
        break;

    case IntEditor:
    {
        QSpinBox *comboBox = new QSpinBox(parent);
        comboBox->setMinimum(0);
        comboBox->setMaximum(INT_MAX);
        return comboBox;
    }
        break;

    case SliderEditor:
    {
        QSlider *slider = new QSlider(parent);
        slider->setMinimum(0);
        slider->setMaximum(100);
        return slider;
    }
        break;

    default:
        break;
    }
    return nullptr;
}
