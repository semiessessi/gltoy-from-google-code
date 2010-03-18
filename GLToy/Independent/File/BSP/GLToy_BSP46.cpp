/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_EnvironmentFile.h>

// GLToy
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_System.h>

// TODO - finish BSP v46 environment geometry loading

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BSP46_Lump
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_uOffset; xStream >> m_uSize; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_uOffset; xStream << m_uSize; }

    u_int m_uOffset;
    u_int m_uSize;

};

class GLToy_BSP46_LumpDirectory
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { for( u_int u = 0; u < 17; ++u ) { xStream >> m_xLumps[ u ]; } }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { for( u_int u = 0; u < 17; ++u ) { xStream << m_xLumps[ u ]; } }


    GLToy_BSP46_Lump m_xLumps[ 17 ];

};


/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EnvironmentFile::LoadBSP46( const GLToy_BitStream& xStream ) const
{
}