#ifndef __GLTOY_RENDER_H_
#define __GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Render
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();

	// GL interface
	static void StartSubmittingTriangles();
	static void StartSubmittingQuads();
	static void StartSubmittingTriangleStrip();
	static void StartSubmittingTriangleFan();
	static void StartSubmittingPolygon();
	static void EndSubmit();

	static void SubmitVertex3( const float* const pfVertex );
	static void SubmitNormal3( const float* const pfNormal );
	static void SubmitColour3( const float* const pfColour );
	static void SubmitTextureCoordinate3( const float* const pfTexCoord, const u_int uTextureUnit );

private:

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Render();

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_BeginRender();
    static void Platform_EndRender();

};

#endif