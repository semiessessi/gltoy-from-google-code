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
#include <Render/GLToy_Texture.h>

// C/C++
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ModelStrip::GLToy_ModelStrip()
: GLToy_Parent()
, m_pxVertices( NULL )
{
}

GLToy_ModelStrip::GLToy_ModelStrip( const GLToy_ModelStrip& xStrip )
: GLToy_Parent( xStrip )
, m_pxVertices( xStrip.m_pxVertices )
{
}

GLToy_ModelStrip::GLToy_ModelStrip( u_int uVertex1, u_int uVertex2, u_int uVertex3 )
: GLToy_Parent()
, m_pxVertices( NULL )
{
    Append( uVertex1 );
    Append( uVertex2 );
    Append( uVertex3 );
}

GLToy_ModelStrip::GLToy_ModelStrip( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4 )
: GLToy_Parent()
, m_pxVertices( NULL )
{
    Append( uVertex1 );
    Append( uVertex2 );
    Append( uVertex3 );
    Append( uVertex4 );
}

GLToy_ModelStrip::GLToy_ModelStrip( const u_int* puVertices, const u_int uCount )
: GLToy_Parent()
, m_pxVertices( NULL )
{
    for( u_int u = 0; u < uCount; ++u )
    {
        Append( puVertices[ u ] );
    }
}

GLToy_ModelStrip& GLToy_ModelStrip::operator =( const GLToy_ModelStrip& xStrip )
{
    GLToy_Parent::operator =( xStrip );
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
: GLToy_Parent()
, m_xVertices()
{
}

GLToy_Model::~GLToy_Model()
{
    Reset();
}

void GLToy_Model::Reset()
{
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
    GLToy_Render::UseProgram( 0 );
    GLToy_Texture_System::BindWhite();
    GLToy_ConstIterate( GLToy_ModelStrip, xIterator, this )
    {
        xIterator.Current().Render();
    }
}

void GLToy_Model::RenderWithPositionAndOrientation( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation ) const
{
    GLToy_Render::PushViewMatrix();
    GLToy_Render::Translate( xPosition );
    GLToy_Matrix_3 xInverseOri = xOrientation;
    xInverseOri.Transpose();
    GLToy_Render::Transform( xInverseOri );

    Render();

    GLToy_Render::PopViewMatrix();
}

void GLToy_Model::UpdateStripPointers()
{
    GLToy_Iterate( GLToy_ModelStrip, xIterator, this )
    {
        xIterator.Current().SetVertexPointer( m_xVertices.GetDataPointer() );
    }
}

u_int GLToy_Model::GetVertexIndex( const GLToy_Vector_3& xVertex )
{
    GLToy_ConstIterate( GLToy_Vector_3, xIterator, &m_xVertices )
    {
        if( xIterator.Current() == xVertex )
        {
            return static_cast< u_int >( xIterator.Index() );
        }
    }

    m_xVertices.Append( xVertex );

    if( m_xVertices.GetCount() == 1 )
    {
        SetBBToPoint( xVertex );
    }
    else
    {
        GrowBBByPoint( xVertex );
    }

    return m_xVertices.GetCount() - 1;
}

void GLToy_Model::LoadFromOBJFile( GLToy_Model* const pxModel, const GLToy_String& szFilename )
{
    pxModel->Reset();
    GLToy_ANSITextFile xFile( szFilename );

    const u_int uLength = xFile.GetSize();
    if( uLength <= 1 )
    {
        return;
    }

    char* szData = new char[ uLength ];
    xFile.GetRawString( szData );

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

void GLToy_Model::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xVertices;
    GLToy_Parent::ReadFromBitStream( xStream );
}

void GLToy_Model::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xVertices;
    GLToy_Parent::WriteToBitStream( xStream );
}