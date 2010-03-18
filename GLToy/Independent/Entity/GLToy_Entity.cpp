/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Compression/GLToy_Compression.h>
#include <Core/Data Structures/GLToy_BitStream.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Entity::ReadFromBitStream( const GLToy_BitStream& xStream )
{   
    // entity file does this for us
    //u_char ucType;
    //xStream >> ucType;
    //xStream >> m_uHash;
    xStream >> m_bActive;
}

void GLToy_Entity::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    // entity file may load these, but write them here
    // TODO - remove this asymmetry
    xStream << static_cast< u_char >( m_eType );
    xStream << m_uHash;
    xStream << m_bActive;
}

void GLToy_Entity_Oriented_AABB::RenderAABB() const
{
    m_xBoundingBox.Render();
}

void GLToy_Entity_Oriented_AABB::RenderOBB() const
{
    RenderAABB();
}

void GLToy_Entity_Oriented_AABB::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    u_int uOri = 0;
    xStream >> m_xBoundingBox;
    xStream >> uOri;

    SetOrientation( GLToy_Decompress::OrthonormalMatrix_4Bytes( uOri ) );
}

void GLToy_Entity_Oriented_AABB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xBoundingBox;
    xStream << GLToy_Compress::OrthonormalMatrix_4Bytes( GetOrientation() );
}