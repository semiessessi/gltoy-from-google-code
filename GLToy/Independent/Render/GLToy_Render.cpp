/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Render::Initialise()
{
    if( !Platform_Initialise() )
    {
        return false;
    }

    if( !Project_Initialise() )
    {
        return false;
    }

    return true;
}

void GLToy_Render::Shutdown()
{
    Project_Shutdown();

    Platform_Shutdown();
}

void GLToy_Render::Render()
{
    Platform_BeginRender();

    Project_Render();

    Platform_EndRender();
}

bool GLToy_Render::Platform_Initialise()
{
    return Platform_GLToy_Render::Initialise();
}

void GLToy_Render::Platform_Shutdown()
{
    Platform_GLToy_Render::Shutdown();
}

void GLToy_Render::Platform_BeginRender()
{
    Platform_GLToy_Render::BeginRender();
}

void GLToy_Render::Platform_EndRender()
{
    Platform_GLToy_Render::EndRender();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Render::Translate( const GLToy_Vector_3& xTranslation )
{
    Platform_GLToy_Render::Translate( xTranslation );
}

void GLToy_Render::Rotate( const GLToy_Vector_3& xAxis, const float fAngle )
{
    Platform_GLToy_Render::Rotate( xAxis, fAngle );
}

void GLToy_Render::StartSubmittingTriangles()
{
    Platform_GLToy_Render::StartSubmittingTriangles();
}

void GLToy_Render::StartSubmittingQuads()
{
    Platform_GLToy_Render::StartSubmittingQuads();
}

void GLToy_Render::StartSubmittingTriangleStrip()
{
    Platform_GLToy_Render::StartSubmittingTriangleStrip();
}

void GLToy_Render::StartSubmittingTriangleFan()
{
    Platform_GLToy_Render::StartSubmittingTriangleFan();
}

void GLToy_Render::StartSubmittingPolygon()
{
    Platform_GLToy_Render::StartSubmittingPolygon();
}

void GLToy_Render::EndSubmit()
{
    Platform_GLToy_Render::EndSubmit();
}

void GLToy_Render::SubmitVertex( const GLToy_Vector_3& xVertex )
{
    Platform_GLToy_Render::SubmitVertex( xVertex );
}

void GLToy_Render::SubmitNormal( const GLToy_Vector_3& xNormal )
{
    Platform_GLToy_Render::SubmitNormal( xNormal );
}

void GLToy_Render::SubmitColour( const GLToy_Vector_3& xColour )
{
    Platform_GLToy_Render::SubmitColour( xColour );
}

void GLToy_Render::SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit )
{
    Platform_GLToy_Render::SubmitTextureCoordinate( xTexCoord, uTextureUnit );
}