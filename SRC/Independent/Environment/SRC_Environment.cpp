
// Thomas Young

#include "Core/GLToy.h"

#include "SRC_Environment.h"

#include "Maths/GLToy_Plane.h"

SRC_Environment::SRC_Environment( const GLToy_Plane& xPlane, const GLToy_String& szTextureName )
: GLToy_Environment_Plane( xPlane, szTextureName ) 
{
}

SRC_Environment::~SRC_Environment()
{
}

void SRC_Environment::Initialise()
{
	GLToy_Parent::Initialise();
}

void SRC_Environment::Update()
{
	GLToy_Parent::Update();
}

void SRC_Environment::Render() const
{
	GLToy_Parent::Render();
}
		
void SRC_Environment::Shutdown()
{
	GLToy_Parent::Shutdown();
}

u_int SRC_Environment::GetType() const
{
	return uSRC_ENVIRONMENT_TYPE;
}

void SRC_Environment::ReadFromBitStream( const GLToy_BitStream& xStream )
{
	GLToy_Parent::ReadFromBitStream( xStream );
}

void SRC_Environment::WriteToBitStream( GLToy_BitStream& xStream ) const
{
	GLToy_Parent::WriteToBitStream( xStream );
}

float SRC_Environment::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
	// TODO: Cut a square out of a plane and make it the correct dimensions for the map blocks
	// also, make this use the depth buffer if it does not already

    // SE - 11/11/10 - just to make clear this is the trace function as in Quake, rather than raytraced rendering
    // at the moment its not that important - although it would be good to have for the editor etc. to trace the mouse to a point

	return GLToy_Parent::Trace( xRay, fLimitingDistance );
}

GLToy_Environment* SRC_CreateEnvironment( const u_int uType )
{
    if( uType == uSRC_ENVIRONMENT_TYPE )
    {
        return new SRC_Environment( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "Generic/Grid1.ptx" );
    }

    return NULL;
}