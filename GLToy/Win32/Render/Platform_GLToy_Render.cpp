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

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>
#include <gl/glu.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int Platform_GLToy_Render::s_uVersion = 0;
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
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // TODO: GLToy_String
    // get version
	const char* szVersionString = reinterpret_cast< const char* >( glGetString( GL_VERSION ) );
    if( !szVersionString )
    {
        return false;
    }

	s_uVersion = 0;

	// make the string into a nice number to compare stuff
    const u_int uVersionStringLength = strlen( szVersionString );
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
    s_pfnCreateShader       = reinterpret_cast< u_int ( __stdcall* )( u_int ) >(                            wglGetProcAddress( "glCreateShader" ) );
    s_pfnCreateProgram      = reinterpret_cast< u_int ( __stdcall* )() >(                                   wglGetProcAddress( "glCreateProgram" ) );
    s_pfnDeleteShader       = reinterpret_cast< void ( __stdcall* )( u_int ) >(                             wglGetProcAddress( "glDeleteShader" ) );
    s_pfnDeleteProgram      = reinterpret_cast< void ( __stdcall* )( u_int ) >(                             wglGetProcAddress( "glDeleteProgram" ) );
    s_pfnValidateProgram    = reinterpret_cast< void ( __stdcall* )( u_int ) >(                             wglGetProcAddress( "glValidateProgram" ) );
    s_pfnCompileShader      = reinterpret_cast< void ( __stdcall* )( u_int ) >(                             wglGetProcAddress( "glCompileShader" ) );
    s_pfnLinkProgram        = reinterpret_cast< void ( __stdcall* )( u_int ) >(                             wglGetProcAddress( "glLinkProgram" ) );
    s_pfnUseProgram         = reinterpret_cast< void ( __stdcall* )( u_int ) >(                             wglGetProcAddress( "glUseProgram" ) );
    s_pfnAttachShader       = reinterpret_cast< void ( __stdcall* )( u_int, u_int ) >(                      wglGetProcAddress( "glAttachShader" ) );
    s_pfnDetachShader       = reinterpret_cast< void ( __stdcall* )( u_int, u_int ) >(                      wglGetProcAddress( "glDetachShader" ) );
    s_pfnShaderSource       = reinterpret_cast< void ( __stdcall* )( u_int, int, char**, const int* ) >(    wglGetProcAddress( "glShaderSource" ) );
    s_pfnGetProgramInfoLog  = reinterpret_cast< void ( __stdcall* )( u_int, int, int*, char* ) >(           wglGetProcAddress( "glGetProgramInfoLog" ) );
    s_pfnGetShaderInfoLog   = reinterpret_cast< void ( __stdcall* )( u_int, int, int*, char* ) >(           wglGetProcAddress( "glGetShaderInfoLog" ) );
    s_pfnGetUniformID = reinterpret_cast< u_int ( __stdcall* )( u_int, const char* ) >(                     wglGetProcAddress( "glGetUniformLocation" ) );
    s_pfnGetAttributeID = reinterpret_cast< u_int ( __stdcall* )( u_int, const char* ) >(                   wglGetProcAddress( "glGetAttribLocation" ) );
    s_pfnBindAttributeID = reinterpret_cast< void ( __stdcall* )( u_int, u_int, const char* ) >(            wglGetProcAddress( "glBindAttribLocation" ) );
    s_pfnSetUniform1i = reinterpret_cast< void ( __stdcall* )( u_int, int ) >(                              wglGetProcAddress( "glUniform1i" ) );
    s_pfnSetUniform2i = reinterpret_cast< void ( __stdcall* )( u_int, int, int ) >(                         wglGetProcAddress( "glUniform2i" ) );
    s_pfnSetUniform3i = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int ) >(                    wglGetProcAddress( "glUniform3i" ) );
    s_pfnSetUniform4i = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int, int ) >(               wglGetProcAddress( "glUniform4i" ) );
    s_pfnSetUniform1f = reinterpret_cast< void ( __stdcall* )( u_int, float ) >(                            wglGetProcAddress( "glUniform1f" ) );
    s_pfnSetUniform2f = reinterpret_cast< void ( __stdcall* )( u_int, float, float ) >(                     wglGetProcAddress( "glUniform2f" ) );
    s_pfnSetUniform3f = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float ) >(              wglGetProcAddress( "glUniform3f" ) );
    s_pfnSetUniform4f = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float, float ) >(       wglGetProcAddress( "glUniform4f" ) );
    s_pfnSetAttribute1i = reinterpret_cast< void ( __stdcall* )( u_int, int ) >(                            wglGetProcAddress( "glVertexAttrib1i" ) );
    s_pfnSetAttribute2i = reinterpret_cast< void ( __stdcall* )( u_int, int, int ) >(                       wglGetProcAddress( "glVertexAttrib2i" ) );
    s_pfnSetAttribute3i = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int ) >(                  wglGetProcAddress( "glVertexAttrib3i" ) );
    s_pfnSetAttribute4i = reinterpret_cast< void ( __stdcall* )( u_int, int, int, int, int ) >(             wglGetProcAddress( "glVertexAttrib4i" ) );
    s_pfnSetAttribute1f = reinterpret_cast< void ( __stdcall* )( u_int, float ) >(                          wglGetProcAddress( "glVertexAttrib1f" ) );
    s_pfnSetAttribute2f = reinterpret_cast< void ( __stdcall* )( u_int, float, float ) >(                   wglGetProcAddress( "glVertexAttrib2f" ) );
    s_pfnSetAttribute3f = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float ) >(            wglGetProcAddress( "glVertexAttrib3f" ) );
    s_pfnSetAttribute4f = reinterpret_cast< void ( __stdcall* )( u_int, float, float, float, float ) >(     wglGetProcAddress( "glVertexAttrib4f" ) );

    return true;
}

void Platform_GLToy_Render::Shutdown()
{

}

void Platform_GLToy_Render::BeginRender()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void Platform_GLToy_Render::EndRender()
{
    GLToy::UpdateBuffers();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

void Platform_GLToy_Render::SetViewport( int iX, int iY, u_int uWidth, u_int uHeight )
{
    glViewport( iX, iY, uWidth, uHeight );
}

void Platform_GLToy_Render::SetIdentityProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
}

void Platform_GLToy_Render::SetPerspectiveProjectionMatrix( float fFOV, u_int uViewportWidth, u_int uViewportHeight )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fFOV, static_cast<float>( uViewportWidth ) / static_cast<float>( uViewportHeight ), 0.01f, 10000.0f );
}

void Platform_GLToy_Render::SetOrthogonalProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );
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
    glLoadIdentity();
    float aafMatrix[ 4 ][ 4 ];

    for( u_int u = 0; u < 3; ++u )
    for( u_int v = 0; v < 3; ++v )
    {
        aafMatrix[ u ][ v ] = xMatrix[ u ][ v ];
    }

    aafMatrix[ 0 ][ 3 ] = aafMatrix[ 1 ][ 3 ] = aafMatrix[ 2 ][ 3 ] = 0.0f;
    aafMatrix[ 3 ][ 0 ] = aafMatrix[ 3 ][ 1 ] = aafMatrix[ 3 ][ 2 ] = 0.0f;
    aafMatrix[ 3 ][ 3 ] = 1.0f;

    glMultMatrixf( aafMatrix[ 0 ] );
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

void Platform_GLToy_Render::SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit )
{
    // TODO - ignore texture unit for now...
    glTexCoord3fv( xTexCoord.GetFloatPointer() );
}

void Platform_GLToy_Render::Flush()
{
    glFlush();
}

u_int Platform_GLToy_Render::CreateShader( u_int uType )
{
    return s_pfnCreateShader( uType );
}

u_int Platform_GLToy_Render::CreateProgram()
{
    return s_pfnCreateProgram();
}

void Platform_GLToy_Render::DeleteShader( u_int uShaderID )
{
    s_pfnDeleteShader( uShaderID );
}

void Platform_GLToy_Render::DeleteProgram( u_int uProgramID )
{
    s_pfnDeleteProgram( uProgramID );
}

void Platform_GLToy_Render::ValidateProgram( u_int uProgramID )
{
    s_pfnValidateProgram( uProgramID );
}

void Platform_GLToy_Render::CompileShader( u_int uShaderID )
{
    s_pfnDeleteShader( uShaderID );
}

void Platform_GLToy_Render::LinkProgram( u_int uProgramID )
{
    s_pfnLinkProgram( uProgramID );
}

void Platform_GLToy_Render::UseProgram( u_int uProgramID )
{
    s_pfnUseProgram( uProgramID );
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
    return s_pfnGetUniformID( uProgramID,szName );
}

u_int Platform_GLToy_Render::GetAttributeID( u_int uProgramID, const char* szName )
{
    return s_pfnGetAttributeID( uProgramID,szName );
}

void Platform_GLToy_Render::BindAttributeID( u_int uProgramID, u_int uIndex, const char* szName )
{
    s_pfnBindAttributeID( uProgramID, uIndex, szName );
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2 )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3 )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2 )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3 )
{
}

void Platform_GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2 )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3 )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2 )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3 )
{
}

void Platform_GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 )
{
}