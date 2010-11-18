/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi, Thomas Young
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SRC_ENVIRONMENT_
#define _SRC_ENVIRONMENT_

// Parent
#include "Environment/GLToy_Environment.h"

// GLToy
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
, public GLToy_Updateable
{
	// These are the blocks which make up a level

	public:

		SRC_Map_Block();
		~SRC_Map_Block();

		void Update();
		void Render() const;

        void ReadFromBitStream( const GLToy_BitStream& xStream );
		void WriteToBitStream( GLToy_BitStream& xStream ) const;

		void SetHeight( float fHeight );
		void SetActive( bool bActive );
		void SetPosition( GLToy_Vector_2 xPosition );

		float GetHeight() const;
		bool IsActive() const { return m_bActive; }

		bool Editor_IsHighlighted() const;
		void Editor_SetHighlighted( bool bHighlight );
		
		bool Editor_IsSelected() const;
		void Editor_SetSelected( bool bSelected );

	private:

		bool m_bActive;

		bool m_bEditor_Highlighted;
		bool m_bEditor_Selected;
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
