#ifndef RUNNER_FLUTTER_WINDOW_H_
#define RUNNER_FLUTTER_WINDOW_H_

#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <flutter/method_channel.h>        // Include this
#include <flutter/standard_method_codec.h> // Include this

#include <memory>

#include "win32_window.h"

class FlutterWindow : public Win32Window
{
public:
  explicit FlutterWindow(const flutter::DartProject &project);
  virtual ~FlutterWindow();

protected:
  bool OnCreate() override;
  void OnDestroy() override;
  LRESULT MessageHandler(HWND window, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept override;

private:
  flutter::DartProject project_;
  void SetupPlatformChannel();

  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> color_channel_; // Ensure correct reference
  COLORREF GetColorAtPoint(int x, int y);

  std::unique_ptr<flutter::FlutterViewController> flutter_controller_;
};

#endif // RUNNER_FLUTTER_WINDOW_H_
