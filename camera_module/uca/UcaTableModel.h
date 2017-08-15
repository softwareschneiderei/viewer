#pragma once
#include <QAbstractTableModel>
#include <uca/uca-camera.h>

class UcaTableModel : public  QAbstractTableModel
{
public:
  explicit UcaTableModel(UcaCamera* camera);
  ~UcaTableModel() override;

  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;

  QModelIndex parent(const QModelIndex &child) const override;
  QVariant data(const QModelIndex &index, int role) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  Qt::ItemFlags flags(QModelIndex const& index) const override;
private:
  UcaCamera* mCamera;
  std::vector<GParamSpec*> mProperties;

};
