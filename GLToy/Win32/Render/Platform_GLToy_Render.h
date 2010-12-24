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

#ifndef __PLATFORM_GLTOY_RENDER_H_
#define __PLATFORM_GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's headers
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32" )

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Platform_GLToy_Render
{

    friend class GLToy_Render;
    friend class GLToy_Texture;
    friend class GLToy_Texture_System;

public:

    static bool Initialise();
    static void Shutdown();

    static void BeginRender();
    static void EndRender();

    static void SetVsyncEnabled( const bool bEnabled );

    static bool SupportFramebuffer() { return s_pfnIsFramebuffer != NULL; }
    static bool SupportShaders() { return s_pfnIsShader != NULL; }

    static void DrawBuffers( const int iCount, const u_int* const puBuffers );

    // framebuffer functions
    static bool IsRenderbuffer( const u_int uRenderBuffer );
    static void BindRenderbuffer( const u_int uTarget, const u_int uRenderBuffer );
    static void DeleteRenderbuffers( const int iCount, u_int* const puRenderBuffers );
    static void GenRenderbuffers( const int iCount, u_int* const puRenderBuffers );
    static void RenderbufferStorage( const u_int uTarget, const u_int uInternalFormat, const int iWidth, const int iHeight );
    static void GetRenderbufferParameter( const u_int uTarget, const u_int uPName, int* const piParams );
    static bool IsFramebuffer( const u_int uFrameBuffer );
    static void BindFramebuffer( const u_int uTarget, const u_int uFrameBuffer );
    static void DeleteFramebuffers( const int iCount, u_int* const puFrameBuffers );
    static void GenFramebuffers( const int iCount, u_int* const puFrameBuffers );
    static u_int CheckFramebufferStatus( const u_int uTarget );
    static void FramebufferTexture1D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel );
    static void FramebufferTexture2D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel );
    static void FramebufferTexture3D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel, const int iZOffset );
    static void FramebufferRenderbuffer( const u_int uTarget, const u_int uAttachment, const u_int uRenderBufferTarget, const u_int uRenderBuffer );
    static void GetFramebufferAttachmentParameter( const u_int uTarget, const u_int uAttachment, const u_int uPName, int* const piParams );
    static void GenerateMipmap( const u_int uTarget );

    // shader functions
    static bool IsShader( const u_int uID );
    static u_int CreateFragmentShader();
    static u_int CreateVertexShader();
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

    static void BeginQuery( u_int uTarget, u_int uID );
    static void EndQuery( u_int uTarget );
    static void GenerateQueries( u_int uCount, u_int* puIDs );
    static void DeleteQueries( u_int uCount, const u_int* puIDs );
    static void GetQueryObject( u_int uID, u_int uParameterName, int* piParameters );
    static void GetQueryObject( u_int uID, u_int uParameterName, unsigned long long* pullParameters );

    static bool IsIntelGraphicsCard() { return s_bIntel; }

private:

    // GL interface
    static u_int GetError();

    static void Clear();
    static void ClearColour( const GLToy_Vector_4& xColour );
    static void ClearDepth( const float fDepth );
    static void SetDepthFunction( const u_int uDepthFunction );

    static void SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight );

    static void SetIdentityProjectionMatrix();
    static void SetPerspectiveProjectionMatrix( const float fFOV, const float fAspectRatio );
    static void SetOrthogonalProjectionMatrix( const float fAspectRatio );

    static void SetIdentityViewMatrix();
    static void SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp );

    static void Translate( const GLToy_Vector_3& xTranslation );
    static void Rotate( const GLToy_Vector_3& xAxis, const float fAngle );
    static void Transform( const GLToy_Matrix_3& xMatrix );

    static void PushViewAttributes();
    static void PopViewAttributes();
    static void PushViewMatrix();
    static void PopViewMatrix();

    static void StartSubmittingLines();
    static void StartSubmittingLineStrip();
    static void StartSubmittingLineLoop();
    static void StartSubmittingTriangles();
    static void StartSubmittingQuads();
    static void StartSubmittingTriangleStrip();
    static void StartSubmittingTriangleFan();
    static void StartSubmittingPolygon();
    static void EndSubmit();

    static void Flush();

    static void DisableBlending();
    static void EnableBlending();
    static void SetBlendFunction( const u_int uSourceBlend, const u_int uDestinationBlend );

    static void DisableDepthTesting();
    static void EnableDepthTesting();
    static void DisableDepthWrites();
    static void EnableDepthWrites();

    static void DisableBackFaceCulling();
    static void EnableBackFaceCulling();
    static void SetCCWFaceWinding();
    static void SetCWFaceWinding();

    static void SubmitVertex( const float fX, const float fY, const float fZ );
    static void SubmitVertex( const GLToy_Vector_2& xVertex );
    static void SubmitVertex( const GLToy_Vector_3& xVertex );
    static void SubmitNormal( const GLToy_Vector_3& xNormal );
    static void SubmitColour( const GLToy_Vector_3& xColour );
    static void SubmitColour( const GLToy_Vector_4& xColour );
    static void SubmitUV( const GLToy_Vector_2& xUV, const u_int uTextureUnit );
    static void SubmitUV( const GLToy_Vector_3& xUV, const u_int uTextureUnit );

    static void ActiveTexture( const u_int uTextureUnit );

    static u_int s_uVersion;
	static bool s_bIntel;

    // wgl functions
    static void ( __stdcall* s_pfnSwapInterval )( u_int );
    // draw buffers
    static void ( __stdcall* s_pfnDrawBuffers )( const int iCount, const u_int* const puBuffers );
    // multitexture functions
    static void ( __stdcall* s_pfnActiveTexture )( u_int );
    static void ( __stdcall* s_pfnMultiTexCoord2fv )( u_int, const float* const );
    static void ( __stdcall* s_pfnMultiTexCoord3fv )( u_int, const float* const );
    // framebuffer functions
    static bool ( __stdcall* s_pfnIsRenderbuffer )( const u_int );
    static void ( __stdcall* s_pfnBindRenderbuffer )( const u_int, const u_int );
    static void ( __stdcall* s_pfnDeleteRenderbuffers )( const int, u_int* const );
    static void ( __stdcall* s_pfnGenRenderbuffers )( const int, u_int* const );
    static void ( __stdcall* s_pfnRenderbufferStorage )( const u_int, const u_int, const int, const int );
    static void ( __stdcall* s_pfnGetRenderbufferParameter )( const u_int, const u_int, int* const );
    static bool ( __stdcall* s_pfnIsFramebuffer )( const u_int );
    static void ( __stdcall* s_pfnBindFramebuffer )( const u_int, const u_int );
    static void ( __stdcall* s_pfnDeleteFramebuffers )( const int, u_int* const );
    static void ( __stdcall* s_pfnGenFramebuffers )( const int, u_int* const );
    static u_int ( __stdcall* s_pfnCheckFramebufferStatus )( const u_int );
    static void ( __stdcall* s_pfnFramebufferTexture1D )( const u_int, const u_int, const u_int, const u_int, const int );
    static void ( __stdcall* s_pfnFramebufferTexture2D )( const u_int, const u_int, const u_int, const u_int, const int );
    static void ( __stdcall* s_pfnFramebufferTexture3D )( const u_int, const u_int, const u_int, const u_int, const int, const int );
    static void ( __stdcall* s_pfnFramebufferRenderbuffer )( const u_int, const u_int, const u_int, const u_int );
    static void ( __stdcall* s_pfnGetFramebufferAttachmentParameter )( const u_int, const u_int, const u_int, int* const );
    static void ( __stdcall* s_pfnGenerateMipmap )( const u_int );
    // shader functions
    static u_int ( __stdcall* s_pfnIsShader )( u_int );
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
    // queries
    static void ( __stdcall* s_pfnBeginQuery )( u_int uTarget, u_int uID );
    static void ( __stdcall* s_pfnEndQuery )( u_int uTarget );
    static void ( __stdcall* s_pfnGenQueries )( u_int uCount, u_int* puIDs );
    static void ( __stdcall* s_pfnDeleteQueries )( u_int uCount, const u_int* puIDs );
    static void ( __stdcall* s_pfnGetQueryObjectiv )( u_int uID, u_int uParameterName, int* piParameters );
    static void ( __stdcall* s_pfnGetQueryObjectui64v )( u_int uID, u_int uParameterName, unsigned long long* pullParameters );

};

#endif
