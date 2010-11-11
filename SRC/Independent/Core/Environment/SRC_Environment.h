
// Thomas Young

#ifndef _SRC_ENVIRONMENT_
#define _SRC_ENVIRONMENT_

#include "Environment/GLToy_Environment_Plane.h"

// Dear Semi, the word Environment is very long to type, I do not enjoy having to type it a lot
// in future please use a nice short word like 'World' or 'Scene', thank you - Tom

class GLToy_Plane;

class SRC_Environment : public GLToy_Environment_Plane
{
	typedef GLToy_Environment_Plane GLToy_Parent;

	public:

		SRC_Environment( const GLToy_Plane& xPlane, const GLToy_String& szTextureName );
		~SRC_Environment();

		void Initialise();

		void Update();
		void Render();
		
		void Shutdown();

		void ReadFromBitStream( const GLToy_BitStream& xStream );
		void WriteToBitStream( GLToy_BitStream& xStream ) const;

		int GetType() const;

		float Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const;		
};

#endif

// eof
