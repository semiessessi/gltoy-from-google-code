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
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Environment::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    // environment file does this for us...
    // TODO - remove the asymmetry
    //u_int uVersion;

    //xStream >> uVersion;

    //if( uVersion > uCURRENT_VERSION )
    //{
    //    GLToy_Assert( uVersion <= uCURRENT_VERSION, "Bad version for environment data: %u", uVersion );
    //    return;
    //}
}

void GLToy_Environment::WriteToBitStream( GLToy_BitStream& xStream ) const
{
}

void GLToy_Environment::Render() const
{
    // draw everything front to back to help minimise overdraw
    DistanceSortedLeafTraverse( GLToy_RenderFunctor< GLToy_EnvironmentLeaf >(), GLToy_Camera::GetPosition() );
}

void GLToy_Environment::Update()
{

}