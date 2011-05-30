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
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_BinaryTree.h>
#include <Core/Data Structures/GLToy_List.h>
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

// Platform
#include <Render/Platform_GLToy_Render.h>

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

#define BUFFER_SIZE                                         0x8764
#define BUFFER_USAGE                                        0x8765
#define QUERY_COUNTER_BITS                                  0x8864
#define CURRENT_QUERY                                       0x8865
#define QUERY_RESULT                                        0x8866
#define QUERY_RESULT_AVAILABLE                              0x8867
#define ARRAY_BUFFER                                        0x8892
#define ELEMENT_ARRAY_BUFFER                                0x8893
#define ARRAY_BUFFER_BINDING                                0x8894
#define ELEMENT_ARRAY_BUFFER_BINDING                        0x8895
#define VERTEX_ATTRIB_ARRAY_BUFFER_BINDING                  0x889F
#define READ_ONLY                                           0x88B8
#define WRITE_ONLY                                          0x88B9
#define READ_WRITE                                          0x88BA
#define BUFFER_ACCESS                                       0x88BB
#define BUFFER_MAPPED                                       0x88BC
#define BUFFER_MAP_POINTER                                  0x88BD
#define STREAM_DRAW                                         0x88E0
#define STREAM_READ                                         0x88E1
#define STREAM_COPY                                         0x88E2
#define STATIC_DRAW                                         0x88E4
#define STATIC_READ                                         0x88E5
#define STATIC_COPY                                         0x88E6
#define DYNAMIC_DRAW                                        0x88E8
#define DYNAMIC_READ                                        0x88E9
#define DYNAMIC_COPY                                        0x88EA
#define SAMPLES_PASSED                                      0x8914

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
class GLToy_Matrix_4;
class GLToy_Renderable;
template < class T > class GLToy_RenderFunctor;
template < class T > class GLToy_RenderDeferredFunctor;
template < class T > class GLToy_RenderTransparentFunctor;
class GLToy_Vector_2;
class GLToy_Vector_3;
class GLToy_Vector_4;

#ifdef GLTOY_DEBUG
#define GLToy_Render_DebugState( xStateName, xState ) GLToy_Render::DebugState.m_##xStateName = xState;
#else
#define GLToy_Render_DebugState( xStateName, xState )
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// SE - almost certainly there is a better way to do this

#define GLToy_EnableOpenGL11Support() static GLToy_Render_OpenGL11Support g_xEnableOpenGL11Support
#define GLToy_SupportsOpenGL11() ( GLToy_Render_OpenGL11Support::IsEnabled() )

class GLToy_Render_OpenGL11Support
{
public:

    GLToy_Render_OpenGL11Support() { s_bEnabled = true; }

    static bool IsEnabled() { return s_bEnabled; }

private:

    static bool s_bEnabled;
};

class GLToy_Render
{

    friend class Platform_GLToy_Render;

public:

    static float GetFOV() { return s_fFOV; }
    static float GetAspectRatio() { return s_fAspectRatio; }
    static float GetMinX() { return -GetAspectRatio(); }
    static float GetMaxX() { return GetAspectRatio(); }
    static float Get2DWidth() { return 2.0f * GetAspectRatio(); }
    static void SetClearFrame( const bool bClear = true ) { s_bClearFrame = bClear; }
    static bool GetClearFrame() { return s_bClearFrame; }
    static bool HasFrameBuffer() { return s_uSwapBuffer != GLToy_MaxUint; }
    static bool HasDeferredBuffer() { return s_uDeferredBuffer != GLToy_MaxUint; }

    static bool Initialise();
    static void Shutdown();
    
    static void BeginRender();
    static void BeginRender2D();
    static void Render();
    static void Render2D();
    static void EndRender();

    static void RegisterDeferred( const GLToy_Renderable* const pxDeferred );
    static void RegisterTransparent( const GLToy_Renderable* const pxTransparent, const float fSquaredDistanceFromCamera );

    static void TransparentPass();
    static void DeferredPass();

    static bool IsVSyncEnabled() { return s_bVsync; }

    static void BindFrameBuffer( const u_int uTextureUnit = 0 );
    static void BindFrameBufferNoCopy( const u_int uTextureUnit = 0 );

    GLToy_ForceInline static void BindLastFrameBufferTexture( const u_int uTextureUnit = 0 )
    {
        if( HasFrameBuffer() )
        {        
            if( s_puCurrentBuffer == &s_uFrameBuffer )
            {
                GLToy_Texture_System::BindFrameBufferTexture( s_uSwapTexture, uTextureUnit );
            }
            else
            {
                GLToy_Texture_System::BindFrameBufferTexture( s_uFrameTexture, uTextureUnit );
            }
        }
    }

    GLToy_ForceInline static void BindDiffuseTexture( const u_int uTextureUnit = 0 )
    {
        if( HasDeferredBuffer() )
        {        
            GLToy_Texture_System::BindFrameBufferTexture( s_uDiffuseTexture, uTextureUnit );
        }
    }

    GLToy_ForceInline static void BindNormalTexture( const u_int uTextureUnit = 0 )
    {
        if( HasDeferredBuffer() )
        {        
            GLToy_Texture_System::BindFrameBufferTexture( s_uNormalTexture, uTextureUnit );
        }
    }

    GLToy_ForceInline static void BindSpecularTexture( const u_int uTextureUnit = 0 )
    {
        if( HasDeferredBuffer() )
        {        
            GLToy_Texture_System::BindFrameBufferTexture( s_uSpecularTexture, uTextureUnit );
        }
    }

    GLToy_ForceInline static void BindDepthTexture( const u_int uTextureUnit = 0 )
    {
        if( HasFrameBuffer() )
        {        
            GLToy_Texture_System::BindFrameBufferTexture( s_uDepthTexture, uTextureUnit );
        }
    }

    GLToy_ForceInline static void StartSamplingDepth()
    {
        BindFramebuffer( FRAMEBUFFER, 0 );
        BindFramebuffer( FRAMEBUFFER, *s_puCurrentBufferNoDepth );
    }

    GLToy_ForceInline static void StopSamplingDepth()
    {
        BindFramebuffer( FRAMEBUFFER, *s_puCurrentBuffer );
    }

    GLToy_ForceInline static bool Platform_Initialise()
    {
        return Platform_GLToy_Render::Initialise();
    }

    GLToy_ForceInline static void Platform_Shutdown()
    {
        Platform_GLToy_Render::Shutdown();
    }

    GLToy_ForceInline static void Platform_BeginRender()
    {
        Platform_GLToy_Render::BeginRender();
    }

    GLToy_ForceInline static void Platform_EndRender()
    {
        Platform_GLToy_Render::EndRender();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    // G L   I N T E R F A C E
    /////////////////////////////////////////////////////////////////////////////////////////////

    GLToy_ForceInline static u_int GetError()
    {
        return Platform_GLToy_Render::GetError();
    }

    GLToy_ForceInline static void Clear()
    {
        Platform_GLToy_Render::Clear();
    }

    GLToy_ForceInline static void ClearColour( const GLToy_Vector_4& xColour )
    {
        Platform_GLToy_Render::ClearColour( xColour );
    }

    GLToy_ForceInline static void ClearDepth( const float fDepth )
    {
        Platform_GLToy_Render::ClearDepth( fDepth );
    }

    GLToy_ForceInline static void SetDepthFunction( const u_int uDepthFunction )
    {
        Platform_GLToy_Render::SetDepthFunction( uDepthFunction );
    }

    GLToy_ForceInline static void SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight )
    {
        s_fAspectRatio = static_cast< float >( uWidth ) / static_cast< float >( uHeight );
        Platform_GLToy_Render::SetViewport( iX, iY, uWidth, uHeight );
    }

    GLToy_ForceInline static void SetIdentityProjectionMatrix()
    {
        Platform_GLToy_Render::SetIdentityProjectionMatrix();
    }

    GLToy_ForceInline static void SetPerspectiveProjectionMatrix()
    {
        Platform_GLToy_Render::SetIdentityProjectionMatrix();
        Platform_GLToy_Render::SetPerspectiveProjectionMatrix( s_fFOV, s_fAspectRatio );
    }

    GLToy_ForceInline static void SetOrthogonalProjectionMatrix()
    {
        Platform_GLToy_Render::SetIdentityProjectionMatrix();
        Platform_GLToy_Render::SetOrthogonalProjectionMatrix( s_fAspectRatio );
    }

    GLToy_ForceInline static void SetIdentityViewMatrix()
    {
        Platform_GLToy_Render::SetIdentityViewMatrix();
    }

    GLToy_ForceInline static void SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp )
    {
        Platform_GLToy_Render::SetIdentityViewMatrix();
        Platform_GLToy_Render::SetLookAtViewMatrix( xPosition, xLookAt, xUp );
    }

    GLToy_ForceInline static void Translate( const GLToy_Vector_3& xTranslation )
    {
        Platform_GLToy_Render::Translate( xTranslation );
    }

    GLToy_ForceInline static void Rotate( const GLToy_Vector_3& xAxis, const float fAngle )
    {
        Platform_GLToy_Render::Rotate( xAxis, fAngle );
    }

    GLToy_ForceInline static void Transform( const GLToy_Matrix_3& xMatrix )
    {
        Platform_GLToy_Render::Transform( xMatrix );
    }

    GLToy_ForceInline static void PushViewAttributes()
    {
        Platform_GLToy_Render::PushViewAttributes();
    }

    GLToy_ForceInline static void PopViewAttributes()
    {
        Platform_GLToy_Render::PopViewAttributes();
    }

    GLToy_ForceInline static void PushViewMatrix()
    {
        Platform_GLToy_Render::PushViewMatrix();
    }

    GLToy_ForceInline static void PopViewMatrix()
    {
        Platform_GLToy_Render::PopViewMatrix();
    }

    GLToy_ForceInline static void SubmitTexturedQuad2D( const float fXMin, const float fYMin, const float fXMax, const float fYMax, const float fUMin = 0.0f, const float fVMin = 0.0f, const float fUMax = 1.0f, const float fVMax = 1.0f )
    {
        SubmitUV( GLToy_Vector_3( fUMin, fVMax, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( fXMin, fYMin, 0.0f ) );

        SubmitUV( GLToy_Vector_3( fUMax, fVMax, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( fXMax, fYMin, 0.0f ) );

        SubmitUV( GLToy_Vector_3( fUMax, fVMin, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( fXMax, fYMax, 0.0f ) );

        SubmitUV( GLToy_Vector_3( fUMin, fVMin, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( fXMin, fYMax, 0.0f ) );
    }

    GLToy_ForceInline static void SubmitTexturedQuad2D( const GLToy_Vector_2& xPosition, const GLToy_Vector_2& xSize, const float fUMin = 0.0f, const float fVMin = 0.0f, const float fUMax = 1.0f, const float fVMax = 1.0f )
    {
        SubmitUV( GLToy_Vector_3( fUMin, fVMax, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( xPosition[ 0 ], xPosition[ 1 ], 0.0f ) );

        SubmitUV( GLToy_Vector_3( fUMax, fVMax, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( xPosition[ 0 ] + xSize[ 0 ], xPosition[ 1 ], 0.0f ) );

        SubmitUV( GLToy_Vector_3( fUMax, fVMin, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( xPosition[ 0 ] + xSize[ 0 ], xPosition[ 1 ] + xSize[ 1 ], 0.0f ) );

        SubmitUV( GLToy_Vector_3( fUMin, fVMin, 0.0f ) );
        SubmitVertex( GLToy_Vector_3( xPosition[ 0 ], xPosition[ 1 ] + xSize[ 1 ], 0.0f ) );
    }

    GLToy_ForceInline static void SubmitDeferredVertex( const GLToy_Vector_3& xVertex, const GLToy_Vector_2& xUV, const GLToy_Vector_3& xNormal, const GLToy_Vector_3& xTangent )
    {
	    SubmitUV( xUV );
	    SubmitUV( GLToy_Vector_4( GLToy_Maths::CompressNormal( xNormal ), GLToy_Maths::CompressNormal( xTangent ) ), 1 );
	    SubmitVertex( xVertex ); 
    }

    GLToy_ForceInline static void SubmitDeferredVertex( const float x, const float y, const float z, const GLToy_Vector_2& xUV, const GLToy_Vector_3& xNormal, const GLToy_Vector_3& xTangent )
    {
	    SubmitUV( xUV );
	    SubmitUV( GLToy_Vector_4( GLToy_Maths::CompressNormal( xNormal ), GLToy_Maths::CompressNormal( xTangent ) ), 1 );
	    SubmitVertex( GLToy_Vector_3( x, y, z ) ); 
    }

    GLToy_ForceInline static void StartSubmittingLines()
    {
        Platform_GLToy_Render::StartSubmittingLines();
    }

    GLToy_ForceInline static void StartSubmittingLineStrip()
    {
        Platform_GLToy_Render::StartSubmittingLineStrip();
    }

    GLToy_ForceInline static void StartSubmittingLineLoop()
    {
        Platform_GLToy_Render::StartSubmittingLineLoop();
    }

    GLToy_ForceInline static void StartSubmittingTriangles()
    {
        Platform_GLToy_Render::StartSubmittingTriangles();
    }

    GLToy_ForceInline static void StartSubmittingQuads()
    {
        Platform_GLToy_Render::StartSubmittingQuads();
    }

    GLToy_ForceInline static void StartSubmittingTriangleStrip()
    {
        Platform_GLToy_Render::StartSubmittingTriangleStrip();
    }

    GLToy_ForceInline static void StartSubmittingTriangleFan()
    {
        Platform_GLToy_Render::StartSubmittingTriangleFan();
    }

    GLToy_ForceInline static void StartSubmittingPolygon()
    {
        Platform_GLToy_Render::StartSubmittingPolygon();
    }

    GLToy_ForceInline static void EndSubmit()
    {
        Platform_GLToy_Render::EndSubmit();
    }

    GLToy_ForceInline static void SubmitVertex( const GLToy_Vector_2& xVertex )
    {
        Platform_GLToy_Render::SubmitVertex( xVertex );
    }

    GLToy_ForceInline static void SubmitVertex( const GLToy_Vector_3& xVertex )
    {
        Platform_GLToy_Render::SubmitVertex( xVertex );
    }

    GLToy_ForceInline static void SubmitVertex( const float fX, const float fY, const float fZ = 0.0f )
    {
        Platform_GLToy_Render::SubmitVertex( fX, fY, fZ );
    }

    GLToy_ForceInline static void SubmitNormal( const GLToy_Vector_3& xNormal )
    {
        Platform_GLToy_Render::SubmitNormal( xNormal );
    }

    GLToy_ForceInline static void SubmitColour( const GLToy_Vector_3& xColour )
    {
        Platform_GLToy_Render::SubmitColour( xColour );
    }

    GLToy_ForceInline static void SubmitColour( const GLToy_Vector_4& xColour )
    {
        Platform_GLToy_Render::SubmitColour( xColour );
    }

    GLToy_ForceInline static void SubmitUV( const GLToy_Vector_2& xUV, const u_int uTextureUnit = 0 )
    {
        Platform_GLToy_Render::SubmitUV( xUV, uTextureUnit );
    }

    GLToy_ForceInline static void SubmitUV( const GLToy_Vector_3& xUV, const u_int uTextureUnit = 0 )
    {
        Platform_GLToy_Render::SubmitUV( xUV, uTextureUnit );
    }

    GLToy_ForceInline static void SubmitUV( const GLToy_Vector_4& xUV, const u_int uTextureUnit = 0 )
    {
        Platform_GLToy_Render::SubmitUV( xUV, uTextureUnit );
    }

    GLToy_ForceInline static void Flush()
    {
        Platform_GLToy_Render::Flush();
    }

    GLToy_ForceInline static void DisableBlending()
    {
        Platform_GLToy_Render::DisableBlending();
        SetBlendFunction( BLEND_ONE, BLEND_ZERO );
    }

    GLToy_ForceInline static void EnableBlending()
    {
        Platform_GLToy_Render::EnableBlending();
    }

    GLToy_ForceInline static void DisableDepthTesting()
    {
        Platform_GLToy_Render::DisableDepthTesting();
    }

    GLToy_ForceInline static void EnableDepthTesting()
    {
        Platform_GLToy_Render::EnableDepthTesting();
    }

    GLToy_ForceInline static void DisableDepthWrites()
    {
        Platform_GLToy_Render::DisableDepthWrites();
    }

    GLToy_ForceInline static void EnableDepthWrites()
    {
        Platform_GLToy_Render::EnableDepthWrites();
    }

    GLToy_ForceInline static void SetBlendFunction( const u_int uSourceBlend, const u_int uDestinationBlend )
    {
        Platform_GLToy_Render::SetBlendFunction( uSourceBlend, uDestinationBlend );
    }

    GLToy_ForceInline static void DisableBackFaceCulling()
    {
        Platform_GLToy_Render::DisableBackFaceCulling();
    }

    GLToy_ForceInline static void EnableBackFaceCulling()
    {
        Platform_GLToy_Render::EnableBackFaceCulling();
    }

    GLToy_ForceInline static void SetCCWFaceWinding()
    {
        Platform_GLToy_Render::SetCCWFaceWinding();
    }

    GLToy_ForceInline static void SetCWFaceWinding()
    {
        Platform_GLToy_Render::SetCWFaceWinding();
    }

    GLToy_ForceInline static void SetVsyncEnabled( const bool bEnabled )
    {
        s_bVsync = bEnabled;
        // pretend this is a console variable
        GLToy_Console::Print( GLToy_String( "vsync is set to " ) + ( bEnabled ? "true" : "false" ) );
        Platform_GLToy_Render::SetVsyncEnabled( bEnabled );
    }

    GLToy_ForceInline static void EnableVertexBuffers()
    {
        Platform_GLToy_Render::EnableVertexBuffers();
    }

    GLToy_ForceInline static void EnableIndexBuffers()
    {
        Platform_GLToy_Render::EnableIndexBuffers();
    }

    GLToy_ForceInline static void DisableVertexBuffers()
    {
        Platform_GLToy_Render::DisableVertexBuffers();
    }

    GLToy_ForceInline static void DisableIndexBuffers()
    {
        Platform_GLToy_Render::DisableIndexBuffers();
    }

    GLToy_ForceInline static void DrawBuffers( const int iCount, const u_int* const puBuffers )
    {
        Platform_GLToy_Render::DrawBuffers( iCount, puBuffers );
    }

    GLToy_ForceInline static bool IsRenderbuffer( const u_int uRenderBuffer )
    {
        return Platform_GLToy_Render::IsRenderbuffer( uRenderBuffer );
    }

    GLToy_ForceInline static void BindRenderbuffer( const u_int uTarget, const u_int uRenderBuffer )
    {
        Platform_GLToy_Render::BindRenderbuffer( uTarget, uRenderBuffer );
    }

    GLToy_ForceInline static void DeleteRenderbuffers( const int iCount, u_int* const puRenderBuffers )
    {
        Platform_GLToy_Render::DeleteFramebuffers( iCount, puRenderBuffers );
    }

    GLToy_ForceInline static void GenRenderbuffers( const int iCount, u_int* const puRenderBuffers )
    {
        Platform_GLToy_Render::GenRenderbuffers( iCount, puRenderBuffers );
    }

    GLToy_ForceInline static void RenderbufferStorage( const u_int uTarget, const u_int uInternalFormat, const int iWidth, const int iHeight )
    {
        Platform_GLToy_Render::RenderbufferStorage( uTarget, uInternalFormat, iWidth, iHeight );
    }

    GLToy_ForceInline static void GetRenderbufferParameter( const u_int uTarget, const u_int uPName, int* const piParams )
    {
        Platform_GLToy_Render::GetRenderbufferParameter( uTarget, uPName, piParams );
    }

    GLToy_ForceInline static bool IsFramebuffer( const u_int uFrameBuffer )
    {
        return Platform_GLToy_Render::IsFramebuffer( uFrameBuffer );
    }

    GLToy_ForceInline static void BindFramebuffer( const u_int uTarget, const u_int uFrameBuffer )
    {
        Platform_GLToy_Render::BindFramebuffer( uTarget, uFrameBuffer );
    }

    GLToy_ForceInline static void DeleteFramebuffers( const int iCount, u_int* const puFrameBuffers )
    {
        Platform_GLToy_Render::DeleteFramebuffers( iCount, puFrameBuffers );
    }

    GLToy_ForceInline static void GenFramebuffers( const int iCount, u_int* const puFrameBuffers )
    {
        Platform_GLToy_Render::GenFramebuffers( iCount, puFrameBuffers );
    }

    GLToy_ForceInline static u_int CheckFramebufferStatus( const u_int uTarget )
    {
        return Platform_GLToy_Render::CheckFramebufferStatus( uTarget );
    }

    GLToy_ForceInline static void FramebufferTexture1D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel )
    {
        Platform_GLToy_Render::FramebufferTexture1D( uTarget, uAttachment, uTexTarget, uTexture, iLevel );
    }

    GLToy_ForceInline static void FramebufferTexture2D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel )
    {
        Platform_GLToy_Render::FramebufferTexture2D( uTarget, uAttachment, uTexTarget, uTexture, iLevel );
    }

    GLToy_ForceInline static void FramebufferTexture3D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel, const int iZOffset )
    {
        Platform_GLToy_Render::FramebufferTexture3D( uTarget, uAttachment, uTexTarget, uTexture, iLevel, iZOffset );
    }

    GLToy_ForceInline static void FramebufferRenderbuffer( const u_int uTarget, const u_int uAttachment, const u_int uRenderBufferTarget, const u_int uRenderBuffer )
    {
        Platform_GLToy_Render::FramebufferRenderbuffer( uTarget, uAttachment, uRenderBufferTarget, uRenderBuffer );
    }

    GLToy_ForceInline void GetFramebufferAttachmentParameter( const u_int uTarget, const u_int uAttachment, const u_int uPName, int* const piParams )
    {
        Platform_GLToy_Render::GetFramebufferAttachmentParameter( uTarget, uAttachment, uPName, piParams );
    }

    GLToy_ForceInline static void GenerateMipmap( const u_int uTarget )
    {
        Platform_GLToy_Render::GenerateMipmap( uTarget );
    }

    GLToy_ForceInline static void GenBuffers( const u_int uCount, u_int* const puIDs )
    {
        Platform_GLToy_Render::GenBuffers( uCount, puIDs );
    }

    GLToy_ForceInline static void DeleteBuffers( const u_int uCount, u_int* const puIDs )
    {
        Platform_GLToy_Render::DeleteBuffers( uCount, puIDs );
    }

    GLToy_ForceInline static void BindBuffer( const u_int uTarget, const u_int uID )
    {
        Platform_GLToy_Render::BindBuffer( uTarget, uID );
    }

    GLToy_ForceInline static void BufferData( const u_int uTarget, const u_int uSizeInBytes, const void* const pData, const u_int uType )
    {
        Platform_GLToy_Render::BufferData( uTarget, uSizeInBytes, pData, uType );
    }

    GLToy_ForceInline static void NormalPointer( const u_int uComponentCount, /*const u_int uType,*/ const u_int uStride, const void* const pOffset )
    {
        Platform_GLToy_Render::NormalPointer( uComponentCount, /*uType,*/ uStride, pOffset );
    }

    GLToy_ForceInline static void VertexPointer( const u_int uComponentCount, /*const u_int uType,*/ const u_int uStride, const void* const pOffset )
    {
        Platform_GLToy_Render::VertexPointer( uComponentCount, /*uType,*/ uStride, pOffset );
    }

    GLToy_ForceInline static void TexCoordPointer( const u_int uComponentCount, /*const u_int uType,*/ const u_int uStride, const void* const pOffset )
    {
        Platform_GLToy_Render::TexCoordPointer( uComponentCount, /*uType,*/ uStride, pOffset );
    }

    GLToy_ForceInline static void DrawTriangles( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawTriangles( uStart, uEnd, uCount, uOffset );
    }

    GLToy_ForceInline static void DrawTriangleStrip( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawTriangleStrip( uStart, uEnd, uCount, uOffset );
    }

    GLToy_ForceInline static void DrawPolygon( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawPolygon( uStart, uEnd, uCount, uOffset );
    }

    GLToy_ForceInline static void DrawTrianglesNoRange( const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawTrianglesNoRange( uCount, uOffset );
    }

    GLToy_ForceInline static void DrawTriangleStripNoRange( const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawTriangleStripNoRange( uCount, uOffset );
    }

    GLToy_ForceInline static void DrawPolygonNoRange( const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawPolygonNoRange( uCount, uOffset );
    }

    GLToy_ForceInline static void DrawTrianglesNoIndexBuffer( const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawTrianglesNoIndexBuffer( uCount, uOffset );
    }

    GLToy_ForceInline static void DrawTriangleStripNoIndexBuffer( const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawTriangleStripNoIndexBuffer( uCount, uOffset );
    }

    GLToy_ForceInline static void DrawPolygonNoIndexBuffer( const u_int uCount, const u_int uOffset )
    {
        Platform_GLToy_Render::DrawPolygonNoIndexBuffer( uCount, uOffset );
    }

    GLToy_ForceInline static void ClientActiveTexture( const u_int uTextureUnit )
    {
        return Platform_GLToy_Render::ClientActiveTexture( uTextureUnit );
    }

    GLToy_ForceInline static bool IsShader( const u_int uID )
    {
        return Platform_GLToy_Render::IsShader( uID );
    }

    GLToy_ForceInline static u_int CreateFragmentShader()
    {
        return Platform_GLToy_Render::CreateFragmentShader();
    }

    GLToy_ForceInline static u_int CreateVertexShader()
    {
        return Platform_GLToy_Render::CreateVertexShader();
    }

    GLToy_ForceInline static u_int CreateProgram()
    {
        return Platform_GLToy_Render::CreateProgram();
    }

    GLToy_ForceInline static void DeleteShader( u_int uShaderID )
    {
        Platform_GLToy_Render::DeleteShader( uShaderID );
    }

    GLToy_ForceInline static void DeleteProgram( u_int uProgramID )
    {
        Platform_GLToy_Render::DeleteProgram( uProgramID );
    }

    GLToy_ForceInline static void ValidateProgram( u_int uProgramID )
    {
        Platform_GLToy_Render::ValidateProgram( uProgramID );
    }

    GLToy_ForceInline static void CompileShader( u_int uShaderID )
    {
        Platform_GLToy_Render::CompileShader( uShaderID );
    }

    GLToy_ForceInline static void LinkProgram( u_int uProgramID )
    {
        Platform_GLToy_Render::LinkProgram( uProgramID );
    }

    GLToy_ForceInline static void UseProgram( u_int uProgramID )
    {
        Platform_GLToy_Render::UseProgram( uProgramID );
    }

    GLToy_ForceInline static void AttachShader( u_int uProgramID, u_int uShaderID )
    {
        Platform_GLToy_Render::AttachShader( uProgramID, uShaderID );
    }

    GLToy_ForceInline static void DetachShader( u_int uProgramID, u_int uShaderID )
    {
        Platform_GLToy_Render::DetachShader( uProgramID, uShaderID );
    }

    GLToy_ForceInline static void ShaderSource( u_int uShaderID, int iStringCount, char** ppszStrings, const int* piLengths )
    {
        Platform_GLToy_Render::ShaderSource( uShaderID, iStringCount, ppszStrings, piLengths );
    }

    GLToy_ForceInline static void GetProgramInfoLog( u_int uProgramID, int iMaxLength, int* iLength, char* szInfoLog )
    {
        Platform_GLToy_Render::GetProgramInfoLog( uProgramID, iMaxLength,  iLength, szInfoLog );
    }

    GLToy_ForceInline static void GetShaderInfoLog( u_int uShaderID, int iMaxLength, int* iLength, char* szInfoLog )
    {
        Platform_GLToy_Render::GetShaderInfoLog( uShaderID, iMaxLength, iLength, szInfoLog );
    }

    GLToy_ForceInline static u_int GetUniformID( u_int uProgramID, const char* szName )
    {
        return Platform_GLToy_Render::GetUniformID( uProgramID, szName );
    }

    GLToy_ForceInline static u_int GetAttributeID( u_int uProgramID, const char* szName )
    {
        return Platform_GLToy_Render::GetAttributeID( uProgramID, szName );
    }

    GLToy_ForceInline static void BindAttributeID( u_int uProgramID, u_int uIndex, const char* szName )
    {
        Platform_GLToy_Render::BindAttributeID( uProgramID, uIndex, szName );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, int iValue )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, iValue );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, int iValue1, int iValue2 )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, iValue1, iValue2 );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3 )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, iValue1, iValue2, iValue3 );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, iValue1, iValue2, iValue3, iValue4 );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, float fValue )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, fValue );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, float fValue1, float fValue2 )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, fValue1, fValue2 );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3 )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, fValue1, fValue2, fValue3 );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, fValue1, fValue2, fValue3, fValue4 );
    }

    GLToy_ForceInline static void SetUniform( u_int uUniformID, const GLToy_Matrix_4& xValue )
    {
        Platform_GLToy_Render::SetUniform( uUniformID, xValue );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, int iValue )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, iValue );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2 )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, iValue1, iValue2 );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3 )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, iValue1, iValue2, iValue3 );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, iValue1, iValue2, iValue3, iValue4 );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, float fValue )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, fValue );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2 )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, fValue1, fValue2 );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3 )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, fValue1, fValue2, fValue3 );
    }

    GLToy_ForceInline static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 )
    {
        Platform_GLToy_Render::SetAttribute( uAttributeID, fValue1, fValue2, fValue3, fValue4 );
    }

    GLToy_ForceInline static void BeginQuery( u_int uTarget, u_int uID )
    {
        Platform_GLToy_Render::BeginQuery( uTarget, uID );
    }

    GLToy_ForceInline static void EndQuery( u_int uTarget )
    {
        Platform_GLToy_Render::EndQuery( uTarget );
    }

    GLToy_ForceInline static void GenerateQueries( u_int uCount, u_int* puIDs )
    {
        Platform_GLToy_Render::GenerateQueries( uCount, puIDs );
    }

    GLToy_ForceInline static void DeleteQueries( u_int uCount, const u_int* puIDs )
    {
        Platform_GLToy_Render::DeleteQueries( uCount, puIDs );
    }

    GLToy_ForceInline static void GetQueryObject( u_int uID, u_int uParameterName, int* piParameters )
    {
        Platform_GLToy_Render::GetQueryObject( uID, uParameterName, piParameters );
    }

    GLToy_ForceInline static void GetQueryObject( u_int uID, u_int uParameterName, unsigned long long* pullParameters )
    {
        Platform_GLToy_Render::GetQueryObject( uID, uParameterName, pullParameters );
    }

    GLToy_ForceInline static bool IsIntelGraphicsCard()
    {
        return Platform_GLToy_Render::IsIntelGraphicsCard();
    }

    GLToy_ForceInline static bool IsExtraCrappyIntelGraphicsCard()
    {
        return Platform_GLToy_Render::IsExtraCrappyIntelGraphicsCard();
    }

    GLToy_ForceInline static const GLToy_Vector_2& GetClipPlanes() { return s_xClipPlanes; }

    GLToy_ForceInline static void NextDebugBuffer() { ++s_uCurrentDebugBuffer; s_uCurrentDebugBuffer %= 11; }
    GLToy_ForceInline static void PreviousDebugBuffer() { --s_uCurrentDebugBuffer; s_uCurrentDebugBuffer %= 11; }

private:

    GLToy_ForceInline static void SetClipPlanes( const GLToy_Vector_2& xClipPlanes ) { s_xClipPlanes = xClipPlanes; }

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Render();

    static float s_fFOV;
    static float s_fAspectRatio;
    static bool s_bDrawFPS;
    static bool s_bDrawTimers;
    static bool s_bDrawCounters;
    static bool s_bDrawBuffers;
    static bool s_bDrawNormals;
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
    static u_int s_uFrameBufferNoDepth;
    static u_int s_uSwapBufferNoDepth;
    static u_int* s_puCurrentBufferNoDepth;

    static GLToy_Vector_2 s_xClipPlanes;

    // deferred buffers
    static u_int s_uDeferredBuffer;
    static u_int s_uDiffuseTexture;
    static u_int s_uNormalTexture;
    static u_int s_uSpecularTexture;

    static u_int s_uCurrentDebugBuffer;

    static GLToy_List< const GLToy_Renderable* > s_xDeferredRenderables;
    static GLToy_BinaryTree< const GLToy_Renderable*, float > s_xTransparents;

#ifdef GLTOY_DEBUG

public:

    static struct DebugStateType
    {
        bool m_bBlendingEnabled;
        bool m_bCullingEnabled;
        bool m_bCullBackFaces;
        bool m_bCCWFrontFaces;
        u_int m_uSourceBlendFunction;
        u_int m_uDestinationBlendFunction;
    } DebugState;

#endif

};

#endif
