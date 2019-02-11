#include "UcaTableModel.h"
#include <QDebug>

QModelIndex UcaTableModel::parent(const QModelIndex &child) const
{
  return {};
}

int UcaTableModel::rowCount(const QModelIndex &parent) const
{
  return static_cast<int>(mProperties.size());
}

int UcaTableModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant UcaTableModel::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DisplayRole)
  {
    return {};
  }

  auto parameterSpec = mProperties[index.row()];
  if (index.column() == 0)
  {
    QString name = g_param_spec_get_name(parameterSpec);
    return name;
  }
  else if (index.column() == 1)
  {
    GValue value = {};
    g_value_init(&value, G_PARAM_SPEC_VALUE_TYPE(parameterSpec));

    g_object_get_property(G_OBJECT(mCamera), g_param_spec_get_name(parameterSpec), &value);
    auto string = g_strdup_value_contents(&value);
    QString result(string);
    g_free(string);
    g_value_unset(&value);
    return result;
  }

  return {};
}

UcaTableModel::UcaTableModel(UcaCamera* camera)
  : mCamera(camera)
{
  if (camera == nullptr)
    return;

  guint propertyCount = 0;
  auto properties = g_object_class_list_properties(G_OBJECT_GET_CLASS(camera), &propertyCount);

  for (guint i = 0; i < propertyCount; ++i)
  {
    mProperties.push_back(properties[i]);
  }

  g_free(properties);
}

UcaTableModel::~UcaTableModel()
{


}

QVariant UcaTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
  {
    return QAbstractItemModel::headerData(section, orientation, role);
  }

  if (section == 0)
  {
    return QString("Property");
  }
  if (section == 1)
  {
    return QString("Value");
  }
  return {};
}

bool UcaTableModel::setData(const QModelIndex &index, QVariant const &newValue, int role)
{
  if (index.isValid() && role == Qt::EditRole)
  {
    int row = index.row();

    if (index.column() != 1)
    {
      return false;
    }

    qDebug() << "Edit: " << newValue;
    auto property = mProperties[index.row()];
    bool ok = true;

    GValue value = {0};
    g_value_init(&value, property->value_type);
    switch (property->value_type)
    {
    case G_TYPE_INT:
      g_value_set_int(&value, newValue.toInt(&ok));
      break;
    case G_TYPE_UINT:
      g_value_set_uint(&value, newValue.toUInt(&ok));
      break;
    case G_TYPE_DOUBLE:
      g_value_set_double(&value, newValue.toDouble(&ok));
      break;
    case G_TYPE_BOOLEAN:
      g_value_set_boolean(&value, newValue.toBool());
      break;
    default:
      ok = false;
      break;
    };

    if (ok)
    {
      g_object_set_property(G_OBJECT (mCamera), property->name, &value);
      emit(dataChanged(index, index));

      qDebug() << "Edited value";
    }
    else
    {
      qDebug() << "Did not edit value";
    }

    g_value_unset(&value);

    return true;
  }

  return false;
}

Qt::ItemFlags UcaTableModel::flags(QModelIndex const &index) const
{
  auto property = mProperties[index.row()];
  if (property && property->flags & G_PARAM_WRITABLE)
  {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  }

  return QAbstractTableModel::flags(index);
}
