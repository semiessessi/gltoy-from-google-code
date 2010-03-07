/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model.h>

// GLToy
#include <File/GLToy_ANSITextFile.h>
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>

// C/C++
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ModelStrip::GLToy_ModelStrip()
: PARENT_DATA()
, PARENT_RENDER()
, m_pxVertices( NULL )
{
}

GLToy_ModelStrip::GLToy_ModelStrip( const GLToy_ModelStrip& xStrip )
: PARENT_DATA( xStrip )
, PARENT_RENDER()
, m_pxVertices( xStrip.m_pxVertices )
{
}

GLToy_ModelStrip::GLToy_ModelStrip( u_int uVertex1, u_int uVertex2, u_int uVertex3 )
: PARENT_DATA()
, PARENT_RENDER()
, m_pxVertices( NULL )
{
    Append( uVertex1 );
    Append( uVertex2 );
    Append( uVertex3 );
}

GLToy_ModelStrip::GLToy_ModelStrip( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4 )
: PARENT_DATA()
, PARENT_RENDER()
, m_pxVertices( NULL )
{
    Append( uVertex1 );
    Append( uVertex2 );
    Append( uVertex3 );
    Append( uVertex4 );
}

GLToy_ModelStrip::GLToy_ModelStrip( const u_int* puVertices, const u_int uCount )
: PARENT_DATA()
, PARENT_RENDER()
, m_pxVertices( NULL )
{
    for( u_int u = 0; u < uCount; ++u )
    {
        Append( puVertices[ u ] );
    }
}

GLToy_ModelStrip& GLToy_ModelStrip::operator =( const GLToy_ModelStrip& xStrip )
{
    PARENT_DATA::operator =( xStrip );
    m_pxVertices = xStrip.m_pxVertices;

    return *this;
}

void GLToy_ModelStrip::Render() const
{
    GLToy_Render::StartSubmittingTriangleStrip();
    
    SubmitVertices();
    
    GLToy_Render::EndSubmit();
}

void GLToy_ModelStrip::SubmitVertices() const
{
    GLToy_ConstIterate( u_int, xIterator, this )
    {
        SubmitVertex( xIterator.Current() );
    }
}

void GLToy_ModelStrip::SubmitVertex( const u_int uIndex ) const
{
    GLToy_Render::SubmitVertex( m_pxVertices[ uIndex ] );
}

//
// GLToy_Model
//

GLToy_Model::GLToy_Model()
: PARENT_DATA()
, PARENT_RENDER()
, m_uVertexCount( 0 )
, m_pxVertices( NULL )
{
}

GLToy_Model::~GLToy_Model()
{
    Reset();
}

void GLToy_Model::Reset()
{
    if( m_pxVertices )
    {
        delete[] m_pxVertices;
        m_pxVertices = NULL;
    }

    DeleteAll();
}

void GLToy_Model::AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 )
{
    const u_int uVertex1 = GetVertexIndex( xVertex1 );
    const u_int uVertex2 = GetVertexIndex( xVertex2 );
    const u_int uVertex3 = GetVertexIndex( xVertex3 );

    Append( new GLToy_ModelStrip( uVertex1, uVertex2, uVertex3 ) );

    UpdateStripPointers();
}

void GLToy_Model::AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 )
{
    const u_int uVertex1 = GetVertexIndex( xVertex1 );
    const u_int uVertex2 = GetVertexIndex( xVertex2 );
    const u_int uVertex3 = GetVertexIndex( xVertex3 );
    const u_int uVertex4 = GetVertexIndex( xVertex4 );

    Append( new GLToy_ModelStrip( uVertex1, uVertex2, uVertex3, uVertex4 ) );

    UpdateStripPointers();
}

void GLToy_Model::AddStripFromIndices( const u_int* puIndices, const u_int uCount )
{
    Append( new GLToy_ModelStrip( puIndices, uCount ) );

    UpdateStripPointers();
}

void GLToy_Model::Render() const
{
    GLToy_ConstIterate( GLToy_ModelStrip, xIterator, this )
    {
        xIterator.Current().Render();
    }
}

void GLToy_Model::RenderWithPositionAndOrientation( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation ) const
{
    GLToy_Render::SetIdentityViewMatrix();
    GLToy_Render::Transform( -xOrientation );
    GLToy_Render::Translate( -xPosition );

    Render();
}

void GLToy_Model::UpdateStripPointers()
{
    GLToy_Iterate( GLToy_ModelStrip, xIterator, this )
    {
        xIterator.Current().SetVertexPointer( m_pxVertices );
    }
}

u_int GLToy_Model::GetVertexIndex( const GLToy_Vector_3& xVertex )
{
    for( u_int u = 0; u < m_uVertexCount; ++u )
    {
        if( m_pxVertices[ u ] == xVertex )
        {
            return u;
        }
    }

    ++m_uVertexCount;
    const GLToy_Vector_3* const pxOldVertices = m_pxVertices;

    m_pxVertices = new GLToy_Vector_3[ m_uVertexCount ];

    if( pxOldVertices )
    {
        for( u_int u = 0; u < m_uVertexCount - 1; ++u )
        {
            m_pxVertices[ u ] = pxOldVertices[ u ];
        }
        delete[] pxOldVertices;
    }

    m_pxVertices[ m_uVertexCount - 1 ] = xVertex;

    return m_uVertexCount - 1;
}

void GLToy_Model::LoadFromOBJFile( GLToy_Model* const pxModel ,const char* const szFilename )
{
    pxModel->Reset();
    GLToy_ANSITextFile xFile( szFilename );

    const u_int uLength = xFile.GetSize();
    if( uLength <= 1 )
    {
        return;
    }

    char* szData = new char[ uLength ];
    xFile.GetString( szData );

    for( u_int u = 0; szData[ u ]; ++u )
    {
        switch( szData[ u ] )
        {
            case 'f':
            {
                // actually obj supports more complicated stuff than this
                // so this won't always work :/
                if( szData[ u + 1 ] = ' ' )
                {
                    // TODO - read the data
                    pxModel->AddStripFromIndices( NULL, 0 );
                }
            }

            case 'v':
            {
                if( szData[ u + 1 ] = ' ' )
                {
                    float fComponents[ 3 ];
                    sscanf( &( szData[ u ] ), "v %f %f %f", &( fComponents[ 0 ] ), &( fComponents[ 1 ] ), &( fComponents[ 2 ] ) );
                    pxModel->GetVertexIndex( GLToy_Vector_3( fComponents[ 0 ], fComponents[ 1 ], fComponents[ 2 ] ) );
                }
            }

            default:
            {
                break;
            }
        }
    }

    delete[] szData;
}