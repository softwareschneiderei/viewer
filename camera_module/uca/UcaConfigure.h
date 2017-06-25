#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <memory>

namespace Ui
{
  class UcaConfigureDialog;
}

class UcaState;

class UcaConfigure : public QDialog
{
public:
  UcaConfigure(std::shared_ptr<UcaState> state, QWidget* parent);
  ~UcaConfigure();

  void onCameraSelected(QString const& camera);

private:
  std::unique_ptr<Ui::UcaConfigureDialog> mUi;
  std::shared_ptr<UcaState> mState;

  void setupTableModel() const;
};


