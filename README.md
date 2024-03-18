# DownE
DownE is a low level engine made in C++, using opengl and a whole lot of object oriented programming patterns. Made for a university discipline.

### Documentation
All functions are documented in their respective headers (aside from main.cpp).

### How to compile the code
Use `make all` to compile the code and `./build/main` to start it.

### Usage
The program uses basically the whole keyboard for different actions because of assinment requirements and personal objectives:
* WASD moves the camera in the XZ plane, while space and E to move up and down (Y plane). Use the mouse to look around.
* HJKL moves the light source, while F and G move up and down.
* RTY increases RGB for the ambient lighting, and UIO decreases it. Each RGB component starts at one and is between 1 and 0.
* Numbers select an object (except light, skybox and camera).
* With an object selected, CVBN moves it (in the same scheme as HJKL), while M and comma move up and down. Z and X increase and decrease the scale of the object, arrow keys rotate it (yaw and pitch), and period toggles the object to use different texture modes (texture linear interpolation with GL_LINEAR versus pixelated textures with GL_NEAREST).
* P toggles all objects to draw their mesh grid instead of the full object and vice-versa.

Note that all objects are bound by the skybox. If any try to go past it they are pushed back. The only exception is scaling an object past the size of the whole skybox.

Adding new objects is trivial: just add the object mesh in res/meshes/ directory, add all the necessary textures in res/textures/, and edit src/main.cpp to add the new object with a matching mesh name (for instance, a "mario" object with mesh in res/meshes/mario would have a mesh name "mario").

Note that because of the flyweight pattern used, creating many instances of the same object is very cheap in terms of GPU VRAM and processing.

