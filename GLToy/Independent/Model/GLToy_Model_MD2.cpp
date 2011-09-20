/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

// This file's header
#include <Model/GLToy_Model_MD2.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Material/GLToy_Material_System.h>
#include <Render/GLToy_Render.h>
//#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_MD2_CommandVertex
{

    float   m_fU;
    float   m_fV;
    u_int   m_uIndex;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_MD2_Animation axMD2_ANIMS[ GLToy_Model_MD2::NUM_ANIMIDS ] =
{
    { 0, 39, 9 },
    { 40, 45, 10 },
    { 46, 53, 10 },
    { 54, 57, 7 },
    { 58, 61, 7 },
    { 62, 65, 7 },
    { 66, 71, 7 },
    { 72, 83, 7 },
    { 84, 94, 7 },
    { 95, 111, 10 },
    { 112, 122, 7 },
    { 123, 134, 6 },
    { 135, 153, 10 },
    { 154, 159, 7 },
    { 160, 168, 10 },
    { 196, 172, 7 },
    { 173, 177, 5 },
    { 178, 183, 7 },
    { 184, 189, 7 },
    { 190, 197, 7 },
    { 198, 198, 5 }
};

static const GLToy_Vector_3 xMD2_NORMALS[] =
{
    GLToy_Vector_3( -0.525731f,  0.000000f,  0.850651f ), 
    GLToy_Vector_3( -0.442863f,  0.238856f,  0.864188f ), 
    GLToy_Vector_3( -0.295242f,  0.000000f,  0.955423f ), 
    GLToy_Vector_3( -0.309017f,  0.500000f,  0.809017f ), 
    GLToy_Vector_3( -0.162460f,  0.262866f,  0.951056f ), 
    GLToy_Vector_3(  0.000000f,  0.000000f,  1.000000f ), 
    GLToy_Vector_3(  0.000000f,  0.850651f,  0.525731f ), 
    GLToy_Vector_3( -0.147621f,  0.716567f,  0.681718f ), 
    GLToy_Vector_3(  0.147621f,  0.716567f,  0.681718f ), 
    GLToy_Vector_3(  0.000000f,  0.525731f,  0.850651f ), 
    GLToy_Vector_3(  0.309017f,  0.500000f,  0.809017f ), 
    GLToy_Vector_3(  0.525731f,  0.000000f,  0.850651f ), 
    GLToy_Vector_3(  0.295242f,  0.000000f,  0.955423f ), 
    GLToy_Vector_3(  0.442863f,  0.238856f,  0.864188f ), 
    GLToy_Vector_3(  0.162460f,  0.262866f,  0.951056f ), 
    GLToy_Vector_3( -0.681718f,  0.147621f,  0.716567f ), 
    GLToy_Vector_3( -0.809017f,  0.309017f,  0.500000f ), 
    GLToy_Vector_3( -0.587785f,  0.425325f,  0.688191f ), 
    GLToy_Vector_3( -0.850651f,  0.525731f,  0.000000f ), 
    GLToy_Vector_3( -0.864188f,  0.442863f,  0.238856f ), 
    GLToy_Vector_3( -0.716567f,  0.681718f,  0.147621f ), 
    GLToy_Vector_3( -0.688191f,  0.587785f,  0.425325f ), 
    GLToy_Vector_3( -0.500000f,  0.809017f,  0.309017f ), 
    GLToy_Vector_3( -0.238856f,  0.864188f,  0.442863f ), 
    GLToy_Vector_3( -0.425325f,  0.688191f,  0.587785f ), 
    GLToy_Vector_3( -0.716567f,  0.681718f, -0.147621f ), 
    GLToy_Vector_3( -0.500000f,  0.809017f, -0.309017f ), 
    GLToy_Vector_3( -0.525731f,  0.850651f,  0.000000f ), 
    GLToy_Vector_3(  0.000000f,  0.850651f, -0.525731f ), 
    GLToy_Vector_3( -0.238856f,  0.864188f, -0.442863f ), 
    GLToy_Vector_3(  0.000000f,  0.955423f, -0.295242f ), 
    GLToy_Vector_3( -0.262866f,  0.951056f, -0.162460f ), 
    GLToy_Vector_3(  0.000000f,  1.000000f,  0.000000f ), 
    GLToy_Vector_3(  0.000000f,  0.955423f,  0.295242f ), 
    GLToy_Vector_3( -0.262866f,  0.951056f,  0.162460f ), 
    GLToy_Vector_3(  0.238856f,  0.864188f,  0.442863f ), 
    GLToy_Vector_3(  0.262866f,  0.951056f,  0.162460f ), 
    GLToy_Vector_3(  0.500000f,  0.809017f,  0.309017f ), 
    GLToy_Vector_3(  0.238856f,  0.864188f, -0.442863f ), 
    GLToy_Vector_3(  0.262866f,  0.951056f, -0.162460f ), 
    GLToy_Vector_3(  0.500000f,  0.809017f, -0.309017f ), 
    GLToy_Vector_3(  0.850651f,  0.525731f,  0.000000f ), 
    GLToy_Vector_3(  0.716567f,  0.681718f,  0.147621f ), 
    GLToy_Vector_3(  0.716567f,  0.681718f, -0.147621f ), 
    GLToy_Vector_3(  0.525731f,  0.850651f,  0.000000f ), 
    GLToy_Vector_3(  0.425325f,  0.688191f,  0.587785f ), 
    GLToy_Vector_3(  0.864188f,  0.442863f,  0.238856f ), 
    GLToy_Vector_3(  0.688191f,  0.587785f,  0.425325f ), 
    GLToy_Vector_3(  0.809017f,  0.309017f,  0.500000f ), 
    GLToy_Vector_3(  0.681718f,  0.147621f,  0.716567f ), 
    GLToy_Vector_3(  0.587785f,  0.425325f,  0.688191f ), 
    GLToy_Vector_3(  0.955423f,  0.295242f,  0.000000f ), 
    GLToy_Vector_3(  1.000000f,  0.000000f,  0.000000f ), 
    GLToy_Vector_3(  0.951056f,  0.162460f,  0.262866f ), 
    GLToy_Vector_3(  0.850651f, -0.525731f,  0.000000f ), 
    GLToy_Vector_3(  0.955423f, -0.295242f,  0.000000f ), 
    GLToy_Vector_3(  0.864188f, -0.442863f,  0.238856f ), 
    GLToy_Vector_3(  0.951056f, -0.162460f,  0.262866f ), 
    GLToy_Vector_3(  0.809017f, -0.309017f,  0.500000f ), 
    GLToy_Vector_3(  0.681718f, -0.147621f,  0.716567f ), 
    GLToy_Vector_3(  0.850651f,  0.000000f,  0.525731f ), 
    GLToy_Vector_3(  0.864188f,  0.442863f, -0.238856f ), 
    GLToy_Vector_3(  0.809017f,  0.309017f, -0.500000f ), 
    GLToy_Vector_3(  0.951056f,  0.162460f, -0.262866f ), 
    GLToy_Vector_3(  0.525731f,  0.000000f, -0.850651f ), 
    GLToy_Vector_3(  0.681718f,  0.147621f, -0.716567f ), 
    GLToy_Vector_3(  0.681718f, -0.147621f, -0.716567f ), 
    GLToy_Vector_3(  0.850651f,  0.000000f, -0.525731f ), 
    GLToy_Vector_3(  0.809017f, -0.309017f, -0.500000f ), 
    GLToy_Vector_3(  0.864188f, -0.442863f, -0.238856f ), 
    GLToy_Vector_3(  0.951056f, -0.162460f, -0.262866f ), 
    GLToy_Vector_3(  0.147621f,  0.716567f, -0.681718f ), 
    GLToy_Vector_3(  0.309017f,  0.500000f, -0.809017f ), 
    GLToy_Vector_3(  0.425325f,  0.688191f, -0.587785f ), 
    GLToy_Vector_3(  0.442863f,  0.238856f, -0.864188f ), 
    GLToy_Vector_3(  0.587785f,  0.425325f, -0.688191f ), 
    GLToy_Vector_3(  0.688191f,  0.587785f, -0.425325f ), 
    GLToy_Vector_3( -0.147621f,  0.716567f, -0.681718f ), 
    GLToy_Vector_3( -0.309017f,  0.500000f, -0.809017f ), 
    GLToy_Vector_3(  0.000000f,  0.525731f, -0.850651f ), 
    GLToy_Vector_3( -0.525731f,  0.000000f, -0.850651f ), 
    GLToy_Vector_3( -0.442863f,  0.238856f, -0.864188f ), 
    GLToy_Vector_3( -0.295242f,  0.000000f, -0.955423f ), 
    GLToy_Vector_3( -0.162460f,  0.262866f, -0.951056f ), 
    GLToy_Vector_3(  0.000000f,  0.000000f, -1.000000f ), 
    GLToy_Vector_3(  0.295242f,  0.000000f, -0.955423f ), 
    GLToy_Vector_3(  0.162460f,  0.262866f, -0.951056f ), 
    GLToy_Vector_3( -0.442863f, -0.238856f, -0.864188f ), 
    GLToy_Vector_3( -0.309017f, -0.500000f, -0.809017f ), 
    GLToy_Vector_3( -0.162460f, -0.262866f, -0.951056f ), 
    GLToy_Vector_3(  0.000000f, -0.850651f, -0.525731f ), 
    GLToy_Vector_3( -0.147621f, -0.716567f, -0.681718f ), 
    GLToy_Vector_3(  0.147621f, -0.716567f, -0.681718f ), 
    GLToy_Vector_3(  0.000000f, -0.525731f, -0.850651f ), 
    GLToy_Vector_3(  0.309017f, -0.500000f, -0.809017f ), 
    GLToy_Vector_3(  0.442863f, -0.238856f, -0.864188f ), 
    GLToy_Vector_3(  0.162460f, -0.262866f, -0.951056f ), 
    GLToy_Vector_3(  0.238856f, -0.864188f, -0.442863f ), 
    GLToy_Vector_3(  0.500000f, -0.809017f, -0.309017f ), 
    GLToy_Vector_3(  0.425325f, -0.688191f, -0.587785f ), 
    GLToy_Vector_3(  0.716567f, -0.681718f, -0.147621f ), 
    GLToy_Vector_3(  0.688191f, -0.587785f, -0.425325f ), 
    GLToy_Vector_3(  0.587785f, -0.425325f, -0.688191f ), 
    GLToy_Vector_3(  0.000000f, -0.955423f, -0.295242f ), 
    GLToy_Vector_3(  0.000000f, -1.000000f,  0.000000f ), 
    GLToy_Vector_3(  0.262866f, -0.951056f, -0.162460f ), 
    GLToy_Vector_3(  0.000000f, -0.850651f,  0.525731f ), 
    GLToy_Vector_3(  0.000000f, -0.955423f,  0.295242f ), 
    GLToy_Vector_3(  0.238856f, -0.864188f,  0.442863f ), 
    GLToy_Vector_3(  0.262866f, -0.951056f,  0.162460f ), 
    GLToy_Vector_3(  0.500000f, -0.809017f,  0.309017f ), 
    GLToy_Vector_3(  0.716567f, -0.681718f,  0.147621f ), 
    GLToy_Vector_3(  0.525731f, -0.850651f,  0.000000f ), 
    GLToy_Vector_3( -0.238856f, -0.864188f, -0.442863f ), 
    GLToy_Vector_3( -0.500000f, -0.809017f, -0.309017f ), 
    GLToy_Vector_3( -0.262866f, -0.951056f, -0.162460f ), 
    GLToy_Vector_3( -0.850651f, -0.525731f,  0.000000f ), 
    GLToy_Vector_3( -0.716567f, -0.681718f, -0.147621f ), 
    GLToy_Vector_3( -0.716567f, -0.681718f,  0.147621f ), 
    GLToy_Vector_3( -0.525731f, -0.850651f,  0.000000f ), 
    GLToy_Vector_3( -0.500000f, -0.809017f,  0.309017f ), 
    GLToy_Vector_3( -0.238856f, -0.864188f,  0.442863f ), 
    GLToy_Vector_3( -0.262866f, -0.951056f,  0.162460f ), 
    GLToy_Vector_3( -0.864188f, -0.442863f,  0.238856f ), 
    GLToy_Vector_3( -0.809017f, -0.309017f,  0.500000f ), 
    GLToy_Vector_3( -0.688191f, -0.587785f,  0.425325f ), 
    GLToy_Vector_3( -0.681718f, -0.147621f,  0.716567f ), 
    GLToy_Vector_3( -0.442863f, -0.238856f,  0.864188f ), 
    GLToy_Vector_3( -0.587785f, -0.425325f,  0.688191f ), 
    GLToy_Vector_3( -0.309017f, -0.500000f,  0.809017f ), 
    GLToy_Vector_3( -0.147621f, -0.716567f,  0.681718f ), 
    GLToy_Vector_3( -0.425325f, -0.688191f,  0.587785f ), 
    GLToy_Vector_3( -0.162460f, -0.262866f,  0.951056f ), 
    GLToy_Vector_3(  0.442863f, -0.238856f,  0.864188f ), 
    GLToy_Vector_3(  0.162460f, -0.262866f,  0.951056f ), 
    GLToy_Vector_3(  0.309017f, -0.500000f,  0.809017f ), 
    GLToy_Vector_3(  0.147621f, -0.716567f,  0.681718f ), 
    GLToy_Vector_3(  0.000000f, -0.525731f,  0.850651f ), 
    GLToy_Vector_3(  0.425325f, -0.688191f,  0.587785f ), 
    GLToy_Vector_3(  0.587785f, -0.425325f,  0.688191f ), 
    GLToy_Vector_3(  0.688191f, -0.587785f,  0.425325f ), 
    GLToy_Vector_3( -0.955423f,  0.295242f,  0.000000f ), 
    GLToy_Vector_3( -0.951056f,  0.162460f,  0.262866f ), 
    GLToy_Vector_3( -1.000000f,  0.000000f,  0.000000f ), 
    GLToy_Vector_3( -0.850651f,  0.000000f,  0.525731f ), 
    GLToy_Vector_3( -0.955423f, -0.295242f,  0.000000f ), 
    GLToy_Vector_3( -0.951056f, -0.162460f,  0.262866f ), 
    GLToy_Vector_3( -0.864188f,  0.442863f, -0.238856f ), 
    GLToy_Vector_3( -0.951056f,  0.162460f, -0.262866f ), 
    GLToy_Vector_3( -0.809017f,  0.309017f, -0.500000f ), 
    GLToy_Vector_3( -0.864188f, -0.442863f, -0.238856f ), 
    GLToy_Vector_3( -0.951056f, -0.162460f, -0.262866f ), 
    GLToy_Vector_3( -0.809017f, -0.309017f, -0.500000f ), 
    GLToy_Vector_3( -0.681718f,  0.147621f, -0.716567f ), 
    GLToy_Vector_3( -0.681718f, -0.147621f, -0.716567f ), 
    GLToy_Vector_3( -0.850651f,  0.000000f, -0.525731f ), 
    GLToy_Vector_3( -0.688191f,  0.587785f, -0.425325f ), 
    GLToy_Vector_3( -0.587785f,  0.425325f, -0.688191f ), 
    GLToy_Vector_3( -0.425325f,  0.688191f, -0.587785f ), 
    GLToy_Vector_3( -0.425325f, -0.688191f, -0.587785f ), 
    GLToy_Vector_3( -0.587785f, -0.425325f, -0.688191f ), 
    GLToy_Vector_3( -0.688191f, -0.587785f, -0.425325f )
};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_MD2::GLToy_Model_MD2()
: m_xNormalIndices()
, m_xGLCommands()
, m_xUVs()
, m_xTriangles()
, m_xWorkingVertices()
, m_xWorkingNormals()
, m_uFrameSize( 0 )
//, m_pxTexture( NULL )
, m_pxMaterial( NULL )
{
}

void GLToy_Model_MD2::InitialiseFirstFrameData()
{
    for( u_int u = 0; u < m_uFrameSize; ++u )
    {
        m_xWorkingVertices[ u ] = m_xVertices[ u ];
        m_xWorkingNormals[ u ] = xMD2_NORMALS[ m_xNormalIndices[ u ] ];
    }
}

void GLToy_Model_MD2::SetFrameSize( const u_int uFrameSize )
{
    m_xWorkingVertices.Resize( uFrameSize );
    m_xWorkingNormals.Resize( uFrameSize );
    m_uFrameSize = uFrameSize;
}

GLToy_AnimationStack* GLToy_Model_MD2::CreateAnimationStack() const
{
    return new GLToy_MD2_AnimationStack;
}

void GLToy_Model_MD2::RenderDeferred() const
{
	// TODO: transform
	const GLToy_Vector_3& xPosition = GetPosition();
	GLToy_Render::Translate( xPosition );

	if( m_pxMaterial )
    {
		if( !( m_pxMaterial->IsDeferred() ) )
		{
			return;
		}
		GLToy_Material_System::BindMaterial( m_pxMaterial->GetHash() );
        //m_pxMaterial->Bind();
    }

	GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();

    // fall back on triangles if we have no GL commands
    if( m_xGLCommands.IsEmpty() )
    {
        GLToy_Render::StartSubmittingTriangles();

        GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
        
        for( GLToy_ConstIterator< GLToy_MD2_Triangle > xIterator; !xIterator.Done( m_xTriangles ); xIterator.Next() )
		{
			const GLToy_MD2_Triangle& xTriangle = xIterator.Current( m_xTriangles );
            for( u_int u = 0; u < 3; ++u )
            {
                //GLToy_Render::SubmitUV( m_xUVs[ xTriangle.m_ausUVs[ u ] ] );
                //GLToy_Render::SubmitNormal( m_xWorkingNormals[ xTriangle.m_ausVertices[ u ] ] );
                //GLToy_Render::SubmitVertex( m_xWorkingVertices[ xTriangle.m_ausVertices[ u ] ] );

				GLToy_Render::SubmitDeferredVertex( m_xWorkingVertices[ xTriangle.m_ausVertices[ u ] ], m_xUVs[ xTriangle.m_ausUVs[ u ] ], m_xWorkingNormals[ xTriangle.m_ausVertices[ u ] ], GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
            }
        }

        GLToy_Render::EndSubmit();

        return;
    }

    u_int uIP = 0;
    while( uIP < m_xGLCommands.GetCount() )
    {
        int iCount = *reinterpret_cast< const int* >( &m_xGLCommands[ uIP ] );
        if( iCount < 0 )
        {
            GLToy_Render::StartSubmittingTriangleFan();
            iCount = -iCount;
        }
        else
        {
            GLToy_Render::StartSubmittingTriangleStrip();
        }

        GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );

        ++uIP;

        GLToy_ConstPointerArray< GLToy_MD2_CommandVertex > xCommandList( reinterpret_cast< const GLToy_MD2_CommandVertex* >( &m_xGLCommands[ uIP ] ), iCount );

        for( GLToy_ConstIterator< GLToy_MD2_CommandVertex > xIterator; !xIterator.Done( xCommandList ); xIterator.Next() )
		{
			const GLToy_MD2_CommandVertex& xCommandVertex = xIterator.Current( xCommandList );
            //GLToy_Render::SubmitUV( GLToy_Vector_3( xCommandVertex.m_fU, xCommandVertex.m_fV, 0.0f ) );
            //GLToy_Render::SubmitNormal( m_xWorkingNormals[ xCommandVertex.m_uIndex ] );
            //GLToy_Render::SubmitVertex( m_xWorkingVertices[ xCommandVertex.m_uIndex ] );

			GLToy_Render::SubmitDeferredVertex( m_xWorkingVertices[ xCommandVertex.m_uIndex ], GLToy_Vector_2( xCommandVertex.m_fU, xCommandVertex.m_fV ), m_xWorkingNormals[ xCommandVertex.m_uIndex ], GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
            
            uIP += 3;
        }

        GLToy_Render::EndSubmit();
    }

    GLToy_Render::DisableBackFaceCulling();

	GLToy_Render::Translate( -xPosition );
}
void GLToy_Model_MD2::Render() const
{
    if( m_pxMaterial )
    {
		if( m_pxMaterial->IsDeferred() )
		{
			GLToy_Render::RegisterDeferred( this );
			return;
		}
        m_pxMaterial->Bind();
    }

	//if( m_pxTexture )
 //   {
 //       m_pxTexture->Bind();
 //   }

    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();

    // fall back on triangles if we have no GL commands
    if( m_xGLCommands.IsEmpty() )
    {
        GLToy_Render::StartSubmittingTriangles();

        GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
        
        for( GLToy_ConstIterator< GLToy_MD2_Triangle > xIterator; !xIterator.Done( m_xTriangles ); xIterator.Next() )
		{
			const GLToy_MD2_Triangle& xTriangle = xIterator.Current( m_xTriangles );
            for( u_int u = 0; u < 3; ++u )
            {
                GLToy_Render::SubmitUV( m_xUVs[ xTriangle.m_ausUVs[ u ] ] );
                GLToy_Render::SubmitNormal( m_xWorkingNormals[ xTriangle.m_ausVertices[ u ] ] );
                GLToy_Render::SubmitVertex( m_xWorkingVertices[ xTriangle.m_ausVertices[ u ] ] );
            }
        }

        GLToy_Render::EndSubmit();

        return;
    }

    u_int uIP = 0;
    while( uIP < m_xGLCommands.GetCount() )
    {
        int iCount = *reinterpret_cast< const int* >( &m_xGLCommands[ uIP ] );
        if( iCount < 0 )
        {
            GLToy_Render::StartSubmittingTriangleFan();
            iCount = -iCount;
        }
        else
        {
            GLToy_Render::StartSubmittingTriangleStrip();
        }

        GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );

        ++uIP;

        GLToy_ConstPointerArray< GLToy_MD2_CommandVertex > xCommandList( reinterpret_cast< const GLToy_MD2_CommandVertex* >( &m_xGLCommands[ uIP ] ), iCount );

        for( GLToy_ConstIterator< GLToy_MD2_CommandVertex > xIterator; !xIterator.Done( xCommandList ); xIterator.Next() )
		{
			const GLToy_MD2_CommandVertex& xCommandVertex = xIterator.Current( xCommandList );
            GLToy_Render::SubmitUV( GLToy_Vector_3( xCommandVertex.m_fU, xCommandVertex.m_fV, 0.0f ) );
            GLToy_Render::SubmitNormal( m_xWorkingNormals[ xCommandVertex.m_uIndex ] );
            GLToy_Render::SubmitVertex( m_xWorkingVertices[ xCommandVertex.m_uIndex ] );
            
            uIP += 3;
        }

        GLToy_Render::EndSubmit();
    }

    GLToy_Render::DisableBackFaceCulling();
}


GLToy_MD2_AnimationState::GLToy_MD2_AnimationState()
: m_eAnimID( GLToy_Model_MD2::ANIMID_STAND )
, m_fTimer( 0.0f )
, m_fTweenInTime( 0.0f )
, m_fTweenOutTime( 0.0f )
, m_fEndTime( static_cast< float >( axMD2_ANIMS[ 0 ].m_uLastFrame ) / static_cast< float >( axMD2_ANIMS[ 0 ].m_uFPS ) )
, m_bLoop( true )
, m_bAnimatedTween( false )
{
}

GLToy_MD2_AnimationState::GLToy_MD2_AnimationState( const GLToy_Model_MD2::AnimID eAnimID, const float fTweenInTime, const float fTweenOutTime, const bool bAnimatedTween )
: m_eAnimID( eAnimID )
, m_fTimer( 0.0f )
, m_fTweenInTime( fTweenInTime )
, m_fTweenOutTime( fTweenOutTime )
, m_bAnimatedTween( bAnimatedTween )
{
    GLToy_Assert( fTweenInTime <= fTweenOutTime, " Bad tween times give to animation state from animation stack" );
    GLToy_Assert( eAnimID < GLToy_Model_MD2::NUM_ANIMIDS, "Invalid anim ID" );
    switch( m_eAnimID )
    {
        case GLToy_Model_MD2::ANIMID_STAND:
        case GLToy_Model_MD2::ANIMID_RUN:
        case GLToy_Model_MD2::ANIMID_CROUCH_STAND:
        case GLToy_Model_MD2::ANIMID_CROUCH_WALK:
        {
            m_bLoop = true;
            break;
        }

        default:
        {
            m_bLoop = false;
            break;
        }
    }

    m_fEndTime = static_cast< float >( axMD2_ANIMS[ m_eAnimID ].m_uLastFrame - axMD2_ANIMS[ m_eAnimID ].m_uFirstFrame ) / static_cast< float >( axMD2_ANIMS[ m_eAnimID ].m_uFPS );
}

void GLToy_MD2_AnimationState::Update()
{
    m_fTimer += GLToy_Timer::GetFrameTime();

    if(  m_fTimer > m_fEndTime )
    {
        m_fTimer = m_bLoop ? 0.0f : m_fEndTime;
    }
}

void GLToy_MD2_AnimationState::Evaluate( GLToy_Model_MD2* const pxModel ) const
{
    const float fAdjustedTime = m_fTimer * static_cast< float >( axMD2_ANIMS[ m_eAnimID ].m_uFPS ) + static_cast< float >( axMD2_ANIMS[ m_eAnimID ].m_uFirstFrame );
    const u_int uFrameSize = pxModel->m_uFrameSize;
    
    if( ( fAdjustedTime * uFrameSize ) > static_cast< float >( pxModel->m_xVertices.GetCount() ) )
    {
        return; // can't animate in this case...
    }
    
    u_int uFrame = static_cast< u_int >( GLToy_Maths::Floor( fAdjustedTime ) );
    u_int uNextFrame = ( uFrame < axMD2_ANIMS[ m_eAnimID ].m_uLastFrame ) ? uFrame + 1 : axMD2_ANIMS[ m_eAnimID ].m_uFirstFrame;
    const float fLerp = fAdjustedTime - static_cast< float >( uFrame );

    GLToy_Array< GLToy_Vector_3 >& xVertices = pxModel->m_xVertices;
    GLToy_Array< u_char >& xNormalIndices = pxModel->m_xNormalIndices;
    GLToy_Array< GLToy_Vector_3 >& xWorkingVertices = pxModel->m_xWorkingVertices;
    GLToy_Array< GLToy_Vector_3 >& xWorkingNormals = pxModel->m_xWorkingNormals;

    const float fTween = ( ( m_fTweenInTime > 0.0f ) ? GLToy_Maths::Min( m_fTimer / m_fTweenInTime, 1.0f ) : 1.0f )
    - ( ( m_fTweenInTime > 0.0f ) ? GLToy_Maths::Max( ( m_fTimer - m_fTweenOutTime ) / ( m_fEndTime - m_fTweenOutTime ), 0.0f ) : 0.0f );

    for( u_int u = 0; u < uFrameSize; ++u )
    {
        const u_int uIndex1 = u + uFrameSize * uFrame;
        const u_int uIndex2 = u + uFrameSize * uNextFrame;
        xWorkingVertices[ u ] = GLToy_Maths::Lerp( xWorkingVertices[ u ], GLToy_Maths::Lerp( xVertices[ uIndex1 ], xVertices[ uIndex2 ], fLerp ), fTween );
        xWorkingNormals[ u ] = GLToy_Maths::Lerp( xWorkingNormals[ u ], GLToy_Maths::Lerp( xMD2_NORMALS[ xNormalIndices[ uIndex1 ] ], xMD2_NORMALS[ xNormalIndices[ uIndex2 ] ], fLerp ), fTween );
    }
}



void GLToy_MD2_AnimationStack::Update()
{
    for( u_int u = 0; u < GetCount(); ++u )
    {
        if( operator []( u ).IsDone() )
        {
            RemoveAt( u );
            --u;
        }
    }

    GLToy_UpdateFunctor< GLToy_MD2_AnimationState > xFunctor;
    Traverse(  xFunctor );
}

void GLToy_MD2_AnimationStack::Evaluate( GLToy_Model* const pxModel ) const
{
    if( IsEmpty() )
    {
        return;
    }

    GLToy_Model_MD2* const pxMD2Model = reinterpret_cast< GLToy_Model_MD2* const >( pxModel );
    GLToy_Array< GLToy_Vector_3 >& xWorkingVertices = pxMD2Model->m_xWorkingVertices;
    GLToy_Array< GLToy_Vector_3 >& xWorkingNormals = pxMD2Model->m_xWorkingNormals;

    for( u_int u = 0; u < pxMD2Model->m_uFrameSize; ++u )
    {
        xWorkingVertices[ u ] = GLToy_Maths::ZeroVector3;
        xWorkingNormals[ u ] = GLToy_Maths::ZeroVector3;
    }

    for( GLToy_ConstIterator< GLToy_MD2_AnimationState > xIterator; !xIterator.Done( *this ); xIterator.Next() )
{
const GLToy_MD2_AnimationState& xState = xIterator.Current( *this );
        xState.Evaluate( pxMD2Model );
    }
}

bool GLToy_MD2_AnimationStack::SupportsAnimID( GLToy_Model* const pxModel, const u_int uAnimID ) const
{
    if( !pxModel )
    {
        return false;
    }
    
    return ( axMD2_ANIMS[ uAnimID ].m_uLastFrame < reinterpret_cast< GLToy_Model_MD2* const >( pxModel )->GetFrameCount() )
        && ( uAnimID < static_cast< u_int >( GLToy_Model_MD2::NUM_ANIMIDS ) );
}

void GLToy_MD2_AnimationStack::Push( const u_int uAnimID, const float fTweenInTime, const float fTweenOutTime, const bool bAnimatedTween )
{
    const float fEndTime = static_cast< float >( axMD2_ANIMS[ uAnimID ].m_uLastFrame - axMD2_ANIMS[ uAnimID ].m_uFirstFrame ) / static_cast< float >( axMD2_ANIMS[ uAnimID ].m_uFPS );
    Append( GLToy_MD2_AnimationState( static_cast< GLToy_Model_MD2::AnimID >( uAnimID ), fTweenInTime, fEndTime - fTweenOutTime, bAnimatedTween ) );
}

void GLToy_MD2_AnimationStack::Stop( const u_int uAnimID, const float fTweenOutTime, const bool bAnimatedTween )
{
}
