# Hello-Fragment-Shader
Boilerplate for drawing an OpenGL 2D* fragment shader in Windows
* *no geometry apart from two triangles that fill the window area*

# Lots of code stolen from :

Tiny C Compiler (TCC) , it's in HFS.zip  

TCC is [for now](http://repo.or.cz/tinycc.git/blob/1a4d4b76e803a32db1168e66ad8f8b26c29c8ea0:/RELICENSING) licensed under [GPL 2.1](http://www.gnu.org/licenses/lgpl-2.1.html)


[OpenGL in a PROPER Windows APP (NO GLUT)](
https://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/)
opens a window , but no shader  


[Hello Triangle OpenGL 4 Up and Running](  
http://antongerdelan.net/opengl/hellotriangle.html)  
loads shader but uses libraries (GLFW and GLEW)

# How to run
## Tiny C Compiler (TCC)
### If you don't have it set up
Just download HFS.zip which includes TCC (yes)
unzip  
run COMPILE AND RUN.bat  
ignore everything else in this repository

### If you already have [or want to](http://repo.or.cz/tinycc.git/commit/d348a9a51d32cece842b7885d27a411436d7887b) set it up yourself  
ignore the zip, download everything else  
in 'COMPILE AND RUN.bat' change ~~gcc~~ -> tcc , then run it  

## MinGW / Mingw-w64 
Assuming you already have it setup  
ignore the zip download everything else  
run 'COMPILE AND RUN.bat  

btw the .bat file is nothing fancy it's just  

<code>
gcc boilerplate.c -o boilerplate.exe -mwindows -lopengl32
boilerplate.exe
pause
</code>
