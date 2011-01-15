/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009-2011 Semi Essessi
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

#ifndef __GLTOY_RENDER_H_
#define __GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define BLEND_ZERO                                          0
#define BLEND_ONE                                           1
#define BLEND_SRC_COLOR                                     0x0300
#define BLEND_ONE_MINUS_SRC_COLOR                           0x0301
#define BLEND_SRC_ALPHA                                     0x0302
#define BLEND_ONE_MINUS_SRC_ALPHA                           0x0303
#define BLEND_DST_ALPHA                                     0x0304
#define BLEND_ONE_MINUS_DST_ALPHA                           0x0305

#define DEPTH_NEVER                                         0x0200
#define DEPTH_LESS                                          0x0201
#define DEPTH_EQUAL                                         0x0202
#define DEPTH_LEQUAL                                        0x0203
#define DEPTH_GREATER                                       0x0204
#define DEPTH_NOTEQUAL                                      0x0205
#define DEPTH_GEQUAL                                        0x0206
#define DEPTH_ALWAYS                                        0x0207

#define TEXTURE_2D                                          0x0DE1

#define TEXTURE0                                            0x84C0

#define STENCIL_INDEX8                                      0x8D48
#define ALPHA4                                              0x803B
#define ALPHA8                                              0x803C
#define ALPHA12                                             0x803D
#define ALPHA16                                             0x803E
#define LUMINANCE4                                          0x803F
#define LUMINANCE8                                          0x8040
#define LUMINANCE12                                         0x8041
#define LUMINANCE16                                         0x8042
#define LUMINANCE4_ALPHA4                                   0x8043
#define LUMINANCE6_ALPHA2                                   0x8044
#define LUMINANCE8_ALPHA8                                   0x8045
#define LUMINANCE12_ALPHA4                                  0x8046
#define LUMINANCE12_ALPHA12                                 0x8047
#define LUMINANCE16_ALPHA16                                 0x8048
#define INTENSITY                                           0x8049
#define INTENSITY4                                          0x804A
#define INTENSITY8                                          0x804B
#define INTENSITY12                                         0x804C
#define INTENSITY16                                         0x804D
#define R3_G3_B2                                            0x2A10
#define RGB4                                                0x804F
#define RGB5                                                0x8050
#define RGB8                                                0x8051
#define RGB10                                               0x8052
#define RGB12                                               0x8053
#define RGB16                                               0x8054
#define RGBA2                                               0x8055
#define RGBA4                                               0x8056
#define RGB5_A1                                             0x8057
#define RGBA8                                               0x8058
#define RGB10_A2                                            0x8059
#define RGBA12                                              0x805A
#define RGBA16                                              0x805B

#define UNSIGNED_BYTE_3_3_2                                 0x8032
#define UNSIGNED_BYTE_2_3_3_REV                             0x8362
#define UNSIGNED_SHORT_4_4_4_4                              0x8033
#define UNSIGNED_SHORT_4_4_4_4_REV                          0x8365
#define UNSIGNED_SHORT_5_6_5                                0x8363
#define UNSIGNED_INT_8_8_8_8                                0x8035
#define UNSIGNED_INT_8_8_8_8_REV                            0x8367
#define UNSIGNED_SHORT_5_6_5_REV                            0x8364
#define UNSIGNED_INT_10_10_10_2                             0x8036
#define UNSIGNED_INT_2_10_10_10_REV                         0x8368
#define UNSIGNED_SHORT_5_5_5_1                              0x8034
#define UNSIGNED_SHORT_1_5_5_5_REV                          0x8366
#define UNSIGNED_SHORT_8_8                                  0x85BA
#define UNSIGNED_SHORT_8_8_REV                              0x85BB

#define DEPTH_COMPONENT16                                   0x81A5
#define DEPTH_COMPONENT24                                   0x81A6
#define DEPTH_COMPONENT32                                   0x81A7
#define TEXTURE_DEPTH_SIZE                                  0x884A
#define DEPTH_TEXTURE_MODE                                  0x884B

#define FRAMEBUFFER                                         0x8D40
#define RENDERBUFFER                                        0x8D41
#define STENCIL_INDEX1                                      0x8D46
#define STENCIL_INDEX4                                      0x8D47
#define STENCIL_INDEX8                                      0x8D48
#define STENCIL_INDEX16                                     0x8D49
#define RENDERBUFFER_WIDTH                                  0x8D42
#define RENDERBUFFER_HEIGHT                                 0x8D43
#define RENDERBUFFER_INTERNAL_FORMAT                        0x8D44
#define RENDERBUFFER_RED_SIZE                               0x8D50
#define RENDERBUFFER_GREEN_SIZE                             0x8D51
#define RENDERBUFFER_BLUE_SIZE                              0x8D52
#define RENDERBUFFER_ALPHA_SIZE                             0x8D53
#define RENDERBUFFER_DEPTH_SIZE                             0x8D54
#define RENDERBUFFER_STENCIL_SIZE                           0x8D55
#define FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE                  0x8CD0
#define FRAMEBUFFER_ATTACHMENT_OBJECT_NAME                  0x8CD1
#define FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL                0x8CD2
#define FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE        0x8CD3
#define FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET           0x8CD4
#define COLOR_ATTACHMENT0                                   0x8CE0
#define COLOR_ATTACHMENT1                                   0x8CE1
#define COLOR_ATTACHMENT2                                   0x8CE2
#define COLOR_ATTACHMENT3                                   0x8CE3
#define COLOR_ATTACHMENT4                                   0x8CE4
#define COLOR_ATTACHMENT5                                   0x8CE5
#define COLOR_ATTACHMENT6                                   0x8CE6
#define COLOR_ATTACHMENT7                                   0x8CE7
#define COLOR_ATTACHMENT8                                   0x8CE8
#define COLOR_ATTACHMENT9                                   0x8CE9
#define COLOR_ATTACHMENT10                                  0x8CEA
#define COLOR_ATTACHMENT11                                  0x8CEB
#define COLOR_ATTACHMENT12                                  0x8CEC
#define COLOR_ATTACHMENT13                                  0x8CED
#define COLOR_ATTACHMENT14                                  0x8CEE
#define COLOR_ATTACHMENT15                                  0x8CEF
#define DEPTH_ATTACHMENT                                    0x8D00
#define STENCIL_ATTACHMENT                                  0x8D20
#define FRAMEBUFFER_COMPLETE                                0x8CD5
#define FRAMEBUFFER_INCOMPLETE_ATTACHMENT                   0x8CD6
#define FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT           0x8CD7
#define FRAMEBUFFER_INCOMPLETE_DIMENSIONS                   0x8CD9
#define FRAMEBUFFER_INCOMPLETE_FORMATS                      0x8CDA
#define FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER                  0x8CDB
#define FRAMEBUFFER_INCOMPLETE_READ_BUFFER                  0x8CDC
#define FRAMEBUFFER_UNSUPPORTED                             0x8CDD
#define FRAMEBUFFER_BINDING                                 0x8CA6
#define RENDERBUFFER_BINDING                                0x8CA7
#define MAX_COLOR_ATTACHMENTS                               0x8CDF
#define MAX_RENDERBUFFER_SIZE                               0x84E8
#define INVALID_FRAMEBUFFER_OPERATION                       0x0506
#define FRAMEBUFFER_BINDING									0x8CA6
#define DRAW_FRAMEBUFFER_BINDING							FRAMEBUFFER_BINDING

#define TIME_ELAPSED                                        0x88BF
#define QUERY_RESULT                                        0x8866
#define QUERY_RESULT_AVAILABLE                              0x8867

#define TEXTURE_DEPTH_SIZE                                  0x884A
#define DEPTH_TEXTURE_MODE                                  0x884B

#define TEXTURE_COMPARE_MODE                                0x884C
#define TEXTURE_COMPARE_FUNC                                0x884D
#define COMPARE_R_TO_TEXTURE                                0x884E

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class DataType, class KeyType > class GLToy_BinaryTree;
template < class T > class GLToy_List;
class GLToy_Matrix_3;
class GLToy_Renderable;
template < class T > class GLToy_RenderFunctor;
template < class T > class GLToy_RenderDeferredFunctor;
template < class T > class GLToy_RenderTransparentFunctor;
class GLToy_Vector_2;
class GLToy_Vector_3;
class GLToy_Vector_4;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Render
{

public:

    static float GetFOV() { return s_fFOV; }
    static float GetAspectRatio() { return s_fAspectRatio; }
    static float GetMinX() { return -GetAspectRatio(); }
    static float GetMaxX() { return GetAspectRatio(); }
    static float Get2DWidth() { return 2.0f * GetAspectRatio(); }
    static void SetClearFrame( const bool bClear = true ) { s_bClearFrame = bClear; }
    static bool GetClearFrame() { return s_bClearFrame; }
    static bool HasFrameBuffer() { return s_uSwapBuffer != 0xFFFFFFFF; }
    static bool HasDeferredBuffer() { return s_uDeferredBuffer != 0xFFFFFFFF; }

    static bool Initialise();
    static void Shutdown();
    
    static void BeginRender();
    static void BeginRender2D();
    static void Render();
    static void Render2D();
    static void EndRender();

    static void RegisterDeferred( const GLToy_Renderable* const pxDeferred );
    static void RegisterTransparent( const GLToy_Renderable* const pxTransparent, const float fSquaredDistanceFromCamera );

    static void BindFrameBuffer( const u_int uTextureUnit = 0 );
    static void BindFrameBufferNoCopy( const u_int uTextureUnit = 0 );
    static void BindLastFrameBufferTexture( const u_int uTextureUnit = 0 );

    static void BindDiffuseTexture( const u_int uTextureUnit = 0 );
    static void BindNormalTexture( const u_int uTextureUnit = 0 );
    static void BindDepthTexture( const u_int uTextureUnit = 0 );

    // GL interface
    static u_int GetError();

    static void Clear();
    static void ClearColour( const GLToy_Vector_4& xColour );
    static void ClearDepth( const float fDepth );
    static void SetDepthFunction( const u_int uDepthFunction );

    static void SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight );

    static void SetIdentityProjectionMatrix();
    static void SetPerspectiveProjectionMatrix();
    static void SetOrthogonalProjectionMatrix();

    static void SetIdentityViewMatrix();
    static void SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp );

    static void Translate( const GLToy_Vector_3& xTranslation );
    static void Rotate( const GLToy_Vector_3& xAxis, const float fAngle );
    static void Transform( const GLToy_Matrix_3& xMatrix );

    static void PushViewAttributes();
    static void PopViewAttributes();
    static void PushViewMatrix();
    static void PopViewMatrix();

    static void SubmitTexturedQuad2D( const float fXMin, const float fYMin, const float fXMax, const float fYMax, const float fUMin = 0.0f, const float fVMin = 0.0f, const float fUMax = 1.0f, const float fVMax = 1.0f );
    static void SubmitTexturedQuad2D( const GLToy_Vector_2& xPosition, const GLToy_Vector_2& xSize, const float fUMin = 0.0f, const float fVMin = 0.0f, const float fUMax = 1.0f, const float fVMax = 1.0f );

    static void StartSubmittingLines();
    static void StartSubmittingLineStrip();
    static void StartSubmittingLineLoop();
    static void StartSubmittingTriangles();
    static void StartSubmittingQuads();
    static void StartSubmittingTriangleStrip();
    static void StartSubmittingTriangleFan();
    static void StartSubmittingPolygon();
    static void EndSubmit();

    static void SubmitVertex( const float fX, const float fY, const float fZ );
    static void SubmitVertex( const GLToy_Vector_2& xVertex );
    static void SubmitVertex( const GLToy_Vector_3& xVertex );
    static void SubmitNormal( const GLToy_Vector_3& xNormal );
    static void SubmitColour( const GLToy_Vector_3& xColour );
    static void SubmitColour( const GLToy_Vector_4& xColour );
    static void SubmitUV( const GLToy_Vector_2& xUV, const u_int uTextureUnit = 0 );
    static void SubmitUV( const GLToy_Vector_3& xUV, const u_int uTextureUnit = 0 );
    static void SubmitUV( const GLToy_Vector_4& xUV, const u_int uTextureUnit = 0 );

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

    static void SetVsyncEnabled( const bool bEnabled );
    static bool IsVSyncEnabled() { return s_bVsync; }

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

    static bool IsIntelGraphicsCard();

private:

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Render();

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_BeginRender();
    static void Platform_EndRender();

    static float s_fFOV;
    static float s_fAspectRatio;
    static bool s_bDrawFPS;
    static bool s_bDrawTimers;
    static bool s_bDrawBuffers;
    static bool s_bVsync;
    static bool s_bClearFrame;

    // frame buffer + depth buffer
    //static u_int s_uDepthBuffer;
    static u_int s_uDepthTexture;
    static u_int s_uFrameBuffer;
    static u_int s_uFrameTexture;
    static u_int s_uSwapBuffer;
    static u_int s_uSwapTexture;
    static u_int* s_puCurrentBuffer;
    static u_int* s_puCurrentTexture;

    // deferred buffers
    static u_int s_uDeferredBuffer;
    static u_int s_uDiffuseTexture;
    static u_int s_uNormalTexture;
    //static u_int s_uSpecularTexture;

    static GLToy_List< const GLToy_Renderable* > s_xDeferredRenderables;
    static GLToy_BinaryTree< const GLToy_Renderable*, float > s_xTransparents;

};

#endif
