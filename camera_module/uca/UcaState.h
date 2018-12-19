#pragma once

#include <uca/uca-camera.h>
#include <uca/uca-plugin-manager.h>
#include <string>
#include <vector>

class UcaState
{
public:
  UcaState();
  ~UcaState();

  UcaPluginManager* getManager();

  UcaCamera* getCamera() const;
  std::string getCameraName() const;
  void setCameraByName(std::string const& cameraName);

  UcaCamera* createCameraByName(std::string const& cameraName) const;

  std::vector<std::string> getAvailableCameras() const;

  void setAutoLevel(bool rhs);
  bool getAutoLevel() const;

private:
  UcaPluginManager* mManager=nullptr;
  UcaCamera* mCamera=nullptr;
  std::string mCameraName;
  bool mAutoLevel=false;
};


