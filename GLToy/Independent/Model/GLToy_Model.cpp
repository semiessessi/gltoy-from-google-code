/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model.h>

// GLToy
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uGLTOY_MODEL_BADINDEX = 0xFFFFFFFF;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_Strip::GLToy_Model_Strip()
: m_uVertexCount( 0 )
, m_puVertexIndices( NULL )
, m_pxVertices( NULL )
{
}

GLToy_Model_Strip::GLToy_Model_Strip( const GLToy_Model_Strip& xStrip )
: m_uVertexCount( xStrip.m_uVertexCount )
, m_puVertexIndices( NULL )
, m_pxVertices( NULL )
{
	if( m_uVertexCount > 0 )
	{
		m_puVertexIndices = new u_int[m_uVertexCount];
		for( u_int u = 0; u < m_uVertexCount; ++u )
		{
			m_puVertexIndices[u] = xStrip.m_puVertexIndices[u];
		}
	}
}

GLToy_Model_Strip::GLToy_Model_Strip( u_int uVertex1, u_int uVertex2, u_int uVertex3 )
: m_uVertexCount( 3 )
, m_puVertexIndices( NULL )
, m_pxVertices( NULL )
{
	m_puVertexIndices = new u_int[3];
	m_puVertexIndices[0] = uVertex1;
	m_puVertexIndices[1] = uVertex2;
	m_puVertexIndices[2] = uVertex3;
}

GLToy_Model_Strip::GLToy_Model_Strip( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4 )
: m_uVertexCount( 4 )
, m_puVertexIndices( NULL )
, m_pxVertices( NULL )
{
	m_puVertexIndices = new u_int[4];
	m_puVertexIndices[0] = uVertex1;
	m_puVertexIndices[1] = uVertex2;
	m_puVertexIndices[2] = uVertex3;
	m_puVertexIndices[3] = uVertex4;
}

GLToy_Model_Strip::~GLToy_Model_Strip()
{
	if( m_puVertexIndices )
	{
		delete m_puVertexIndices;
		m_puVertexIndices = NULL;
	}
}

GLToy_Model_Strip& GLToy_Model_Strip::operator =( const GLToy_Model_Strip& xStrip )
{
	if( m_puVertexIndices )
	{
		delete m_puVertexIndices;
		m_puVertexIndices = NULL;
	}

	m_uVertexCount = xStrip.m_uVertexCount;

	if( m_uVertexCount > 0 )
	{
		m_puVertexIndices = new u_int[m_uVertexCount];
		for( u_int u = 0; u < m_uVertexCount; ++u )
		{
			m_puVertexIndices[u] = xStrip.m_puVertexIndices[u];
		}
	}

	return *this;
}

void GLToy_Model_Strip::Render()
{
	GLToy_Render::StartSubmittingTriangleStrip();
	
	for( u_int u = 0; u < m_uVertexCount; ++u )
	{
		GLToy_Render::SubmitVertex( m_pxVertices[m_puVertexIndices[u]] );
	}
	
	GLToy_Render::EndSubmit();
}

GLToy_Model::GLToy_Model()
: m_uVertexCount( 0 )
, m_pxVertices( NULL )
, m_uStripCount( 0 )
, m_pxStrips( NULL )
{
}

GLToy_Model::~GLToy_Model()
{
	if( m_pxVertices )
	{
		delete m_pxVertices;
		m_pxVertices = NULL;
	}

	if( m_pxStrips )
	{
		delete m_pxStrips;
		m_pxStrips = NULL;
	}
}

void GLToy_Model::AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 )
{
	++m_uStripCount;
	GLToy_Model_Strip* pxOldStrips = m_pxStrips;

	m_pxStrips = new GLToy_Model_Strip[m_uStripCount];

	if( pxOldStrips )
	{
		for( u_int u = 0; u < m_uStripCount; ++u )
		{
			m_pxStrips[u] = pxOldStrips[u];
		}
		delete pxOldStrips;
	}

	u_int uVertex1 = GetVertexIndex( xVertex1 );
	u_int uVertex2 = GetVertexIndex( xVertex2 );
	u_int uVertex3 = GetVertexIndex( xVertex3 );

	m_pxStrips[m_uStripCount - 1] = GLToy_Model_Strip( uVertex1, uVertex2, uVertex3 );

	UpdateStripPointers();
}

void GLToy_Model::AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 )
{
	++m_uStripCount;
	const GLToy_Model_Strip* const pxOldStrips = m_pxStrips;

	m_pxStrips = new GLToy_Model_Strip[m_uStripCount];

	if( pxOldStrips )
	{
		for( u_int u = 0; u < m_uStripCount; ++u )
		{
			m_pxStrips[u] = pxOldStrips[u];
		}
		delete pxOldStrips;
	}

	u_int uVertex1 = GetVertexIndex( xVertex1 );
	u_int uVertex2 = GetVertexIndex( xVertex2 );
	u_int uVertex3 = GetVertexIndex( xVertex3 );
	u_int uVertex4 = GetVertexIndex( xVertex4 );

	m_pxStrips[m_uStripCount - 1] = GLToy_Model_Strip( uVertex1, uVertex2, uVertex3, uVertex4 );

	UpdateStripPointers();
}

void GLToy_Model::Render()
{
	for( u_int u = 0; u < m_uStripCount; ++u )
	{
		m_pxStrips[u].Render();
	}
}

void GLToy_Model::UpdateStripPointers()
{
	for( u_int u = 0; u < m_uStripCount; ++u )
	{
		m_pxStrips[u].SetVertexPointer( m_pxVertices );
	}
}

u_int GLToy_Model::GetVertexIndex( const GLToy_Vector_3& xVertex )
{
	for( u_int u = 0; u < m_uVertexCount; ++u )
	{
		if( m_pxVertices[u] == xVertex )
		{
			return u;
		}
	}

	++m_uVertexCount;
	const GLToy_Vector_3* const pxOldVertices = m_pxVertices;

	m_pxVertices = new GLToy_Vector_3[m_uVertexCount];

	if( pxOldVertices )
	{
		for( u_int u = 0; u < m_uVertexCount; ++u )
		{
			m_pxVertices[u] = pxOldVertices[u];
		}
		delete pxOldVertices;
	}

	return m_uVertexCount - 1;
}