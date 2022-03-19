#### A note regarding cubic polynomial solution
I have coded an algorithm to detect ellipse collisions. This is used in all the sub-programs to detect collisions of circles, faces and ellipses.
I had derived the collision detection equations using the matlab code I wrote, which is in the mentioned pdf(matlab code + rough work):
https://drive.google.com/file/d/1CSYccXwQ3Q3U0knSSWiHS8P9smSGjRQm/view?usp=sharing

I had used brent's algorithm from [here](https://cenit.github.io/jburkardt/brent/brent.html). This is used to minimize a (non quadratic) distance function which is not solvable analytacilly ([ref](https://stackoverflow.com/questions/2945337/how-to-detect-if-an-ellipse-intersectscollides-with-a-circle))

\* The masses of all the objects are assumed to be same.
## Features Implemented

- [x] P1: Bouncing Balls
- [x] P2: Four Bouncing Balls
- [x] P3: Bouncing Faces
- [x] P4: Four Bouncing Faces
- [x] P5: Bouncing ELLIPSES and faces

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
- brent.cpp (included in the folder include/brent/brent.cpp)

## Usage

- To run this program, use console command `./bin/boucing_and_colliding_ellipses` (or `./build/boucing_and_colliding_ellipses` if you compiled this program manually) 
  in the same directory as this file. 
- Hit number key 1~5 to switch between different problem's solutions. 
    - 1: Bouncing Balls (default)
        - Mouse left-click: Initialize a bouncing ball at the location of cursor. The ball has the radius, velocity (pixels per frame step) and direction of velocity (in degrees 0 to 360) parameters pulled from three values present in the file 'body_params.txt'
        - After left-click, there ball appears on the screen and starts moving. 
        - NOTE: At any step press 'SPACE' on keyboard to delete all the objects
        - NOTE: At any step press 'D' to enter debug mode where lines were drawn from an object to the predicted collision point on other ellipse (or circle).
        - NOTE: press 'F' to disable debug mode
    - 2: Four Bouncing Balls 
        - This is just as same as the above mode. Just added to help TA w.r.t the task being graded.
    - 3: Bouncing Faces
        - Switching to this mode clears any objects present on the screen. 
        - The procedure is just as same as that of above mode. Except a recursive faces are drawn instead of Shaded Circles.
    - 4:  Four Bouncing Faces
        - This is just as same as above mode.
    - 5: Ellipse Mode
        - This mode has the solutions for the last bonus problem. I had chosen to implement collison detection for objects more complex than circle, which is an ellipse.
        - Use the parameters in 'ellipse_params.txt' config to add new ellipses. While you can add circles just by left-clicking on the screen. Circles are added with config pulled from the initial config file.
        - 'ellipse_params.txt' has multiple lines each having following space separated param values. 
        - [centerX, centerY, ellipse_A_radii, ellipse_B_radii, ellipse_alpha_in_degrees, velocity_X, velocity_Y, generation_of_face_it_its_a_face (else 0), 1 if its a face else 0 ]
        - I had used my above mention rough notes to understand the requirements and then coded a matlab symbolic expression that solves for the parametric equartion of one ellipse in the space normalized for another ellipse. 
        - The space normalizaiton is because, I am detecting intercestion by transforming one ellipse into a unit sized circle centered at orizin and then plotting the distance from orizin to points on the rim of another ellipse (which is transformed along with the first one)
        - This distance function is formulated and minimized in run time using Brent's algorithm. The normal and collision points are computed using parametric equations.
 


