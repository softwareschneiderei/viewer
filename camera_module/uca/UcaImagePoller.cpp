
#include "UcaImagePoller.h"

#include <cstring>
#include "ui_UcaConfigure.h"
#include "UcaConfigure.h"

namespace {
guint bytesPerPixel(guint bits)
{
  if (bits==8||bits==16||bits==32)
    return bits / 8;
  if (bits==14)
    return 2;

  throw std::runtime_error("Unsupported bit depth");
}

void checkError(GError* error, std::string const &context)
{
  if (error== nullptr)
    return;

  auto message = context + error->message;
  g_error_free(error);
  throw std::runtime_error(message);
}

template <typename T>
AbstractImagePoller::Result blitImage(std::vector<std::uint8_t> const& buffer, unsigned int width, unsigned int height)
{
  QImage targetImage(width, height, QImage::Format_RGB32);

  std::uint32_t min = std::numeric_limits<std::uint32_t>::max();
  std::uint32_t max = std::numeric_limits<std::uint32_t>::min();

  for (int y = 0; y < height; ++y)
  {
    auto target = reinterpret_cast<QRgb*>(targetImage.scanLine(y));
    auto source = buffer.data() + y * width * sizeof(T);
    for (int x = 0; x < width; ++x)
    {
      T gray;
      std::memcpy(&gray, source + x*sizeof(T), sizeof(T));
      min = std::min(min, static_cast<std::uint32_t>(gray));
      max = std::max(max, static_cast<std::uint32_t>(gray));
      gray  = gray >> 8*(sizeof(T)-1);
      target[x] = qRgb(gray, gray, gray);
    }
  }

  return {targetImage, min, max};
}

template <typename T>
AbstractImagePoller::Result blitImageAutoLevel(std::vector<std::uint8_t> const& buffer, unsigned int width, unsigned int height)
{
  QImage targetImage(width, height, QImage::Format_RGB32);

  std::uint32_t min = std::numeric_limits<std::uint32_t>::max();
  std::uint32_t max = std::numeric_limits<std::uint32_t>::min();

  for (int y = 0; y < height; ++y)
  {
    auto source = buffer.data() + y * width * sizeof(T);
    for (int x = 0; x < width; ++x)
    {
      T gray;
      std::memcpy(&gray, source + x*sizeof(T), sizeof(T));
      min = std::min(min, static_cast<std::uint32_t>(gray));
      max = std::max(max, static_cast<std::uint32_t>(gray));
    }
  }

  if (min != max)
  {
    for (int y = 0; y < height; ++y)
    {
      auto target = reinterpret_cast<QRgb*>(targetImage.scanLine(y));
      auto source = buffer.data() + y * width * sizeof(T);
      for (int x = 0; x < width; ++x)
      {
        T gray;
        std::memcpy(&gray, source + x*sizeof(T), sizeof(T));
        gray  = (gray - min)*255 / (max - min);
        target[x] = qRgb(gray, gray, gray);
      }
    }
  }
  else
  {
    for (int y = 0; y < height; ++y)
    {
      auto target = reinterpret_cast<QRgb*>(targetImage.scanLine(y));
      for (int x = 0; x < width; ++x)
      {
        target[x] = qRgb(0, 0, 0);
      }
    }
  }

  return {targetImage, min, max};
}
}

UcaImagePoller::UcaImagePoller(std::string const& name)
: mState(std::make_shared<UcaState>())
{
  mState->setCameraByName(name);
}

UcaImagePoller::~UcaImagePoller() = default;

void UcaImagePoller::startAcquisition()
{
  /*g_object_set (G_OBJECT (mState->getCamera()),
                "trigger-source",
                UCA_CAMERA_TRIGGER_SOURCE_AUTO,
                NULL);*/

  g_object_get (G_OBJECT (mState->getCamera()),
                "roi-width", &mWidth,
                "roi-height", &mHeight,
                "sensor-bitdepth", &mBits,
                NULL);


  GError* error=nullptr;
  uca_camera_start_recording(mState->getCamera(), &error);

  checkError(error, "Error starting camera:\n");

  mBuffer.resize(bytesPerPixel(mBits)*mWidth*mHeight);
}

void UcaImagePoller::poll()
{
  GError* error=nullptr;
  uca_camera_grab(mState->getCamera(), mBuffer.data(), &error);
  checkError(error, "Error grabbing image:\n");

  if (mState->getAutoLevel())
  {
    switch(bytesPerPixel(mBits))
    {
      case 1:
        dispatch(blitImageAutoLevel<std::uint8_t>(mBuffer, mWidth, mHeight));
        return;
      case 2:
        dispatch(blitImageAutoLevel<std::uint16_t>(mBuffer, mWidth, mHeight));
        return;
      case 4:
        dispatch(blitImageAutoLevel<std::uint32_t>(mBuffer, mWidth, mHeight));
        return;
      default:
        return;
    }
  }
  else
  {
    switch(bytesPerPixel(mBits))
    {
      case 1:
        dispatch(blitImage<std::uint8_t>(mBuffer, mWidth, mHeight));
        return;
      case 2:
        dispatch(blitImage<std::uint16_t>(mBuffer, mWidth, mHeight));
        return;
      case 4:
        dispatch(blitImage<std::uint32_t>(mBuffer, mWidth, mHeight));
        return;
      default:
        return;
    }
  }
}

void UcaImagePoller::stopAcquisition()
{
  GError* error=nullptr;
  uca_camera_stop_recording (mState->getCamera(), &error);

  checkError(error, "Error stopping camera:\n");
}

QWidget* UcaImagePoller::configure(QWidget* parent)
{
  return new UcaConfigure(mState, parent);
}

void UcaImagePoller::trigger()
{
  GError* error=nullptr;
  uca_camera_trigger(mState->getCamera(), &error);
  checkError(error, "Error triggering camera:\n");
}
