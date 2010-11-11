
// Thomas Young

#ifndef _SRC_ENVIRONMENT_
#define _SRC_ENVIRONMENT_

#include "Environment/GLToy_Environment_Plane.h"

// Dear Semi, the word Environment is very long to type, I do not enjoy having to type it a lot
// in future please use a nice short word like 'World' or 'Scene', thank you - Tom

// Dear Tom - Ctrl-Space (not Tab :P) is your friend but point taken - world/scene would probably be a lot clearer

class GLToy_Plane;

static const u_int uSRC_ENVIRONMENT_TYPE = 0xDEADBEEF;

class SRC_Environment : public GLToy_Environment_Plane
{
	typedef GLToy_Environment_Plane GLToy_Parent;

	public:

		SRC_Environment( const GLToy_Plane& xPlane, const GLToy_String& szTextureName );
		~SRC_Environment();

		virtual void Initialise();

		virtual void Update();
		virtual void Render() const;
		
		virtual void Shutdown();

		virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
		virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

		virtual u_int GetType() const;

		float Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const;
};

GLToy_Environment* SRC_CreateEnvironment( const u_int uType );

#endif

// eof
