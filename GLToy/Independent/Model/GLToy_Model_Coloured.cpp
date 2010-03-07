/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model_Coloured.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ModelStrip_Coloured::GLToy_ModelStrip_Coloured()
: PARENT()
, m_xColour( 1.0f, 1.0f, 1.0f )
{
}

GLToy_ModelStrip_Coloured::GLToy_ModelStrip_Coloured( const GLToy_ModelStrip_Coloured& xStrip )
: PARENT( xStrip )
, m_xColour( xStrip.m_xColour )
{
}

GLToy_ModelStrip_Coloured::GLToy_ModelStrip_Coloured( u_int uVertex1, u_int uVertex2, u_int uVertex3, const GLToy_Vector_3& xColour )
: PARENT( uVertex1, uVertex2, uVertex3 )
, m_xColour( xColour )
{
}

GLToy_ModelStrip_Coloured::GLToy_ModelStrip_Coloured( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4, const GLToy_Vector_3& xColour )
: PARENT( uVertex1, uVertex2, uVertex3, uVertex4 )
, m_xColour( xColour )
{
}

GLToy_ModelStrip_Coloured::~GLToy_ModelStrip_Coloured()
{
}

GLToy_ModelStrip_Coloured& GLToy_ModelStrip_Coloured::operator =( const GLToy_ModelStrip_Coloured& xStrip )
{
    PARENT::operator =( xStrip );
    m_xColour = xStrip.m_xColour;

    return *this;
}

void GLToy_ModelStrip_Coloured::Render() const
{
    GLToy_Render::StartSubmittingTriangleStrip();
    
    GLToy_Render::SubmitColour( m_xColour );

    SubmitVertices();
    
    GLToy_Render::EndSubmit();
}

GLToy_Model_Coloured::GLToy_Model_Coloured()
: PARENT()
{
}

GLToy_Model_Coloured::~GLToy_Model_Coloured()
{
}

void GLToy_Model_Coloured::AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xColour )
{
    const u_int uVertex1 = GetVertexIndex( xVertex1 );
    const u_int uVertex2 = GetVertexIndex( xVertex2 );
    const u_int uVertex3 = GetVertexIndex( xVertex3 );

    Append( new GLToy_ModelStrip_Coloured( uVertex1, uVertex2, uVertex3, xColour ) );

    UpdateStripPointers();
}

void GLToy_Model_Coloured::AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4, const GLToy_Vector_3& xColour )
{
    const u_int uVertex1 = GetVertexIndex( xVertex1 );
    const u_int uVertex2 = GetVertexIndex( xVertex2 );
    const u_int uVertex3 = GetVertexIndex( xVertex3 );
    const u_int uVertex4 = GetVertexIndex( xVertex4 );

    Append( new GLToy_ModelStrip_Coloured( uVertex1, uVertex2, uVertex3, uVertex4, xColour ) );

    UpdateStripPointers();
}