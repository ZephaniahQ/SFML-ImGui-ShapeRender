
## SFML-ImGui ShapeRender

This project uses a combinatoin of SFML and ImGui to make a simple renderer and control interface to load sfml shapes with different attributes dynamically from a provided file and renders them according to the attributes mentioned in the file;

The following are the attribs loaded from the config file.

### Shape attributes:

    - n         string name;
    - x         float starting coordinate x
    - y         float starting coordinate y
    - sx        float x velocity
    - sy        float y velocity
    - R         float radius for circles
    - W, H      float width and height for rectangles
    - R,G,B     int RBG color info

### Window and Font attributes:
    - sWidth, sHeight   int  screen width and height for window resolution
    - font file name    string filename for the font to load for shape names

## Functionality

### Screen border colision detection

After rendering the shapes are continuously checked for colision with the screen borders and if colision is detected their velocity is reversed to give a bound back effect

### ImGui control panel

An ImGui interface is provided to edit anything from any shapes attributes at runtime

## Libraries used:

    - SFML          
    - ImGui-SFML(SFML compatible third party version of ImGui)  

## Build system and platform compatability

Both libraries used are platform independant so build system is the only thing that will differ.
I am working in a WSL Debian environment and using GNU make as my build system.

For running this code in Windows you will find many tutorials to setup a Visual Studio project with SFML + ImGui-SFML online,
after that you can just coppy the source files.

Or you may use a build system like CMAKE and set it up like my make file.

# Note:

The source code deffinately has a lot of room for improvement especially in class abstraction, inheritance and member access. I am still a C++ beginner learning OOP.
