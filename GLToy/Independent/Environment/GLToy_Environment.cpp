/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/GLToy_Environment.h>

// GLToy
#include <Core/Data Structures/GLToy_Bitstream.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uCURRENT_VERSION = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Environment::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    u_int uVersion;

    xStream >> uVersion;

    if( uVersion > uCURRENT_VERSION )
    {
        GLToy_Assert( false, "Bad version for environment data: %u", uVersion );
        return;
    }
}

void GLToy_Environment::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << uCURRENT_VERSION;
}

void GLToy_Environment::Render() const
{
    // draw everything front to back to help minimise overdraw
    DistanceSortedLeafTraverse( &GLToy_RenderFunctor< GLToy_EnvironmentLeaf >::GetInstance(), GLToy_Camera::GetPosition() );
}

void GLToy_Environment::Update()
{

}