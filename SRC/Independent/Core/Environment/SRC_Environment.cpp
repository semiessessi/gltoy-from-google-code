
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

void SRC_Environment::Render()
{
	GLToy_Parent::Render();
}
		
void SRC_Environment::Shutdown()
{
	GLToy_Parent::Shutdown();
}

int SRC_Environment::GetType() const
{
	// huh? return a 'type', the enumeration for which is in engine code o_O
	return 0xDeadBeef;
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

	return GLToy_Parent::Trace(xRay, fLimitingDistance);
}