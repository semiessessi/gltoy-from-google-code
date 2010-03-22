/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Maths/GLToy_Plane.h>

// GLToy
#include <Compression/GLToy_Compression.h>
#include <Core/Data Structures/GLToy_BitStream.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Plane::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    u_short usNormal;

    xStream >> usNormal;
    xStream >> m_fDistance;

    m_xNormal = GLToy_Decompress::UnitVector_2Bytes( usNormal );
}

void GLToy_Plane::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << GLToy_Compress::UnitVector_2Bytes( m_xNormal );
    xStream << m_fDistance;
}