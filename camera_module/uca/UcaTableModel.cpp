#include "UcaTableModel.h"

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
  return 1;
}

QVariant UcaTableModel::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DisplayRole)
    return {};

  auto property = mProperties[index.row()];
  QString name = property->name;
  return name;
}

UcaTableModel::UcaTableModel(UcaCamera* camera)
{
  guint propertyCount = 0;
  auto properties = g_object_class_list_properties(G_OBJECT_GET_CLASS(camera), &propertyCount);

  for (guint i=0; i < propertyCount; ++i)
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
    return QAbstractItemModel::headerData(section, orientation, role);

  return QString("Property");
}
