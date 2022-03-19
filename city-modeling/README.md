## Features Implemented

- [x] P1: 1st three shapes
- [x] P2: Icosahedron->Circle w/ subdivision
- [x] P3: sphere, a cylinder, and a cone
- [x] P4: Quad ellipsoid
- [x] P5: Torus
- [x] P6: Super Quadrics 
- [x] P7: Dodecahedron
- [x] P8: City
- [x] P9: Bonus 


(the code is very messy as this is a time-limited assignment task)

1. To extract the vertices from parametric equations, I had used tesellation shaders instead of processing them in the CPU.
2. Copmuting normals with this approach had become a bit complex, so I tried to handle it in a separate geometry shader. And it finally worked. 
3. I implemented the 2nd problem's subdivision in hardware accellerated way. In the assignemnt it was asked to subdivide in the c++ code which is run by CPU. But I also implemented another solution at F10, where tesellation shaders are used to subdivide existing vertices.  
3. I tried to make the city interesting by adding football fields and some buildings that are made out of super quadrics. 

## Compile & Run

### How to Compile

Run the following console commands in the same directory as this file: 

```
mkdir build
cd build
cmake ..
make
```

### Dependencies

- OpenGL & GLEW
- glfw3
- glad

## Usage

- To run this program, use console command `./bin/city_modeling` (or `./build/city_modeling` if you compiled this program manually) in the src directory. 
- All the instructions mentioned in the city_modeling are followed.
- Explore probem solutions using F1,F2,....F10
- W,A,S,D to move the camera
- Trackpad to orient the camera
- Scroll to zoom in/out
- 1,2,3,4 to change the display modes
- (Shift+=) will increase the subdivision levels and if max is reached it goes back to minimal subdivision level and rotate
- same with the '-', but it decreases the subdivision level
- H wil do a horizontal loop  (usefull only in F8,F9) and V will do a vertical loop
