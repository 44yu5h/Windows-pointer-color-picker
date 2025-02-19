import 'package:flutter/material.dart';
import 'package:win_color_picker/colorPicker.dart';
import 'package:window_size/window_size.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  setWindowTitle("Custom window title");
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: ColorPickerWidget(),
    );
  }
}

class ColorPickerWidget extends StatefulWidget {
  @override
  _ColorPickerWidgetState createState() => _ColorPickerWidgetState();
}

class _ColorPickerWidgetState extends State<ColorPickerWidget> {
  Color pickedColor = Colors.white;
  String hexColor = "#FFFFFF";

  @override
  void initState() {
    super.initState();
    startColorPicking();
  }

  void startColorPicking() {
    Future.delayed(Duration(milliseconds: 500), () async {
      while (mounted) {
        String color = await ColorPicker().getColorAtCursor();
        setState(() {
          hexColor = color;
          pickedColor = Color(int.parse(color.replaceFirst('#', '0xff')));
        });
        await Future.delayed(Duration(milliseconds: 500));
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
          backgroundColor: Colors.white,
          title: Center(
              child: Text(
            "what a picky person!",
            style: TextStyle(
              fontSize: 20,
              fontFamily: 'monospace',
            ),
          ))),
      backgroundColor: Colors.white, // Set background color to white
      body: Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            Container(
              width: 200,
              height: 200,
              decoration: BoxDecoration(
                color: pickedColor,
                borderRadius: BorderRadius.circular(16), // Add rounded corners
              ),
              child: Center(
                child: Text(
                  hexColor,
                  style: TextStyle(
                    color: Colors.black,
                    fontSize: 20,
                    fontFamily: 'monospace', // Set text style to monospace
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
