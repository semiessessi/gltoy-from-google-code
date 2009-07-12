#ifndef __GLTOY_RENDER_H_
#define __GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_3;

class GLToy_Render
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();

    // GL interface
    static void SetViewport( int iX, int iY, u_int uWidth, u_int uHeight );

    static void SetIdentityProjectionMatrix();
    static void SetPerspectiveProjectionMatrix( u_int uViewportWidth, u_int uViewportHeight );
    static void SetOrthogonalProjectionMatrix();

    static void SetIdentityViewMatrix();
    static void SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp );

    static void Translate( const GLToy_Vector_3& xTranslation );
    static void Rotate( const GLToy_Vector_3& xAxis, const float fAngle );

    static void StartSubmittingTriangles();
    static void StartSubmittingQuads();
    static void StartSubmittingTriangleStrip();
    static void StartSubmittingTriangleFan();
    static void StartSubmittingPolygon();
    static void EndSubmit();

    static void SubmitVertex( const GLToy_Vector_3& xVertex );
    static void SubmitNormal( const GLToy_Vector_3& xNormal );
    static void SubmitColour( const GLToy_Vector_3& xColour );
    static void SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit );

    static void Flush();

private:

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Render();

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_BeginRender();
    static void Platform_EndRender();

    static float s_fFOV;

};

#endif