# Programming Assignment 1

I have laid an algorithm to rasterize polynomials based on the midpoint strategy. The solution works for all third degree polynomials (y=ax^3+ bx^2 + cx +d | a!=0).
I have not added in any clipping strategy, so some of the points on polynomial that fall beyond the screen will be computed. 
In some cases where polynomial goes very far (like y = 30k) beyond the screen due to sharp peaks, the UI seems to lag as many computations are done. (if the polynomial is very abrupt, this may take very long time). 

My rough work towards arriving to cubic polynomial solution: https://drive.google.com/file/d/1giXA1vo2_nC94kC5rrjbUvU6Xpc6Rogq/view?usp=sharing

## Features Implemented

- [x] Line segment (any slope)
- [x] Ploy-line (& Polygon)
- [x] Circle
- [x] Ellipse
- [x] Cubic curve

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

- To run this program, use console command `./bin/line_curve_draw` (or `./build/line_curve_draw` if you compiled this program manually) 
  in the same directory as this file. 
- Hit number key 1~6 to switch between different functionalities. 
    - 1: Line Segment Mode (default)
        - Mouse left-click: Set start point of the line segment. 
        - After left-click, there will be a preview of the line as you move your mouse. 
        - Right-click your mouse to finally determine the line. 
        - Left-click your mouse again to clean the canvas and start drawing another line. 
    - 2: Poly-line Mode
        - Mouse left-click: Set start point of the poly-line, or add new nodes. 
        - After each left-click, there will be a preview of the line as you move your mouse. 
        - Right-click your mouse to finally determine the poly-line. 
        - Switch to other modes and switch back to clear the screen. 
    - 3: Polygon Mode
        - Same as ploy-line, except polyline will be closed automatically when you right-click the mouse. 
        - Switch to other modes and switch back to clear the screen.
    - 4: Circle Mode
        - Mouse left-click: Set center of circle 
        - After left-click, there will be a preview of the circle as you move your mouse. 
        - Right-click your mouse to finally determine the circle. 
        - Left-click your mouse again to clean the canvas and start drawing another circle. 
        - Switch to other modes and switch back to clear the screen.
    - 5: Ellipse Mode
        - Simmilar to the circle mode, but the length of small and large axes of ellipse is defined by the |x1-x2| distance and |y1-y2| distance of selected and current cursor points
    - 6: Cubic Curve Mode
        - There are 6 polynomials displayed in this mode by default. We can display all the polynomials by pressing the mouse button (either left or right).
        - Furthermore more polynomials can be added by adding new (a,b,c,d) attributes to the file named curveparams.txt which sits in the main folder along with README.md 
        - As mentioned in the above note regarding cubic curves, clipping of the curve is not implemented. That means, while x is between -screenWidth/2 (i.e., at x=-800) and +screenWidth/2 (i.e., at x=+800), if the value can go to extremes of y(x) like +/-10000 or more, the interface may look hanged up cause it will be computing points in that timeperiod (wchich might not be shown on the screen).



