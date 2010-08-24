/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

// GLToy
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Vector.h>
#include <String/GLToy_String.h>

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>
#include <gl/glu.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int Platform_GLToy_Render::s_uVersion = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSwapInterval )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnActiveTexture )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnMultiTexCoord2fv )( u_int, const float* const ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnMultiTexCoord3fv )( u_int, const float* const ) = 0;
bool ( __stdcall* Platform_GLToy_Render::s_pfnIsRenderbuffer )( const u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnBindRenderbuffer )( const u_int, const u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnDeleteRenderbuffers )( const int, u_int* const ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGenRenderbuffers )( const int, u_int* const ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnRenderbufferStorage )( const u_int, const u_int, const int, const int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGetRenderbufferParameter )( const u_int, const u_int, int* const ) = 0;
bool ( __stdcall* Platform_GLToy_Render::s_pfnIsFramebuffer )( const u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnBindFramebuffer )( const u_int, const u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnDeleteFramebuffers )( const int, u_int* const ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGenFramebuffers )( const int, u_int* const ) = 0;
u_int ( __stdcall* Platform_GLToy_Render::s_pfnCheckFramebufferStatus )( const u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnFramebufferTexture1D )( const u_int, const u_int, const u_int, const u_int, const int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnFramebufferTexture2D )( const u_int, const u_int, const u_int, const u_int, const int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnFramebufferTexture3D )( const u_int, const u_int, const u_int, const u_int, const int, const int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnFramebufferRenderbuffer )( const u_int, const u_int, const u_int, const u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGetFramebufferAttachmentParameter )( const u_int, const u_int, const u_int, int* const ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGenerateMipmap )( const u_int ) = 0;
u_int ( __stdcall* Platform_GLToy_Render::s_pfnIsShader )( u_int ) = 0;
u_int ( __stdcall* Platform_GLToy_Render::s_pfnCreateShader )( u_int ) = 0;
u_int ( __stdcall* Platform_GLToy_Render::s_pfnCreateProgram )() = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnDeleteShader )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnDeleteProgram )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnValidateProgram )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnCompileShader )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnLinkProgram )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnUseProgram )( u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnAttachShader )( u_int, u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnDetachShader )( u_int, u_int ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnShaderSource )( u_int, int, char**, const int* ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGetProgramInfoLog )( u_int, int, int*, char* ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnGetShaderInfoLog )( u_int, int, int*, char* ) = 0;
u_int ( __stdcall* Platform_GLToy_Render::s_pfnGetUniformID )( u_int, const char* ) = 0;
u_int ( __stdcall* Platform_GLToy_Render::s_pfnGetAttributeID )( u_int, const char* ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnBindAttributeID )( u_int, u_int, const char* ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform1i )( u_int uUniformID, int iValue ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform2i )( u_int uUniformID, int iValue1, int iValue2 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform3i )( u_int uUniformID, int iValue1, int iValue2, int iValue3 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform4i )( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform1f )( u_int uUniformID, float fValue ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform2f )( u_int uUniformID, float fValue1, float fValue2 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform3f )( u_int uUniformID, float fValue1, float fValue2, float fValue3 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetUniform4f )( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute1i )( u_int uAttributeID, int iValue ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute2i )( u_int uAttributeID, int iValue1, int iValue2 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute3i )( u_int uAttributeID, int iValue1, int iValue2, int iValue3 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute4i )( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute1f )( u_int uAttributeID, float fValue ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute2f )( u_int uAttributeID, float fValue1, float fValue2 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute3f )( u_int uAttributeID, float fValue1, float fValue2, float fValue3 ) = 0;
void ( __stdcall* Platform_GLToy_Render::s_pfnSetAttribute4f )( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 ) = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_GLToy_Render::Initialise()
{
    GLToy_DebugOutput( "\r\nPlatform_GLToy_Render::Initialise() - Initialising Graphics\r\n" );

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glClearStencil( 0 );

    // get version
    GLToy_String szVersionString = reinterpret_cast< const char* >( glGetString( GL_VERSION ) );
    if( szVersionString.GetLength() == 0 )
    {
        return false;
    }

    s_uVersion = 0;

    // make the string into a nice number to compare stuff
    const u_int uVersionStringLength = szVersionString.GetLength();
    if( uVersionStringLength > 2 )
    {
        const char cMajor = static_cast< char > ( szVersionString[ 0 ] );
        const char cMinor = static_cast< char > ( szVersionString[ 2 ] );
        const char cRevision = ( uVersionStringLength > 4 ) ? static_cast< char > ( szVersionString[ 4 ] ) : '0';
        
        s_uVersion =    static_cast< u_int >( cMajor - '0' ) * 100
                        + static_cast< u_int >( cMinor - '0' ) * 10
                        + static_cast< u_int >( cRevision - '0' );
    }
    else
    {
        return false;
    }

    GLToy_DebugOutput( "\r\n  OpenGL %d.%d is available\r\n", s_uVersion / 100, s_uVersion % 100 );

    if( !s_uVersion )
    {
        return false;
    }

    // fill extensions - they should be set to null if unsupported...
    s_pfnSwapInterval                       = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "wglSwapIntervalEXT" ) );

    s_pfnActiveTexture                      = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glActiveTexture" ) );
    s_pfnMultiTexCoord2fv                   = reinterpret_cast< void ( __stdcall* )( u_int, const float* const ) >(                                                     wglGetProcAddress( "glMultiTexCoord2fv" ) );
    s_pfnMultiTexCoord3fv                   = reinterpret_cast< void ( __stdcall* )( u_int, const float* const ) >(                                                     wglGetProcAddress( "glMultiTexCoord3fv" ) );

    s_pfnIsRenderbuffer                     = reinterpret_cast< bool ( __stdcall* )( const u_int ) >(                                                                   wglGetProcAddress( "glIsRenderbufferEXT" ) );
    s_pfnBindRenderbuffer                   = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int ) >(                                                      wglGetProcAddress( "glBindRenderbufferEXT" ) );
    s_pfnDeleteRenderbuffers                = reinterpret_cast< void ( __stdcall* )( const int, u_int* const ) >(                                                       wglGetProcAddress( "glDeleteRenderbuffersEXT" ) );
    s_pfnGenRenderbuffers                   = reinterpret_cast< void ( __stdcall* )( const int, u_int* const ) >(                                                       wglGetProcAddress( "glGenRenderbuffersEXT" ) );
    s_pfnRenderbufferStorage                = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, const int, const int ) >(                                wglGetProcAddress( "glRenderbufferStorageEXT" ) );
    s_pfnGetRenderbufferParameter           = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, int* const ) >(                                          wglGetProcAddress( "glGetRenderbufferParameterivEXT" ) );
    s_pfnIsFramebuffer                      = reinterpret_cast< bool ( __stdcall* )( const u_int ) >(                                                                   wglGetProcAddress( "glIsFramebufferEXT" ) );
    s_pfnBindFramebuffer                    = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int ) >(                                                      wglGetProcAddress( "glBindFramebufferEXT" ) );
    s_pfnDeleteFramebuffers                 = reinterpret_cast< void ( __stdcall* )( const int, u_int* const ) >(                                                       wglGetProcAddress( "glDeleteFramebuffersEXT" ) );
    s_pfnGenFramebuffers                    = reinterpret_cast< void ( __stdcall* )( const int, u_int* const ) >(                                                       wglGetProcAddress( "glGenFramebuffersEXT" ) );
    s_pfnCheckFramebufferStatus             = reinterpret_cast< u_int ( __stdcall* )( const u_int ) >(                                                                  wglGetProcAddress( "glCheckFramebufferStatusEXT" ) );
    s_pfnFramebufferTexture1D               = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, const u_int, const u_int, const int ) >(                 wglGetProcAddress( "glFramebufferTexture1DEXT" ) );
    s_pfnFramebufferTexture2D               = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, const u_int, const u_int, const int ) >(                 wglGetProcAddress( "glFramebufferTexture2DEXT" ) );
    s_pfnFramebufferTexture3D               = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, const u_int, const u_int, const int, const int ) >(      wglGetProcAddress( "glFramebufferTexture3DEXT" ) );
    s_pfnFramebufferRenderbuffer            = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, const u_int, const u_int ) >(                            wglGetProcAddress( "glFramebufferRenderbufferEXT" ) );
    s_pfnGetFramebufferAttachmentParameter  = reinterpret_cast< void ( __stdcall* )( const u_int, const u_int, const u_int, int* const ) >(                             wglGetProcAddress( "glGetFramebufferAttachmentParameterivEXT" ) );
    s_pfnGenerateMipmap                     = reinterpret_cast< void ( __stdcall* )( const u_int ) >(                                                                   wglGetProcAddress( "glGenerateMipmapEXT" ) );

    s_pfnIsShader                           = reinterpret_cast< u_int ( __stdcall* )( u_int ) >(                                                                        wglGetProcAddress( "glIsShader" ) );
    s_pfnCreateShader                       = reinterpret_cast< u_int ( __stdcall* )( u_int ) >(                                                                        wglGetProcAddress( "glCreateShader" ) );
    s_pfnCreateProgram                      = reinterpret_cast< u_int ( __stdcall* )() >(                                                                               wglGetProcAddress( "glCreateProgram" ) );
    s_pfnDeleteShader                       = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glDeleteShader" ) );
    s_pfnDeleteProgram                      = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glDeleteProgram" ) );
    s_pfnValidateProgram                    = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glValidateProgram" ) );
    s_pfnCompileShader                      = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glCompileShader" ) );
    s_pfnLinkProgram                        = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glLinkProgram" ) );
    s_pfnUseProgram                         = reinterpret_cast< void ( __stdcall* )( u_int ) >(                                                                         wglGetProcAddress( "glUseProgram" ) );
    s_pfnAttachShader                       = reinterpret_cast< void ( __stdcall* )( u_int, u_int ) >(                                                                  wglGetProcAddress( "glAttachShader" ) );
    s_pfnDetachShader                       = reinterpret_cast< void ( __stdcall* )( u_int, u_int ) >(                                                                  wglGetProcAddress( "glDetachShader" ) );
    s_pfnShaderSource                       = reinterpret_cast< void ( __stdcall* )( u_int, int, char**, const int* ) >(                                                wglGetProcAddress( "glShaderSource" ) );
    s_pfnGetProgramInfoLog                  = reinterpret_cast< void ( __stdcall* )( u_int, int, int*, char* ) >(                                                       wglGetProcAddress( "glGetProgramInfoLog" ) );
    s_pfnGetShaderInfoLog                   = reinterpret_cast< void ( __stdcall* )( u_int, int, int*, char* ) >(                                                       wglGetProcAddress( "glGetShaderInfoLog" ) );
    s_pfnGetUniformID                       = reinterpret_cast< u_int ( __stdcall* )( u_int, const char* ) >(                                                           wglGetProcAddress( "glGetUniformLocation" ) );
    s_pfnGetAttributeID                     = reinterpret_cast< u_int ( __stdcall* )( u_int, const char* ) >(                                                           wglGetProcAddress( "glGetAttribLocation" ) );
    s_pfnBindAttributeID                    = reinterpret_cast< void ( __stdcall* )( u_int, u_int, const char* ) >(                                                     wglGetProcAddress( "glBindAttribLocation" ) );
    s_pfnSetUniform1i                       = reinterpret_cast< void ( __stdcall* )( u_int, int ) >(                                                                    wglGetProcAddress( "glUniform1i" ) );
    s_pfnSetUniform2i                       = reinterpret_cast< void ( __stdcall* )( u_int, int, int ) >(                                                               wglGetProcAddress( "glUniform2i" ) );
    s_pfnSetUniform3i                       = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int ) >(                                                          wglGetProcAddress( "glUniform3i" ) );
    s_pfnSetUniform4i                       = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int, int ) >(                                                     wglGetProcAddress( "glUniform4i" ) );
    s_pfnSetUniform1f                       = reinterpret_cast< void ( __stdcall* )( u_int, float ) >(                                                                  wglGetProcAddress( "glUniform1f" ) );
    s_pfnSetUniform2f                       = reinterpret_cast< void ( __stdcall* )( u_int, float, float ) >(                                                           wglGetProcAddress( "glUniform2f" ) );
    s_pfnSetUniform3f                       = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float ) >(                                                    wglGetProcAddress( "glUniform3f" ) );
    s_pfnSetUniform4f                       = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float, float ) >(                                             wglGetProcAddress( "glUniform4f" ) );
    s_pfnSetAttribute1i                     = reinterpret_cast< void ( __stdcall* )( u_int, int ) >(                                                                    wglGetProcAddress( "glVertexAttrib1i" ) );
    s_pfnSetAttribute2i                     = reinterpret_cast< void ( __stdcall* )( u_int, int, int ) >(                                                               wglGetProcAddress( "glVertexAttrib2i" ) );
    s_pfnSetAttribute3i                     = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int ) >(                                                          wglGetProcAddress( "glVertexAttrib3i" ) );
    s_pfnSetAttribute4i                     = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int, int ) >(                                                     wglGetProcAddress( "glVertexAttrib4i" ) );
    s_pfnSetAttribute1f                     = reinterpret_cast< void ( __stdcall* )( u_int, float ) >(                                                                  wglGetProcAddress( "glVertexAttrib1f" ) );
    s_pfnSetAttribute2f                     = reinterpret_cast< void ( __stdcall* )( u_int, float, float ) >(                                                           wglGetProcAddress( "glVertexAttrib2f" ) );
    s_pfnSetAttribute3f                     = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float ) >(                                                    wglGetProcAddress( "glVertexAttrib3f" ) );
    s_pfnSetAttribute4f                     = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float, float ) >(                                             wglGetProcAddress( "glVertexAttrib4f" ) );

    return true;
}

void Platform_GLToy_Render::Shutdown()
{
}

void Platform_GLToy_Render::BeginRender()
{
    glClear( ( GLToy_Render::GetClearFrame() ? GL_COLOR_BUFFER_BIT : 0 ) | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void Platform_GLToy_Render::EndRender()
{
    GLToy::UpdateBuffers();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

u_int Platform_GLToy_Render::GetError()
{
    return glGetError();
}

void Platform_GLToy_Render::Clear()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void Platform_GLToy_Render::ClearDepth( const float fDepth )
{
    glClearDepth( fDepth );
    glClear( GL_DEPTH_BUFFER_BIT );
}

void Platform_GLToy_Render::SetDepthFunction( const u_int uDepthFunction )
{
    glDepthFunc( uDepthFunction );
}

void Platform_GLToy_Render::SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight )
{
    glViewport( iX, iY, uWidth, uHeight );
}

void Platform_GLToy_Render::SetIdentityProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
}

void Platform_GLToy_Render::SetPerspectiveProjectionMatrix( const float fFOV, const float fAspectRatio )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fFOV, fAspectRatio, 0.01f, 1048576.0f );
}

void Platform_GLToy_Render::SetOrthogonalProjectionMatrix( const float fAspectRatio )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -fAspectRatio, fAspectRatio, -1.0f, 1.0f, -4.0f, 4.0f );
}

void Platform_GLToy_Render::SetIdentityViewMatrix()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void Platform_GLToy_Render::SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp )
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( xPosition[0], xPosition[1], xPosition[2],
                xLookAt[0], xLookAt[1], xLookAt[2],
                xUp[0], xUp[1], xUp[2] );
}

void Platform_GLToy_Render::StartSubmittingLines()
{
    glBegin( GL_LINES );
}

void Platform_GLToy_Render::StartSubmittingLineStrip()
{
    glBegin( GL_LINE_STRIP );
}

void Platform_GLToy_Render::StartSubmittingLineLoop()
{
    glBegin( GL_LINE_LOOP );
}

void Platform_GLToy_Render::StartSubmittingTriangles()
{
    glBegin( GL_TRIANGLES );
}

void Platform_GLToy_Render::StartSubmittingQuads()
{
    glBegin( GL_QUADS );
}

void Platform_GLToy_Render::StartSubmittingTriangleStrip()
{
    glBegin( GL_TRIANGLE_STRIP );
}

void Platform_GLToy_Render::StartSubmittingTriangleFan()
{
    glBegin( GL_TRIANGLE_FAN );
}

void Platform_GLToy_Render::StartSubmittingPolygon()
{
    glBegin( GL_POLYGON );
}

void Platform_GLToy_Render::EndSubmit()
{
    glEnd();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

void Platform_GLToy_Render::Translate( const GLToy_Vector_3& xTranslation )
{
    glTranslatef( xTranslation[0], xTranslation[1], xTranslation[2] );
}

void Platform_GLToy_Render::Rotate( const GLToy_Vector_3& xAxis, const float fAngle )
{
    glRotatef( fAngle, xAxis[0], xAxis[1], xAxis[2] );
}

void Platform_GLToy_Render::Transform( const GLToy_Matrix_3& xMatrix )
{
    float aafMatrix[ 4 ][ 4 ];

    for( u_int u = 0; u < 3; ++u )
    for( u_int v = 0; v < 3; ++v )
    {
        aafMatrix[ u ][ v ] = xMatrix[ v ][ u ];
    }

    aafMatrix[ 0 ][ 3 ] = aafMatrix[ 1 ][ 3 ] = aafMatrix[ 2 ][ 3 ] = 0.0f;
    aafMatrix[ 3 ][ 0 ] = aafMatrix[ 3 ][ 1 ] = aafMatrix[ 3 ][ 2 ] = 0.0f;
    aafMatrix[ 3 ][ 3 ] = 1.0f;

    glMultMatrixf( aafMatrix[ 0 ] );
}

void Platform_GLToy_Render::PushViewAttributes()
{
    glPushAttrib( GL_VIEWPORT_BIT );
}

void Platform_GLToy_Render::PopViewAttributes()
{
    glPopAttrib();
}

void Platform_GLToy_Render::PushViewMatrix()
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
}

void Platform_GLToy_Render::PopViewMatrix()
{
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
}

void Platform_GLToy_Render::SubmitVertex( const float fX, const float fY, const float fZ )
{
    glVertex3f( fX, fY, fZ );
}

void Platform_GLToy_Render::SubmitVertex( const GLToy_Vector_2& xVertex )
{
    glVertex2fv( xVertex.GetFloatPointer() );
}

void Platform_GLToy_Render::SubmitVertex( const GLToy_Vector_3& xVertex )
{
    glVertex3fv( xVertex.GetFloatPointer() );
}

void Platform_GLToy_Render::SubmitNormal( const GLToy_Vector_3& xNormal )
{
    glNormal3fv( xNormal.GetFloatPointer() );
}

void Platform_GLToy_Render::SubmitColour( const GLToy_Vector_3& xColour )
{
    glColor3fv( xColour.GetFloatPointer() );
}

void Platform_GLToy_Render::SubmitColour( const GLToy_Vector_4& xColour )
{
    glColor4fv( xColour.GetFloatPointer() );
}

void Platform_GLToy_Render::SubmitUV( const GLToy_Vector_2& xUV, const u_int uTextureUnit )
{
    if( uTextureUnit == 0 )
    {
        glTexCoord2fv( xUV.GetFloatPointer() );
    }
    else if( s_pfnMultiTexCoord2fv )
    {
        s_pfnMultiTexCoord2fv( TEXTURE0 + uTextureUnit, xUV.GetFloatPointer() );
    }
}

void Platform_GLToy_Render::SubmitUV( const GLToy_Vector_3& xUV, const u_int uTextureUnit )
{
    if( uTextureUnit == 0 )
    {
        glTexCoord3fv( xUV.GetFloatPointer() );
    }
    else if( s_pfnMultiTexCoord3fv )
    {
        s_pfnMultiTexCoord3fv( TEXTURE0 + uTextureUnit, xUV.GetFloatPointer() );
    }
}

void Platform_GLToy_Render::Flush()
{
    glFlush();
}

void Platform_GLToy_Render::DisableBlending()
{
    glDisable( GL_BLEND );
}

void Platform_GLToy_Render::EnableBlending()
{
    glEnable( GL_BLEND );
}

void Platform_GLToy_Render::SetBlendFunction( const u_int uSourceBlend, const u_int uDestinationBlend )
{
    glBlendFunc( uSourceBlend, uDestinationBlend );
}

void Platform_GLToy_Render::DisableDepthTesting()
{
    glDisable( GL_DEPTH_TEST );
}

void Platform_GLToy_Render::EnableDepthTesting()
{
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL);
}

void Platform_GLToy_Render::DisableDepthWrites()
{
    glDepthMask( GL_FALSE );
}

void Platform_GLToy_Render::EnableDepthWrites()
{
    glDepthMask( GL_TRUE );
}

void Platform_GLToy_Render::EnableBackFaceCulling()
{
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}

void Platform_GLToy_Render::DisableBackFaceCulling()
{
    glDisable( GL_CULL_FACE );
}

void Platform_GLToy_Render::SetCCWFaceWinding()
{
    glFrontFace( GL_CCW );
}

void Platform_GLToy_Render::SetCWFaceWinding()
{
    glFrontFace( GL_CW );
}

void Platform_GLToy_Render::SetVsyncEnabled( const bool bEnabled )
{
    if( s_pfnSwapInterval )
    {
        s_pfnSwapInterval( bEnabled ? 1 : 0 );
    }

    glDrawBuffer( bEnabled ? GL_BACK : GL_FRONT );
}

void Platform_GLToy_Render::ActiveTexture( const u_int uTextureUnit )
{
    if( s_pfnActiveTexture )
    {
        s_pfnActiveTexture( uTextureUnit );
    }
}

bool Platform_GLToy_Render::IsRenderbuffer( const u_int uRenderBuffer )
{
    if( s_pfnIsRenderbuffer )
    {
        return s_pfnIsRenderbuffer( uRenderBuffer );
    }

    return false;
}

void Platform_GLToy_Render::BindRenderbuffer( const u_int uTarget, const u_int uRenderBuffer )
{
    if( s_pfnBindRenderbuffer )
    {
        s_pfnBindRenderbuffer( uTarget, uRenderBuffer ); 
    }
}

void Platform_GLToy_Render::DeleteRenderbuffers( const int iCount, u_int* const puRenderBuffers )
{
    if( s_pfnDeleteRenderbuffers )
    {
        s_pfnDeleteRenderbuffers( iCount, puRenderBuffers );
    }
}

void Platform_GLToy_Render::GenRenderbuffers( const int iCount, u_int* const puRenderBuffers )
{
    if( s_pfnGenRenderbuffers )
    {
        s_pfnGenRenderbuffers( iCount, puRenderBuffers ); 
    }
}

void Platform_GLToy_Render::RenderbufferStorage( const u_int uTarget, const u_int uInternalFormat, const int iWidth, const int iHeight )
{
    if( s_pfnRenderbufferStorage )
    {
        s_pfnRenderbufferStorage( uTarget, uInternalFormat, iWidth, iHeight );
    }
}

void Platform_GLToy_Render::GetRenderbufferParameter( const u_int uTarget, const u_int uPName, int* const piParams )
{
    if( s_pfnGetRenderbufferParameter )
    {
        s_pfnGetRenderbufferParameter( uTarget, uPName, piParams );
    }
}

bool Platform_GLToy_Render::IsFramebuffer( const u_int uFrameBuffer )
{
    if( s_pfnIsFramebuffer )
    {
        return s_pfnIsFramebuffer( uFrameBuffer );
    }

    return false;
}

void Platform_GLToy_Render::BindFramebuffer( const u_int uTarget, const u_int uFrameBuffer )
{
    if( s_pfnBindFramebuffer )
    {
        s_pfnBindFramebuffer( uTarget, uFrameBuffer );
    }
}

void Platform_GLToy_Render::DeleteFramebuffers( const int iCount, u_int* const puFrameBuffers )
{
    if( s_pfnDeleteFramebuffers )
    {
        s_pfnDeleteFramebuffers( iCount, puFrameBuffers );
    }
}

void Platform_GLToy_Render::GenFramebuffers( const int iCount, u_int* const puFrameBuffers )
{
    if( s_pfnGenFramebuffers )
    {
        s_pfnGenFramebuffers( iCount, puFrameBuffers );
    }
}

u_int Platform_GLToy_Render::CheckFramebufferStatus( const u_int uTarget )
{
    if( s_pfnCheckFramebufferStatus )
    {
        return s_pfnCheckFramebufferStatus( uTarget );
    }

    return 0;
}

void Platform_GLToy_Render::FramebufferTexture1D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel )
{
    if( s_pfnFramebufferTexture1D )
    {
        s_pfnFramebufferTexture1D( uTarget, uAttachment, uTexTarget, uTexture, iLevel );
    }
}

void Platform_GLToy_Render::FramebufferTexture2D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel )
{
    if( s_pfnFramebufferTexture2D )
    {
        s_pfnFramebufferTexture2D( uTarget, uAttachment, uTexTarget, uTexture, iLevel );
    }
}

void Platform_GLToy_Render::FramebufferTexture3D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel, const int iZOffset )
{
    if( s_pfnFramebufferTexture3D )
    {
        s_pfnFramebufferTexture3D( uTarget, uAttachment, uTexTarget, uTexture, iLevel, iZOffset );
    }
}

void Platform_GLToy_Render::FramebufferRenderbuffer( const u_int uTarget, const u_int uAttachment, const u_int uRenderBufferTarget, const u_int uRenderBuffer )
{
    if( s_pfnFramebufferRenderbuffer )
    {
        s_pfnFramebufferRenderbuffer( uTarget, uAttachment, uRenderBufferTarget, uRenderBuffer );
    }
}

void Platform_GLToy_Render::GetFramebufferAttachmentParameter( const u_int uTarget, const u_int uAttachment, const u_int uPName, int* const piParams )
{
    if( s_pfnGetFramebufferAttachmentParameter )
    {
        s_pfnGetFramebufferAttachmentParameter( uTarget, uAttachment, uPName, piParams );
    }
}

void Platform_GLToy_Render::GenerateMipmap( const u_int uTarget )
{
    if( s_pfnGenerateMipmap )
    {
        s_pfnGenerateMipmap( uTarget );
    }
}

bool Platform_GLToy_Render::IsShader( const u_int uID )
{
    if( s_pfnIsShader )
    {
        return s_pfnIsShader( uID ) == GL_TRUE;
    }
    
    return false;
}

u_int Platform_GLToy_Render::CreateFragmentShader()
{
    if( s_pfnCreateShader )
    {
        return s_pfnCreateShader( GL_FRAGMENT_SHADER );
    }
    
    return 0;
}

u_int Platform_GLToy_Render::CreateVertexShader()
{
    if( s_pfnCreateShader )
    {
        return s_pfnCreateShader( GL_VERTEX_SHADER );
    }
    
    return 0;
}

u_int Platform_GLToy_Render::CreateProgram()
{
    if( s_pfnCreateProgram )
    {
        return s_pfnCreateProgram();
    }
    
    return 0;
}

void Platform_GLToy_Render::DeleteShader( u_int uShaderID )
{
    if( s_pfnDeleteShader )
    {
        s_pfnDeleteShader( uShaderID );
    }
}

void Platform_GLToy_Render::DeleteProgram( u_int uProgramID )
{
    if( s_pfnDeleteProgram )
    {
        s_pfnDeleteProgram( uProgramID );
    }
}

void Platform_GLToy_Render::ValidateProgram( u_int uProgramID )
{
    if( s_pfnValidateProgram )
    {
        s_pfnValidateProgram( uProgramID );
    }
}

void Platform_GLToy_Render::CompileShader( u_int uShaderID )
{
    if( s_pfnCompileShader )
    {
        s_pfnCompileShader( uShaderID );
    }
}

void Platform_GLToy_Render::LinkProgram( u_int uProgramID )
{
    if( s_pfnLinkProgram )
    {
        s_pfnLinkProgram( uProgramID );
    }
}

void Platform_GLToy_Render::UseProgram( u_int uProgramID )
{
    if( s_pfnUseProgram )
    {
        s_pfnUseProgram( uProgramID );
    }
}

void Platform_GLToy_Render::AttachShader( u_int uProgramID, u_int uShaderID )
{
    s_pfnAttachShader( uProgramID, uShaderID );
}

void Platform_GLToy_Render::DetachShader( u_int uProgramID, u_int uShaderID )
{
    s_pfnDetachShader( uProgramID, uShaderID );
}

void Platform_GLToy_Render::ShaderSource( u_int uShaderID, int iStringCount, char** ppszStrings, const int* piLengths )
{
    s_pfnShaderSource( uShaderID, iStringCount, ppszStrings, piLengths );
}

void Platform_GLToy_Render::GetProgramInfoLog( u_int uProgramID, int iMaxLength, int* piLength, char* szInfoLog )
{
    s_pfnGetProgramInfoLog( uProgramID, iMaxLength, piLength, szInfoLog );
}

void Platform_GLToy_Render::GetShaderInfoLog( u_int uShaderID, int iMaxLength, int* piLength, char* szInfoLog )
{
    s_pfnGetShaderInfoLog( uShaderID, iMaxLength, piLength, szInfoLog );
}

u_int Platform_GLToy_Render::GetUniformID( u_int uProgramID, const char* szName )
{
    return s_pfnGetUniformID( uProgramID, szName );
}

u_int Platform_GLToy_Render::GetAttributeID( u_int uProgramID, const char* szName )
{
    return s_pfnGetAttributeID( uProgramID, szName );
}

void Platform_GLToy_Render::BindAttributeID( u_int uProgramID, u_int uIndex, const char* szName )
{
    s_pfnBindAttributeID( uProgramID, uIndex, szName );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue )
{
    s_pfnSetUniform1i( uUniformID, iValue );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2 )
{
    s_pfnSetUniform2i( uUniformID, iValue1, iValue2 );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3 )
{
    s_pfnSetUniform3i( uUniformID, iValue1, iValue2, iValue3 );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 )
{
    s_pfnSetUniform4i( uUniformID, iValue1, iValue2, iValue3, iValue4 );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue )
{
    s_pfnSetUniform1f( uUniformID, fValue );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2 )
{
    s_pfnSetUniform2f( uUniformID, fValue1, fValue2 );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3 )
{
    s_pfnSetUniform3f( uUniformID, fValue1, fValue2, fValue3 );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 )
{
    s_pfnSetUniform4f( uUniformID, fValue1, fValue2, fValue3, fValue4 );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue )
{
    s_pfnSetAttribute1i( uAttributeID, iValue );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2 )
{
    s_pfnSetAttribute2i( uAttributeID, iValue1, iValue2 );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3 )
{
    s_pfnSetAttribute3i( uAttributeID, iValue1, iValue2, iValue3 );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 )
{
    s_pfnSetAttribute4i( uAttributeID, iValue1, iValue2, iValue3, iValue4 );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue )
{
    s_pfnSetAttribute1f( uAttributeID, fValue );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2 )
{
    s_pfnSetAttribute2f( uAttributeID, fValue1, fValue2 );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3 )
{
    s_pfnSetAttribute3f( uAttributeID, fValue1, fValue2, fValue3 );
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 )
{
    s_pfnSetAttribute4f( uAttributeID, fValue1, fValue2, fValue3, fValue4 );
}
