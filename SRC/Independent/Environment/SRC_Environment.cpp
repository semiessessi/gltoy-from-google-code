
// Thomas Young

#include "Core/GLToy.h"

#include "SRC_Environment.h"

#include "Core/GLToy_Timer.h"
#include "Maths/GLToy_Plane.h"
#include "Render/GLToy_Render.h"
#include "Render/GLToy_Texture.h"

// ________________________________ SRC_Map_Block __________________________________

SRC_Map_Block::SRC_Map_Block()
: m_bActive( true )
{
}

SRC_Map_Block::~SRC_Map_Block()
{
}

void SRC_Map_Block::SetHeight( float fHeight )
{
	m_xMax[1] = fHeight;
}

void SRC_Map_Block::SetActive( bool bActive )
{
	m_bActive = bActive;
}

void SRC_Map_Block::SetPosition( GLToy_Vector_2 xPosition )
{
	m_xMin[0] = xPosition[0];
	m_xMin[1] = fSRC_ENV_VERY_LOW; // SE - this doesn't get read anywhere?
	m_xMin[2] = xPosition[1];
	
	m_xMax[0] = xPosition[0] + 1.0f;
	m_xMax[1] = 0.0f;
	m_xMax[2] = xPosition[1] + 1.0f;

    m_xMin *= 64.0f;
    m_xMax *= 64.0f;
}

void SRC_Map_Block::Update()
{
}

void SRC_Map_Block::Render() const
{
	if( !m_bActive )
	{
		return;
	}

	GLToy_Texture_System::BindTexture( "Generic/Grid2.png" );

    GLToy_Render::StartSubmittingQuads();
	{
		// Walls

		// Top bit

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], m_xMax[ 1 ], m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], m_xMax[ 1 ], m_xMax[ 2 ] ) );
		
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], m_xMax[ 1 ], m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], m_xMax[ 1 ], m_xMin[ 2 ] ) );

		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], m_xMax[ 1 ], m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], m_xMax[ 1 ], m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], m_xMax[ 1 ], m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, m_xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], m_xMax[ 1 ], m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );

		// Fade

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_VERY_LOW, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_VERY_LOW, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_VERY_LOW, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_VERY_LOW, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_VERY_LOW, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], fSRC_ENV_VERY_LOW, m_xMax[ 2 ] ) );
		
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_VERY_LOW, m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], fSRC_ENV_VERY_LOW, m_xMax[ 2 ] ) );
	}
    GLToy_Render::EndSubmit();

	// Top

	GLToy_Texture_System::BindTexture( "Generic/Grid3.png" );

    GLToy_Render::StartSubmittingQuads();
	{
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], m_xMax[ 1 ], m_xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMax[ 0 ], m_xMax[ 1 ], m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], m_xMax[ 1 ], m_xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( m_xMin[ 0 ], m_xMax[ 1 ], m_xMax[ 2 ] ) );
		
	}
	GLToy_Render::EndSubmit();
}

// ________________________________ SRC_Environment __________________________________

SRC_Environment::SRC_Environment( const GLToy_Plane& xPlane, const GLToy_String& szTextureName )
: GLToy_Environment_Plane( xPlane, szTextureName ) 
, m_pxBlocks( 0 )
{
}

SRC_Environment::~SRC_Environment()
{
}

void SRC_Environment::Initialise()
{
	GLToy_Parent::Initialise();

	CreateEnv();
}

void SRC_Environment::CreateEnv()
{
	GLToy_Assert( !m_pxBlocks, "Blocks pointer should not be set" );

	m_pxBlocks = new SRC_Map_Block*[ uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS ];
	memset( m_pxBlocks, 0, sizeof( SRC_Map_Block* ) * uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS );

	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		m_pxBlocks[iBlock] = new SRC_Map_Block();

		const float fX = static_cast<float>( iBlock % uSRC_ENV_BLOCKS );
		const float fY = static_cast<float>( iBlock / uSRC_ENV_BLOCKS );

		m_pxBlocks[iBlock]->SetPosition( GLToy_Vector_2( fX, fY ) );

		// TODO: Remove hack

		m_pxBlocks[iBlock]->SetHeight( GLToy_Maths::Floor( GLToy_Maths::Random( 0.0f, 1.0f ) * 3.0f ) );
		m_pxBlocks[iBlock]->SetActive( GLToy_Maths::Random( 0.0f, 1.0f ) > 0.3f );
	}
}

SRC_Map_Block* SRC_Environment::GetBlock( int iX, int iY )
{
	if( !m_pxBlocks )
	{
		return 0;
	}

	return ( m_pxBlocks[0] + ( iY * uSRC_ENV_BLOCKS + iX % uSRC_ENV_BLOCKS ) );
}

void SRC_Environment::Update()
{
	GLToy_Parent::Update();
	
	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		m_pxBlocks[iBlock]->Update();
	}
}

void SRC_Environment::Render() const
{
	if( !m_pxBlocks )
	{
		return;
	}

	GLToy_Render::EnableBlending();
    
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		m_pxBlocks[iBlock]->Render();
	}

	GLToy_Render::EnableDepthWrites();
}
		
void SRC_Environment::Shutdown()
{
	GLToy_Parent::Shutdown();

	ClearEnv();
}

void SRC_Environment::ClearEnv()
{
	if( !m_pxBlocks )
	{
		return;
	}

	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		delete m_pxBlocks[iBlock];
	}
	delete []m_pxBlocks;
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

bool SRC_Environment::Load( const GLToy_String& xName )
{
	return false;
}