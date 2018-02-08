#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/gl.h>
#include "klister.c"

/* Function prototypes */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int _open_osfhandle(long long int,int);/* <- Works without this but gives warning */
void draw(),setup();

#define false 0
#define true 1 /* keep in mind true is actually anything but 0 , not just 1 */

unsigned long long int 
otsc,
tscrate,
tscmillirate,
frameCount = 0;

double time = 0;

unsigned long long int 
TSC(){
	unsigned long long int ret = 0;
  asm(
    "rdtsc\n"
    "shlq $32,%%rdx\n"
    "orq %%rax,%%rdx\n"
    "movq %%rdx,%0\n"
    :
    "=r"(ret)
    :
    : "rax","rdx"
    );
	return ret;
}

unsigned long long int 
millis(){ return (TSC()-otsc) / tscmillirate; }
double
seconds(){ return (double)(TSC()-otsc) / tscrate; }

/* defaults to 500x500 if size() function isn't used */
unsigned short
    width = 500,
    height = 500,
    frameRate = 60,
    frameMillis = 16;
int 
    mouseX,pmouseX,
    mouseY,pmouseY;

void size(unsigned short w, unsigned short h){
  width = w;
  height = h;
}
void framerate(unsigned short n){
  frameRate = n;
  frameMillis = 1000 / n;
}

unsigned int RENDERER;

char glready = false,
     draw_during_resize = false;
     /*window is black during resize if this is false, can be set to true, but may be slow*/

void SetStdOutToNewConsole(){
    /* found @ https://stackoverflow.com/a/3009145/4900546 */
    int hConHandle;
    long long int lStdHandle;
    FILE *fp;
    AllocConsole();
    lStdHandle = (long long int)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, 0x4000);
    fp = _fdopen(hConHandle, "w");
    *stdout = *fp;
    setvbuf(stdout, 0, _IONBF, 0);
}

int linecount = 0;
int *lineLengths;
GLchar **ret;
GLchar *buffer;
GLchar **loadStrings(GLchar *path){
  free(buffer);
  free(ret);
  free(lineLengths);
  linecount = 0;
  FILE *in = fopen(path, "r");
  fseek(in,0,SEEK_END);
  int len = ftell(in);
  fseek(in,0,SEEK_SET);
  buffer = malloc(len * sizeof(GLchar));
  ret = malloc(len * sizeof(GLchar**));
  lineLengths = malloc(len * sizeof(int*));
  fread(buffer, sizeof(GLchar), len, in);
  GLchar * end = &buffer[len];
  while (*end==0)end--;
  if(*end!='\n')*++end='\n';
  GLchar *i,*j;
  i = j = buffer;
  for(;i<=end;i++){
    if(*i=='\n'){
      lineLengths[linecount] = i-j + 1;
      ret[linecount++] = j;
      *i='\n';
      j=i+1;
    }
  }
  ret = realloc(ret,linecount * sizeof(GLchar**));
  lineLengths = realloc(lineLengths ,linecount * sizeof(int*));
  return ret;
}
GLuint loadShader(char *fragFilename,char *vertFilename){
  GLchar **vert,**frag;
  GLuint vs,fs,prog;
  vert = loadStrings(vertFilename);
  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs,linecount,(const GLchar**)vert,lineLengths);
  glCompileShader(vs);
  frag = loadStrings(fragFilename);
  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs,linecount,(const GLchar**)frag,lineLengths);
  glCompileShader(fs);
  prog = glCreateProgram();
  glAttachShader(prog, fs);
  glAttachShader(prog, vs);
  glLinkProgram(prog);
  return prog;
}

struct Globals{
    HINSTANCE hInstance;    // window app instance
    HWND hwnd;              // the window
    HDC   hdc;              // device context
    HGLRC hglrc;            // OpenGL rendering context
}g;

/* In a Windows app, the starting point is WinMain() */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow ){
    /* pre setup */
    SetStdOutToNewConsole();
    otsc = TSC();
    Sleep(1000);
    tscrate = TSC() - otsc;
    tscmillirate = tscrate/1000;
    g.hInstance = hInstance;
    WNDCLASS wc = {
      CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
      WndProc,
      0,
      0,
      hInstance,
      LoadIcon( NULL, IDI_ASTERISK),
      LoadCursor( NULL, IDC_ARROW ),
      (HBRUSH)GetStockObject( BLACK_BRUSH ),
      NULL,
      TEXT("hello fragment shader")
    };
    /* Register that class with the Windows O/S */
    RegisterClass(&wc);

    setup();

    /* post setup */

    int window_style = WS_OVERLAPPEDWINDOW;
    RECT wrect;
    SetRect( &wrect,50,50,50+width,50+height);
    AdjustWindowRect( &wrect, window_style, false );
    /*
    3 lines above unneccesary if width and height refer to size including border,
    or you know the size of border for the window_style
                          50, 50,
                          width + borderwidth, height + borderheight,
    */
    g.hwnd = CreateWindow(TEXT("hello fragment shader"),
                          TEXT("hello fragment shader"),
                          window_style,
                          wrect.left, wrect.top,
                          wrect.right - wrect.left, wrect.bottom - wrect.top,
                          NULL, NULL,
                          hInstance, NULL);

    if( !g.hwnd ) FatalAppExit( 0, TEXT("CreateWindow() failed!") );

    ShowWindow( g.hwnd, iCmdShow );
    g.hdc = GetDC( g.hwnd );
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );    // just its size
    pfd.nVersion = 1;   // always 1
    pfd.dwFlags = PFD_SUPPORT_OPENGL |  // OpenGL support - not DirectDraw
                  PFD_DOUBLEBUFFER   |  // double buffering support
                  PFD_DRAW_TO_WINDOW;   // draw to the app window, not to a bitmap image
    pfd.iPixelType = PFD_TYPE_RGBA ;    // red, green, blue, alpha for each pixel
    pfd.cColorBits = 24;                // 24 bit == 8 bits for red, 8 for green, 8 for blue.
                                        // This count of color bits EXCLUDES alpha.
    pfd.cDepthBits = 32;                // 32 bits to measure pixel depth.  That's accurate!
    /*
    note
    processing, ARGB 0xAARRGGBB
    windows , RGBA 0xAABBGGRR i think ?
    */
    
    if (!SetPixelFormat(g.hdc,ChoosePixelFormat(g.hdc,&pfd),&pfd))
    FatalAppExit( 0, TEXT("SetPixelFormat() failed!") );
    g.hglrc = wglCreateContext( g.hdc );
    wglMakeCurrent( g.hdc, g.hglrc );
    klistra();
    RENDERER = loadShader("./core/draw.frag","./core/draw.vert");
    glUseProgram(RENDERER);
    MSG msg;
    glready = true;

    uniform(vec2,resolution,width,height);
    while( true ){
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
            if( msg.message == WM_QUIT ) break;
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else {
          unsigned long long int frameTime=0, frameEnd=0 , frameStart = millis();
          uniform( FLOAT, time, seconds() );
          draw();
          pmouseX = mouseX;
          pmouseY = mouseY;
          glDrawArrays(5,0,4);
          SwapBuffers(g.hdc);
          frameCount++;
          frameEnd = millis();
          frameTime = frameEnd - frameStart;
          Sleep(frameMillis - frameTime); 
        }
    }
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( g.hglrc );
    ReleaseDC( g.hwnd, g.hdc );
    AnimateWindow( g.hwnd, 200, AW_HIDE | AW_BLEND );
    return msg.wParam;
}

LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ){
    switch( message ){
    case WM_CREATE: return 0;
    case WM_DESTROY:PostQuitMessage(0); return 0;
    case WM_SIZE:{
        width = LOWORD(lparam);
        height = HIWORD(lparam);
        if(glready){
        glViewport(0,0,width,height);
        uniform(vec2,resolution,width,height);
        if(draw_during_resize){
        glDrawArrays(5,0,4);
        SwapBuffers(g.hdc);
        }
        }
    } return 0;
    case WM_KEYDOWN:
        switch( wparam ){
        case VK_ESCAPE:PostQuitMessage(0);
        } return 0;
    case WM_MOUSEMOVE:{
        POINT mxy;
        GetCursorPos(&mxy);
        ScreenToClient(g.hwnd,&mxy);
        mouseX = mxy.x;
        mouseY = height-mxy.y;
        uniform(vec2,mouse,mouseX,mouseY);
    }
    }
    return DefWindowProc( hwnd, message, wparam, lparam );
}