#include "propertytree.h"
#include "propertydelegate.h"

//#include <QHeaderView>

PropertyTree::PropertyTree(QWidget *parent) : QTreeView(parent),
    m_model(new QStandardItemModel),
    m_entityName(new QStandardItem),
    m_hide(new QStandardItem),
    m_materialSection(new QStandardItem("Материал")),
    m_ambientColor(new QStandardItem),
    m_ambientRed(new QStandardItem),
    m_ambientGreen(new QStandardItem),
    m_ambientBlue(new QStandardItem),
    m_diffuseColor(new QStandardItem),
    m_diffuseRed(new QStandardItem),
    m_diffuseGreen(new QStandardItem),
    m_diffuseBlue(new QStandardItem),
    m_specularColor(new QStandardItem),
    m_specularRed(new QStandardItem),
    m_specularGreen(new QStandardItem),
    m_specularBlue(new QStandardItem),
    m_shininess(new QStandardItem),
    m_transparency(new QStandardItem)
{
    setModel(m_model);
    //QHeaderView *header = new QHeaderView(Qt::Horizontal, this);
    //header->setSectionsMovable(false);
    //header->setSectionsClickable(false);
    //header->setSectionResizeMode(QHeaderView::Stretch);
    m_model->setHorizontalHeaderLabels({"Свойство", "Значение"});
    //setHeader(header);

    // Свойства
    m_model->appendRow({new QStandardItem("Имя"), m_entityName});
    m_entityName->setData(TextEditor, EditorType);

    // Скрыть
    m_hide->setCheckable(true);
    m_model->appendRow({new QStandardItem("Скрыть"), m_hide});
    connect(m_model, &QStandardItemModel::itemChanged, this, [this](QStandardItem *item)
    {
        if (item == m_hide)
        {
        }
        else if (item == m_ambientColor)
        {
        }
    });

    // Материал
    m_model->appendRow(m_materialSection);
    // Ambient
    QStandardItem *ambientSection = new QStandardItem("Ambient");
    m_ambientColor->setData(ColorEditor, EditorType);
    m_materialSection->appendRow({ambientSection, m_ambientColor});
    m_ambientRed->setData(ByteEditor, EditorType);
    ambientSection->appendRow({new QStandardItem("Красный"), m_ambientRed});
    m_ambientGreen->setData(ByteEditor, EditorType);
    ambientSection->appendRow({new QStandardItem("Зелёный"), m_ambientGreen});
    m_ambientBlue->setData(ByteEditor, EditorType);
    ambientSection->appendRow({new QStandardItem("Синий"), m_ambientBlue});

    // Diffuse
    QStandardItem *diffuseSection = new QStandardItem("Diffuse");
    m_diffuseColor->setData(ColorEditor, EditorType);
    m_materialSection->appendRow({diffuseSection, m_diffuseColor});
    m_diffuseRed->setData(ByteEditor, EditorType);
    diffuseSection->appendRow({new QStandardItem("Красный"), m_diffuseRed});
    m_diffuseGreen->setData(ByteEditor, EditorType);
    diffuseSection->appendRow({new QStandardItem("Зелёный"), m_diffuseGreen});
    m_diffuseBlue->setData(ByteEditor, EditorType);
    diffuseSection->appendRow({new QStandardItem("Синий"), m_diffuseBlue});

    // Specular
    QStandardItem *specularSection = new QStandardItem("Блик");
    m_specularColor->setData(ColorEditor, EditorType);
    m_materialSection->appendRow({specularSection, m_specularColor});
    m_specularRed->setData(ByteEditor, EditorType);
    specularSection->appendRow({new QStandardItem("Красный"), m_specularRed});
    m_specularGreen->setData(ByteEditor, EditorType);
    specularSection->appendRow({new QStandardItem("Зелёный"), m_specularGreen});
    m_specularBlue->setData(ByteEditor, EditorType);
    specularSection->appendRow({new QStandardItem("Синий"), m_specularBlue});

    // Блеск
    QStandardItem *shininessSection = new QStandardItem("Блеск");
    m_shininess->setData(IntEditor, EditorType);
    m_materialSection->appendRow({shininessSection, m_shininess});

    // Прозрачность
    QStandardItem *transparencySection = new QStandardItem("Прозрачность");
    m_transparency->setData(ByteEditor, EditorType);
    m_materialSection->appendRow({transparencySection, m_transparency});

    //----------------
    setEntity(nullptr);

    // Делегат
    PropertyDelegate *delegate = new PropertyDelegate;
    setItemDelegate(delegate);
    //connect(m_model, &QStandardItemModel::itemChanged, this, [this](QStandardItem *item)
    connect(delegate, &PropertyDelegate::editingFinished, this, [this](QStandardItem *item)
    {
        if (item == m_entityName)
        {
            m_entityShell->setName(item->text());
            //emit entityNameChanged(item->text());
        }

        else if (item == m_ambientColor)
        {
            QColor color = m_ambientColor->data(ColorType).value<QColor>();
            m_entityShell->setAmbient(color);
            m_ambientRed->setText(QString::number(color.red()));
            m_ambientGreen->setText(QString::number(color.green()));
            m_ambientBlue->setText(QString::number(color.blue()));
        }
        else if (item == m_ambientRed || item == m_ambientGreen || item == m_ambientBlue)
        {
            int r = m_ambientRed->data(Qt::DisplayRole).toInt();
            int g = m_ambientGreen->data(Qt::DisplayRole).toInt();
            int b = m_ambientBlue->data(Qt::DisplayRole).toInt();
            QColor color(r, g, b);
            PropertyDelegate::setItemColor(m_ambientColor, color);
            m_entityShell->setAmbient(color);
        }

        else if (item == m_ambientColor)
        {
            QColor color = m_ambientColor->data(ColorType).value<QColor>();
            m_entityShell->setAmbient(color);
            m_ambientRed->setText(QString::number(color.red()));
            m_ambientGreen->setText(QString::number(color.green()));
            m_ambientBlue->setText(QString::number(color.blue()));
        }
        else if (item == m_ambientRed || item == m_ambientGreen || item == m_ambientBlue)
        {
            int r = m_ambientRed->data(Qt::DisplayRole).toInt();
            int g = m_ambientGreen->data(Qt::DisplayRole).toInt();
            int b = m_ambientBlue->data(Qt::DisplayRole).toInt();
            QColor color(r, g, b);
            PropertyDelegate::setItemColor(m_ambientColor, color);
            m_entityShell->setAmbient(color);
        }

        else if (item == m_diffuseColor)
        {
            QVariant var = m_diffuseColor->data(ColorType);
            QColor color = var.value<QColor>();
            color.setAlpha(m_transparency->data(Qt::DisplayRole).toInt());
            m_entityShell->setDiffuse(color);
            m_diffuseRed->setText(QString::number(color.red()));
            m_diffuseGreen->setText(QString::number(color.green()));
            m_diffuseBlue->setText(QString::number(color.blue()));
        }
        else if (item == m_diffuseRed || item == m_diffuseGreen || item == m_diffuseBlue)
        {
            int r = m_diffuseRed->data(Qt::DisplayRole).toInt();
            int g = m_diffuseGreen->data(Qt::DisplayRole).toInt();
            int b = m_diffuseBlue->data(Qt::DisplayRole).toInt();
            int a = m_transparency->data(Qt::DisplayRole).toInt();
            QColor color(r, g, b, a);
            PropertyDelegate::setItemColor(m_diffuseColor, color);
            m_entityShell->setDiffuse(color);
        }

        else if (item == m_specularColor)
        {
            QVariant var = m_specularColor->data(ColorType);
            QColor color = var.value<QColor>();
            m_entityShell->setSpecular(color);
            m_specularRed->setText(QString::number(color.red()));
            m_specularGreen->setText(QString::number(color.green()));
            m_specularBlue->setText(QString::number(color.blue()));
        }
        else if (item == m_specularRed || item == m_specularGreen || item == m_specularBlue)
        {
            int r = m_specularRed->data(Qt::DisplayRole).toInt();
            int g = m_specularGreen->data(Qt::DisplayRole).toInt();
            int b = m_specularBlue->data(Qt::DisplayRole).toInt();
            QColor color(r, g, b);
            PropertyDelegate::setItemColor(m_specularColor, color);
            m_entityShell->setSpecular(color);
        }

        else if (item == m_shininess)
        {
            m_entityShell->setShininess(m_shininess->data(Qt::DisplayRole).toInt());
        }

        else if (item == m_transparency)
        {
            m_entityShell->setTransparency(m_transparency->data(Qt::DisplayRole).toInt());
        }
    });
}

void PropertyTree::setEntity(EntityShell *entityShell)
{
    setSectionHidden(m_entityName, true);
    setSectionHidden(m_materialSection, true);
    setSectionHidden(m_hide, true);

    if (entityShell == nullptr) return;

    m_entityShell = entityShell;
    setSectionHidden(m_entityName, false);
    setSectionHidden(m_hide, false);
    m_entityName->setText(entityShell->name());
    if (entityShell->hasMaterial())
    {
        setSectionHidden(m_materialSection, false);

        PropertyDelegate::setItemColor(m_ambientColor, m_entityShell->ambient());
        //m_ambientColor->setData(m_material->ambient(), ColorType);
        m_ambientRed->setText(QString::number(m_entityShell->ambient().red()));
        m_ambientGreen->setText(QString::number(m_entityShell->ambient().green()));
        m_ambientBlue->setText(QString::number(m_entityShell->ambient().blue()));

        PropertyDelegate::setItemColor(m_diffuseColor, m_entityShell->diffuse());
        m_diffuseRed->setText(QString::number(m_entityShell->diffuse().red()));
        m_diffuseGreen->setText(QString::number(m_entityShell->diffuse().green()));
        m_diffuseBlue->setText(QString::number(m_entityShell->diffuse().blue()));

        PropertyDelegate::setItemColor(m_specularColor, m_entityShell->specular());
        m_specularColor->setData(m_entityShell->specular(), ColorType);
        m_specularRed->setText(QString::number(m_entityShell->specular().red()));
        m_specularGreen->setText(QString::number(m_entityShell->specular().green()));
        m_specularBlue->setText(QString::number(m_entityShell->specular().blue()));

        m_shininess->setText(QString::number(m_entityShell->shininess()));

        m_transparency->setText(QString::number(m_entityShell->diffuse().alpha()));
    }
    /*m_entity = entity;

    QStandardItem *name = new QStandardItem(m_entity->objectName());
    name->setData(Name, PropertyType);
    name->setData(TextEditor, EditorType);
    m_model->appendRow({new QStandardItem("Имя"), name});

    for (Qt3DCore::QComponent *component : m_entity->components())
    {
        if (qobject_cast<Qt3DExtras::QPhongMaterial*>(component))
        {
            m_material = qobject_cast<Qt3DExtras::QPhongMaterial*>(component);
            QStandardItem *materialItem = new QStandardItem("Материал");
            m_model->appendRow(materialItem);

            setColorProperty(materialItem, "Ambient", m_material->ambient(), AmbientColor);
            setColorProperty(materialItem, "Diffuse", m_material->diffuse(), DiffuseColor);
            setColorProperty(materialItem, "Specular", m_material->specular(), SpecularColor);

            QStandardItem *shininess = new QStandardItem;
            materialItem->appendRow(shininess);
            shininess->setText("Блеск");
            //shininess->setText(1, QString::number(material->shininess()));
        }
    }*/

    //clear();
    /*setSectionHidden(m_entityName, true);
    setSectionHidden(m_materialSection, true);
    setSectionHidden(m_hide, true);
    m_entity = entity;
    if (entity == nullptr) return;

    setSectionHidden(m_entityName, false);
    setSectionHidden(m_hide, false);
    m_entityName->setText(entity->objectName());
    for (Qt3DCore::QComponent *component : m_entity->components())
    {
        if (qobject_cast<Qt3DExtras::QDiffuseSpecularMaterial*>(component))
        {
            setSectionHidden(m_materialSection, false);
            m_material = qobject_cast<Qt3DExtras::QDiffuseSpecularMaterial*>(component);

            PropertyDelegate::setItemColor(m_ambientColor, m_material->ambient());
            //m_ambientColor->setData(m_material->ambient(), ColorType);
            m_ambientRed->setText(QString::number(m_material->ambient().red()));
            m_ambientGreen->setText(QString::number(m_material->ambient().green()));
            m_ambientBlue->setText(QString::number(m_material->ambient().blue()));

            QColor diffuse = m_material->diffuse().value<QColor>();
            PropertyDelegate::setItemColor(m_diffuseColor, m_material->diffuse());
            m_diffuseRed->setText(QString::number(diffuse.red()));
            m_diffuseGreen->setText(QString::number(diffuse.green()));
            m_diffuseBlue->setText(QString::number(diffuse.blue()));

            QColor specular = m_material->specular().value<QColor>();
            PropertyDelegate::setItemColor(m_specularColor, m_material->specular());
            m_specularColor->setData(m_material->specular(), ColorType);
            m_specularRed->setText(QString::number(specular.red()));
            m_specularGreen->setText(QString::number(specular.green()));
            m_specularBlue->setText(QString::number(specular.blue()));

            m_transparency->setText(QString::number(diffuse.alpha()));
        }
    }*/
}

void PropertyTree::update()
{
    setModel(m_model);
}

void PropertyTree::clear()
{
    //m_model->removeRows(0, m_model->rowCount());
    //while (m_model->rowCount())
    //    m_model->takeRow(0);
}

void PropertyTree::setSectionHidden(QStandardItem *item, bool hide)
{
    setRowHidden(item->row(), QModelIndex(), hide);
}

/*void PropertyTree::setColorProperty(QStandardItem *root, const QString &name, const QColor &color, Property id)
{
    QStandardItem *colorProperty = new QStandardItem(name);

    QStandardItem *colorValue = new QStandardItem(color.name());
    PropertyDelegate::setColorIcon(colorValue, color);
    colorValue->setData(id, PropertyType);
    colorValue->setData(ColorEditor, EditorType);
    colorValue->setData(color, ColorType);
    root->appendRow({colorProperty, colorValue});

    QStandardItem *red = new QStandardItem(QString::number(color.red()));
    red->setData(id + 1, PropertyType);
    red->setData(ByteEditor, EditorType);
    colorProperty->appendRow({new QStandardItem("Красный"), red});

    QStandardItem *green = new QStandardItem(QString::number(color.green()));
    green->setData(id + 2, PropertyType);
    green->setData(ByteEditor, EditorType);
    colorProperty->appendRow({new QStandardItem("Зелёный"), green});

    QStandardItem *blue = new QStandardItem(QString::number(color.blue()));
    blue->setData(id + 3, PropertyType);
    blue->setData(ByteEditor, EditorType);
    colorProperty->appendRow({new QStandardItem("Синий"), blue});
}*/
