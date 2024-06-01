# Introduction
This is my submission for an assignment in OpenGL for the course Computer Graphics.

# Features
On running this code, the screen will have an n sided prism; n being the input. User can press different keys to view different kinds of animation, either by affecting the shape or the camera.

## Keys and their effects
`T` : Toggle between n sided prism and n sided pyramid  
`R` : Toggle for rotating shape  
Arrow keys: Move shape in that direction  
`N` : Move shape nearer to screen  
`F` : Move shape further from the screen    
`A, D` : Move camera to left/right (rotates about the shape)  
`Q, E` : Move camera up/down (rotates about the shape)  
`W, S` : Move camera closer/away from shape  
`esc` : Close the window

# Running the code
After downloading the code:  
`cd opengl_assignment`  
`g++ main.cpp -o test glad.o -lGL -lglfw && ./test n`  
Replace n with the number of sides you want the shape to have