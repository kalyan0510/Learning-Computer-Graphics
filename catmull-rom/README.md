### Note:
For C2 splines: 
- To ensure c2 continuity, I have followed the below process (since as discussed in the class, stitching can be customized to ensure the conditions). 
    - After the first 4 points, for every new point two new intermediate points are added. The first intermediate point ensures C1 continuity, while the second ensures the C2 continuity. 
    - So, the curve passes through all the input points except second and third 

For Catmull-rom splines: 
- The tangents for the last and first points are interpolated from their neighbours, respectively. So, the curve starts rendering right after the second point.
      
My rough work towards arriving at conditions for stitching the curves: https://drive.google.com/file/d/1QzVPN-Mpbv6ju12TR3McYWEi-S9V1nQX/view?usp=sharing
## Features Implemented

- [x] 4 Point Bezier curve segment
- [x] Piecewise  Bezier curve 
- [x] Piecewise  Catmull-Rom spline
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

- To run this program, use console command `./bin/catmull_rom` (or `./build/catmull_rom` if you compiled this program manually) in the src directory. 
- Hit number key 1~3 to switch between solutions for different problems. 
    - 1: 4 Point Bezier curve segment(default)
        - Mouse left-click: add points for the bezier segment (atmost latest 4 are preserved). 
        - After left-click, there will be a preview of the curve as you move your mouse. 
        - Right-click your mouse to add the final point. 
        - switch to another problem by clicking '2' or '3', and come back to 1 to see this page refreshed. 
    - 2: Piecewise  Bezier curve 
        - Mouse left-click: add points for the piecewise bezier segment. 
        - After each left-click (and while hover) after adding atleast 3 points, there will be a preview of the curve as you move your mouse. 
        - Right-click your mouse to finally freeze the points. 
        - Drag: click on a point and drag to modify the curve
        - Add: left click on two consecutive points or one edge points, and right click on new place to add a point in between them. 
        - Delete: left click (make sure the mouse does not change position while click happens) and press delete key to delete a given point,
        - press L: to load presaved curve points.
        - press S: to save current curve points.
        - press R: to refresh (i.e. forget selected points, re-render view) 
    - 3: Piecewise  Catmull-Rom spline
        - Mouse left-click: add points for the piecewise Catmull-Rom splines. 
        - After each left-click (and while hover) after adding atleast 3 points, there will be a preview of the curve as you move your mouse. 
        - Right-click your mouse to finally freeze the points. 
        - Drag: click on a point and drag to modify the curve
        - Add: left click on two consecutive points or one edge points, and right click on new place to add a point in between them. 
        - Delete: left click (make sure the mouse does not change position while click happens) and press delete key to delete a given point,
        - press L: to load presaved curve points.
        - press S: to save current curve points.
        - press R: to refresh (i.e. forget selected points, re-render view) 
