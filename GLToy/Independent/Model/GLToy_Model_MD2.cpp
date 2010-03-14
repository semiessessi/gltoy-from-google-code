/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model_MD2.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_MD2_CommandVertex
{

    float   m_fU;
    float   m_fV;
    u_int   m_uIndex;

};


struct GLToy_MD2_AnimationState
{

    u_int   m_uFirstFrame;
    u_int   m_uLastFrame;
    u_int   m_uFPS;

    float   m_fCurrentTime;
    float   m_fOldTime;
    float   m_fInterpolate;

    u_int   m_uType;

    u_int   m_uCurrentFrame;
    u_int   m_uNextFrame;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_MD2_Animation xMD2_ANIM_LIST[ GLToy_Model_MD2::NumAnimIDs ] =
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
{
}

void GLToy_Model_MD2::Render() const
{
    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCCWFaceWinding();

    // fall back on triangles for now
    if( m_xGLCommands.GetCount() == 0 )
    {
        GLToy_Render::StartSubmittingTriangles();

        GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
        
        GLToy_ConstIterate( GLToy_MD2_Triangle, xIterator, &m_xTriangles )
        {
            const GLToy_MD2_Triangle& xTriangle = xIterator.Current();

            for( u_int u = 0; u < 3; ++u )
            {
                GLToy_Render::SubmitTextureCoordinate( m_xTexCoords[ xTriangle.m_ausTexCoords[ u ] ] );
                GLToy_Render::SubmitNormal( xMD2_NORMALS[ m_xNormalIndices[ xTriangle.m_ausVertices[ u ] ] ] );
                GLToy_Render::SubmitVertex( m_xVertices[ xTriangle.m_ausVertices[ u ] ] );
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

        GLToy_ConstIterate( GLToy_MD2_CommandVertex, xIterator, &xCommandList )
        {
            GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( xIterator.Current().m_fU, xIterator.Current().m_fV, 0.0f ) );
            GLToy_Render::SubmitNormal( xMD2_NORMALS[ m_xNormalIndices[ xIterator.Current().m_uIndex ] ] );
            GLToy_Render::SubmitVertex( m_xVertices[ xIterator.Current().m_uIndex ] );
            
            uIP += 3;
        }

        GLToy_Render::EndSubmit();
    }

    GLToy_Render::DisableBackFaceCulling();
}
