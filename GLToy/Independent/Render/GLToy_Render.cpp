/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

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

void GLToy_Render::SubmitVertex3( const float* const pfVertex )
{
	Platform_GLToy_Render::SubmitVertex3( pfVertex );
}

void GLToy_Render::SubmitNormal3( const float* const pfNormal )
{
	Platform_GLToy_Render::SubmitNormal3( pfNormal );
}

void GLToy_Render::SubmitColour3( const float* const pfColour )
{
	Platform_GLToy_Render::SubmitColour3( pfColour );
}

void GLToy_Render::SubmitTextureCoordinate3( const float* const pfTexCoord, const u_int uTextureUnit )
{
	Platform_GLToy_Render::SubmitTextureCoordinate3( pfTexCoord, uTextureUnit );
}