/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/RTToy_Environment_Plane.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void RTToy_Environment_Plane::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPlane;
    xStream >> m_uTextureHash;
}

void RTToy_Environment_Plane::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPlane;
    xStream << m_uTextureHash;
}

void RTToy_Environment_Plane::Render() const
{
    GLToy_Render::StartSubmittingQuads();



    GLToy_Render::EndSubmit();
}

void RTToy_Environment_Plane::Update()
{
}