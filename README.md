# Hello-Fragment-Shader
Boilerplate for drawing an OpenGL fragment shader using MinGW only

No external libraries
No geometry, apart from two triangles that fills the window area

Compile with:
gcc boilerplate.c -o boilerplate.exe -mwindows -lopengl32
or run the MAKE.bat file
 
# Lots of code stolen from :

OpenGL in a PROPER Windows APP (NO GLUT)  
https://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/
opens a window , but no shader

# And some from:

Hello Triangle OpenGL 4 Up and Running  
http://antongerdelan.net/opengl/hellotriangle.html
loads shader but uses libraries (GLFW and GLEW)
