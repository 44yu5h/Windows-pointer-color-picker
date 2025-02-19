import 'package:flutter/services.dart';

class ColorPicker {
  static const platform = MethodChannel('com.example.colorpicker');

  Future<String> getColorAtCursor() async {
    try {
      final String color = await platform.invokeMethod('getColorAt');
      return color;
    } on PlatformException catch (e) {
      print("Error: ${e.message}");
      return "#FFFFFF"; // Default to white if there's an error
    }
  }
}
