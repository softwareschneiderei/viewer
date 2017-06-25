#pragma once
#include <QAbstractTableModel>
#include <uca/uca-camera.h>

class UcaTableModel : public  QAbstractTableModel
{
public:
  UcaTableModel(UcaCamera* camera);
  ~UcaTableModel();

  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;

  QModelIndex parent(const QModelIndex &child) const override;
  QVariant data(const QModelIndex &index, int role) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


private:
  std::vector<GParamSpec*> mProperties;

};
