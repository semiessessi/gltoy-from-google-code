/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Entity::ReadFromBitStream( const GLToy_BitStream& xStream )
{   
    u_char ucType;
    xStream >> ucType;
    xStream >> m_bActive;
    xStream >> m_uHash;

    m_eType = static_cast< GLToy_EntityType>( ucType );
}

void GLToy_Entity::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << static_cast< u_char >( m_eType );
    xStream << m_bActive;
    xStream << m_uHash;
}