# QtVtk
###### An integration between Qt Quick Controls 2 and VTK for 3D rendering
___

With this sample application you will be able to open multiple STL/OBJ files and view them in a 3D canvas.
This 3D canvas is rendered using VTK but is immersed into a QML window, where you can overlay any QtQuickControls2 component. In this example we have added a ComboBox to change the models representation, a Slider to change their opacity, a Switch to enable Gouraud interpolation and three SpinBoxes to choose the RGB color.

The code was tested using Qt 5.9.4 and VTK 8.1.1 in both Linux and Windows.

![Import and view STL files](resources/QtVtk_1.gif "Import and view STL files")

![Overlay QtQuickControls2 components](resources/QtVtk_2.gif "Overlay QtQuickControls2 components")


##### How to build in Linux

1. Clone the repository
    ```sh
    git clone https://github.com/nicanor-romero/QtVtk.git
    ```

2. Set the environmental variables
    ```sh
    $ export QTDIR=/path/to/Qt/5.9.4/gcc_64
    $ export VTK_DIR=/path/to/VTK-8.1.1/build_dir
    ```

3. Run CMake
    ```sh
    $ cd /path/to/QtVtk
    $ mkdir build && cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    ```

4. Compile the code
    ```sh
    $ make
    ```

5. Run the application
    ```sh
    $ cd build/
    $ ./QtVtk
    ```

