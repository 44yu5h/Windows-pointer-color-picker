#include "flutter_window.h"

#include <optional>
#include <iostream>
#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>
#include <flutter/method_result_functions.h>

FlutterWindow::FlutterWindow(const flutter::DartProject &project) : project_(project) {}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate()
{
  if (!Win32Window::OnCreate())
  {
    return false;
  }

  RECT frame;
  GetClientRect(GetHandle(), &frame);

  flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
      frame.right - frame.left, frame.bottom - frame.top, project_);

  if (!flutter_controller_->engine() || !flutter_controller_->view())
  {
    return false;
  }

  SetChildContent(flutter_controller_->view()->GetNativeWindow());

  flutter_controller_->engine()->SetNextFrameCallback([&]()
                                                      { this->Show(); });

  SetupPlatformChannel(); // Setup the method channel for Flutter communication

  return true;
}

void FlutterWindow::OnDestroy()
{
  if (flutter_controller_)
  {
    flutter_controller_ = nullptr;
  }
  Win32Window::OnDestroy();
}

LRESULT FlutterWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) noexcept
{
  if (flutter_controller_)
  {
    std::optional<LRESULT> result = flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam, lparam);
    if (result)
    {
      return *result;
    }
  }

  return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}

// 📌 Get the color at the cursor position
COLORREF FlutterWindow::GetColorAtPoint(int x, int y)
{
  HDC hdc = GetDC(NULL);
  if (!hdc)
  {
    return RGB(255, 255, 255);
  }

  COLORREF color = GetPixel(hdc, x, y);
  ReleaseDC(NULL, hdc);
  return color;
}

// 📌 Setup platform channel for Flutter
void FlutterWindow::SetupPlatformChannel()
{
  auto messenger = flutter_controller_->engine()->messenger();

  auto channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      messenger, "com.example.colorpicker", &flutter::StandardMethodCodec::GetInstance());

  channel->SetMethodCallHandler(
      [this](const flutter::MethodCall<flutter::EncodableValue> &call,
             std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
      {
        if (call.method_name() == "getColorAt")
        {
          POINT cursorPos;
          if (GetCursorPos(&cursorPos))
          {
            int x = cursorPos.x;
            int y = cursorPos.y;
            COLORREF color = GetColorAtPoint(x, y);

            char hexColor[10];
            sprintf_s(hexColor, "#%02X%02X%02X", GetRValue(color), GetGValue(color), GetBValue(color));

            result->Success(flutter::EncodableValue(std::string(hexColor)));
          }
          else
          {
            result->Error("UNAVAILABLE", "Failed to get cursor position");
          }
        }
        else
        {
          result->NotImplemented();
        }
      });

  color_channel_ = std::move(channel);
}
