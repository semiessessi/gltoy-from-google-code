
// Thomas Young

#ifndef _SRC_ENVIRONMENT_
#define _SRC_ENVIRONMENT_

// Parent
#include "Environment/GLToy_Environment.h"

// GLToy
#include <Core/GLToy_Serialisable.h>
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

class GLToy_Plane;

static const u_int uSRC_ENVIRONMENT_TYPE = 0xDEADBEEF;
static const GLToy_Hash xSRC_ENV_PHYSICS_HASH = GLToy_GetHash( "SRC_Env" );

static const u_int uSRC_ENV_BLOCKS = 16;  // Blocks in map ( length and breadth )

static const float fSRC_ENV_VERY_LOW = -1024.0f;
static const float fSRC_ENV_MIN_BLOCK_HEIGHT = -64.0f;

class SRC_Map_Block
: public GLToy_Bounded_AABB
, public GLToy_Renderable
, public GLToy_Serialisable
, public GLToy_Updateable
{
	// These are the blocks which make up a level

	public:

		SRC_Map_Block();
		~SRC_Map_Block();

		void Update();
		void Render() const;

        virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
		virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

		void SetHeight( float fHeight );
		void SetActive( bool bActive );
		void SetPosition( GLToy_Vector_2 xPosition );

		float GetHeight() const;
		bool IsActive() const { return m_bActive; }

		bool Editor_IsHighlighted() const;
		void Editor_SetHighlighted( bool bHighlight );

	private:

		bool m_bActive;

		bool m_bEditor_Highlighted;
};

class SRC_Environment : public GLToy_Environment
{
	typedef GLToy_Environment GLToy_Parent;

	public:

		SRC_Environment();
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
		void CreatePhysics();
		void ClearEnv();
		SRC_Map_Block& GetBlock( int iX, int iY );
		
        GLToy_SerialisableArray< SRC_Map_Block > m_xBlocks;
		//SRC_Map_Block** m_pxBlocks; // TODO: Use propper data structure

		GLToy_String m_xFilename;
};

GLToy_Environment* SRC_CreateEnvironment( const u_int uType );

#endif

// eof
