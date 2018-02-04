/*
crude tiny OpenGL function loader by Prince Polka
uses wglGetProcAddress()
only loads what's necassary for basic shader

Also defines macros for setting uniforms of shaders,
but expands to calling glGetUniformLocation() every time
the first time is necessary but subsequent calls are not

This is something I will try to address
*/

typedef char GLchar; // why isn't this in gl.h ? anyways redifining it should be fine as type isn't changed

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif

/*
works by default
(*glDrawArrays)(GLenum,GLint,GLsizei)
(*glViewport)(GLint x,GLint y,GLsizei,GLsizei)
*/

GLint
(*glGetUniformLocation)(GLuint, const GLchar*);

GLuint
(*glCreateShader)(GLenum),
(*glCreateProgram)(void);

void
(*glShaderSource)(GLuint,GLsizei,const GLchar **, const GLint *),
(*glCompileShader)(GLuint),
(*glAttachShader)(GLuint,GLuint),
(*glLinkProgram )(GLuint),
(*glUseProgram )(GLuint),

(*glUniform1f)(GLint,GLfloat),
(*glUniform2f)(GLint,GLfloat,GLfloat),
(*glUniform3f)(GLint,GLfloat,GLfloat,GLfloat),
(*glUniform4f)(GLint,GLfloat,GLfloat,GLfloat,GLfloat),

(*glUniform1i)(GLint,GLint),
(*glUniform2i)(GLint,GLint,GLint),
(*glUniform3i)(GLint,GLint,GLint,GLint),
(*glUniform4i)(GLint,GLint,GLint,GLint,GLint),

(*glUniform1ui)(GLint,GLuint),
(*glUniform2ui)(GLint,GLuint,GLuint),
(*glUniform3ui)(GLint,GLuint,GLuint,GLuint),
(*glUniform4ui)(GLint,GLuint,GLuint,GLuint,GLuint),

(*glUniform1fv)(GLint,GLsizei,const GLfloat *),
(*glUniform2fv)(GLint,GLsizei,const GLfloat *),
(*glUniform3fv)(GLint,GLsizei,const GLfloat *),
(*glUniform4fv)(GLint,GLsizei,const GLfloat *),

(*glUniform1iv)(	GLint ,GLsizei ,const GLint *),
(*glUniform2iv)(	GLint ,GLsizei ,const GLint *),
(*glUniform3iv)(	GLint ,GLsizei ,const GLint *),
(*glUniform4iv)(	GLint ,GLsizei ,const GLint *),

(*glUniform1uiv)(	GLint ,GLsizei ,const GLuint *),
(*glUniform2uiv)(	GLint ,GLsizei ,const GLuint *),
(*glUniform3uiv)(	GLint ,GLsizei ,const GLuint *),
(*glUniform4uiv)(	GLint ,GLsizei ,const GLuint *),

(*glUniformMatrix2fv)(GLint,GLsizei,GLboolean,const GLfloat *),
(*glUniformMatrix3fv)(GLint,GLsizei,GLboolean,const GLfloat *),
(*glUniformMatrix4fv)(GLint,GLsizei,GLboolean,const GLfloat *),

(*glUniformMatrix2x3fv)(GLint,GLsizei,GLboolean,const GLfloat *),
(*glUniformMatrix3x2fv)(GLint,GLsizei,GLboolean,const GLfloat *),
(*glUniformMatrix2x4fv)(GLint,GLsizei,GLboolean,const GLfloat *),

(*glUniformMatrix4x2fv)(GLint,GLsizei,GLboolean,const GLfloat *),
(*glUniformMatrix3x4fv)(GLint,GLsizei,GLboolean,const GLfloat *),
(*glUniformMatrix4x3fv)(GLint,GLsizei,GLboolean,const GLfloat *)
;

/* bad macros */
#define FLOAT 1f
#define vec2 2f
#define vec3 3f
#define vec4 4f
#define INT 1i
#define ivec2 2i
#define ivec3 3i
#define ivec4 4i
#define gluniform(type,uniform,args...)(glUniform##type(glGetUniformLocation(RENDERER,#uniform),args))
#define uniform(t,u,a...)gluniform(t,u,a)
#define uniformv(t,u,a...)uniform(t##v,u,a)
#define uniformarray(t,u,a...)uniformv(t,u,a)



#define KLISTRA(x)x=(void*)wglGetProcAddress(#x);
/* run klistra(); after gl context is created */
void klistra(){
KLISTRA(glGetUniformLocation)
KLISTRA(glCreateShader)
KLISTRA(glCreateProgram)
KLISTRA(glShaderSource)
KLISTRA(glCompileShader)
KLISTRA(glAttachShader)
KLISTRA(glLinkProgram)
KLISTRA(glUseProgram)

KLISTRA(glUniform1f)
KLISTRA(glUniform2f)
KLISTRA(glUniform3f)
KLISTRA(glUniform4f)

KLISTRA(glUniform1i)
KLISTRA(glUniform2i)
KLISTRA(glUniform3i)
KLISTRA(glUniform4i)

KLISTRA(glUniform1ui)
KLISTRA(glUniform2ui)
KLISTRA(glUniform3ui)
KLISTRA(glUniform4ui)

KLISTRA(glUniform1fv)
KLISTRA(glUniform2fv)
KLISTRA(glUniform3fv)
KLISTRA(glUniform4fv)

KLISTRA(glUniform1iv)
KLISTRA(glUniform2iv)
KLISTRA(glUniform3iv)
KLISTRA(glUniform4iv)

KLISTRA(glUniform1uiv)
KLISTRA(glUniform2uiv)
KLISTRA(glUniform3uiv)
KLISTRA(glUniform4uiv)

KLISTRA(glUniformMatrix2fv)
KLISTRA(glUniformMatrix3fv)
KLISTRA(glUniformMatrix4fv)
KLISTRA(glUniformMatrix2x3fv)
KLISTRA(glUniformMatrix3x2fv)
KLISTRA(glUniformMatrix2x4fv)

KLISTRA(glUniformMatrix4x2fv)
KLISTRA(glUniformMatrix3x4fv)
KLISTRA(glUniformMatrix4x3fv)
}