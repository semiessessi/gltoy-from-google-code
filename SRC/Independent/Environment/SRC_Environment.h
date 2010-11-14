
// Thomas Young

#ifndef _SRC_ENVIRONMENT_
#define _SRC_ENVIRONMENT_

#include "Environment/GLToy_Environment_Plane.h"

#include "Maths/GLToy_Vector.h"

// Dear Semi, the word Environment is very long to type, I do not enjoy having to type it a lot
// in future please use a nice short word like 'World' or 'Scene', thank you - Tom

// Dear Tom - Ctrl-Space (not Tab :P) is your friend but point taken - world/scene would probably be a lot clearer

class GLToy_Plane;

static const u_int uSRC_ENVIRONMENT_TYPE = 0xDEADBEEF;

static const u_int uSRC_ENV_BLOCKS = 16;  // Blocks in map ( length and breadth )
// static const u_int uSRC_ENV_BLOCK_SIZE = 16;

static const float fSRC_ENV_VERY_LOW = -256.0f;
static const float fSRC_ENV_MIN_BLOCK_HEIGHT = -64.0f;

class SRC_Map_Block
{
	// These are the blocks which make up a level

	public:

		SRC_Map_Block();
		~SRC_Map_Block();

		void Update();
		void Render() const;

		void SetHeight( float fHeight );
		void SetActive( bool bActive );
		void SetPosition( GLToy_Vector_2 xPosition );

	private:

		bool m_bActive;
		GLToy_Vector_3 m_xMax;
		GLToy_Vector_3 m_xMin;
};

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
		
		virtual bool Load( const GLToy_String& xName );

		virtual u_int GetType() const;

		float Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const;

	private:

		void CreateEnv();
		void ClearEnv();
		SRC_Map_Block* GetBlock( int iX, int iY );
		
		SRC_Map_Block** m_pxBlocks; // TODO: Use propper data structure

		GLToy_String m_xFilename;
};

GLToy_Environment* SRC_CreateEnvironment( const u_int uType );

#endif

// eof
