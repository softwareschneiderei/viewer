#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <memory>
#include <uca/uca-plugin-manager.h>

namespace Ui
{
  class UcaConfigureDialog;
}

class UcaConfigure : public QDialog
{
public:
  UcaConfigure(UcaPluginManager* manager, QWidget* parent);
  ~UcaConfigure();

private:
  std::unique_ptr<Ui::UcaConfigureDialog> mUi;
  UcaPluginManager* mManager;

};


