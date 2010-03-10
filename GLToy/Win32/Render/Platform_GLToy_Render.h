#ifndef __PLATFORM_GLTOY_RENDER_H_
#define __PLATFORM_GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's headers
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_3;
class GLToy_Vector_3;

class Platform_GLToy_Render
{

    friend class GLToy_Render;

public:

    static bool Initialise();
    static void Shutdown();

    static void BeginRender();
    static void EndRender();

    // shader functions
    static u_int CreateShader( u_int uType );
    static u_int CreateProgram();
    static void DeleteShader( u_int uShaderID );
    static void DeleteProgram( u_int uProgramID );
    static void ValidateProgram( u_int uProgramID );
    static void CompileShader( u_int uShaderID );
    static void LinkProgram( u_int uProgramID );
    static void UseProgram( u_int uProgramID );
    static void AttachShader( u_int uProgramID, u_int uShaderID );
    static void DetachShader( u_int uProgramID, u_int uShaderID );
    static void ShaderSource( u_int uShaderID, int iStringCount, char** ppszStrings, const int* piLengths );
    static void GetProgramInfoLog( u_int uProgramID, int iMaxLength, int* iLength, char* szInfoLog );
    static void GetShaderInfoLog( u_int uShaderID, int iMaxLength, int* iLength, char* szInfoLog );
    static u_int GetUniformID( u_int uProgramID, const char* szName );
    static u_int GetAttributeID( u_int uProgramID, const char* szName );
    static void BindAttributeID( u_int uProgramID, u_int uIndex, const char* szName );
    static void SetUniform( u_int uUniformID, int iValue );
    static void SetUniform( u_int uUniformID, int iValue1, int iValue2 );
    static void SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3 );
    static void SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 );
    static void SetUniform( u_int uUniformID, float fValue );
    static void SetUniform( u_int uUniformID, float fValue1, float fValue2 );
    static void SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3 );
    static void SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 );
    static void SetAttribute( u_int uAttributeID, int iValue );
    static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2 );
    static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3 );
    static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 );
    static void SetAttribute( u_int uAttributeID, float fValue );
    static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2 );
    static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3 );
    static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 );

private:

    // GL interface
    static void SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight );

    static void SetIdentityProjectionMatrix();
    static void SetPerspectiveProjectionMatrix( const float fFOV, const u_int uViewportWidth, const u_int uViewportHeight );
    static void SetOrthogonalProjectionMatrix();

    static void SetIdentityViewMatrix();
    static void SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp );

    static void Translate( const GLToy_Vector_3& xTranslation );
    static void Rotate( const GLToy_Vector_3& xAxis, const float fAngle );
    static void Transform( const GLToy_Matrix_3& xMatrix );

    static void StartSubmittingTriangles();
    static void StartSubmittingQuads();
    static void StartSubmittingTriangleStrip();
    static void StartSubmittingTriangleFan();
    static void StartSubmittingPolygon();
    static void EndSubmit();

    static void Flush();

    static void SubmitVertex( const GLToy_Vector_3& xVertex );
    static void SubmitNormal( const GLToy_Vector_3& xNormal );
    static void SubmitColour( const GLToy_Vector_3& xColour );
    static void SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit );

    static u_int s_uVersion;

    static u_int ( __stdcall* s_pfnCreateShader )( u_int );
    static u_int ( __stdcall* s_pfnCreateProgram )();
    static void ( __stdcall* s_pfnDeleteShader )( u_int );
    static void ( __stdcall* s_pfnDeleteProgram )( u_int );
    static void ( __stdcall* s_pfnValidateProgram )( u_int );
    static void ( __stdcall* s_pfnCompileShader )( u_int );
    static void ( __stdcall* s_pfnLinkProgram )( u_int );
    static void ( __stdcall* s_pfnUseProgram )( u_int );
    static void ( __stdcall* s_pfnAttachShader )( u_int, u_int );
    static void ( __stdcall* s_pfnDetachShader )( u_int, u_int );
    static void ( __stdcall* s_pfnShaderSource )( u_int, int, char**, const int* );
    static void ( __stdcall* s_pfnGetProgramInfoLog )( u_int, int, int*, char* );
    static void ( __stdcall* s_pfnGetShaderInfoLog )( u_int, int, int*, char* );
    static u_int ( __stdcall* s_pfnGetUniformID )( u_int, const char* );
    static u_int ( __stdcall* s_pfnGetAttributeID )( u_int, const char* );
    static void ( __stdcall* s_pfnBindAttributeID )( u_int, u_int, const char* );
    static void ( __stdcall* s_pfnSetUniform1i )( u_int uUniformID, int iValue );
    static void ( __stdcall* s_pfnSetUniform2i )( u_int uUniformID, int iValue1, int iValue2 );
    static void ( __stdcall* s_pfnSetUniform3i )( u_int uUniformID, int iValue1, int iValue2, int iValue3 );
    static void ( __stdcall* s_pfnSetUniform4i )( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 );
    static void ( __stdcall* s_pfnSetUniform1f )( u_int uUniformID, float fValue );
    static void ( __stdcall* s_pfnSetUniform2f )( u_int uUniformID, float fValue1, float fValue2 );
    static void ( __stdcall* s_pfnSetUniform3f )( u_int uUniformID, float fValue1, float fValue2, float fValue3 );
    static void ( __stdcall* s_pfnSetUniform4f )( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 );
    static void ( __stdcall* s_pfnSetAttribute1i )( u_int uAttributeID, int iValue );
    static void ( __stdcall* s_pfnSetAttribute2i )( u_int uAttributeID, int iValue1, int iValue2 );
    static void ( __stdcall* s_pfnSetAttribute3i )( u_int uAttributeID, int iValue1, int iValue2, int iValue3 );
    static void ( __stdcall* s_pfnSetAttribute4i )( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 );
    static void ( __stdcall* s_pfnSetAttribute1f )( u_int uAttributeID, float fValue );
    static void ( __stdcall* s_pfnSetAttribute2f )( u_int uAttributeID, float fValue1, float fValue2 );
    static void ( __stdcall* s_pfnSetAttribute3f )( u_int uAttributeID, float fValue1, float fValue2, float fValue3 );
    static void ( __stdcall* s_pfnSetAttribute4f )( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 );
};

#endif