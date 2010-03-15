#ifndef __GLTOY_RENDER_H_
#define __GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define BLEND_ZERO                           0
#define BLEND_ONE                            1
#define BLEND_SRC_COLOR                      0x0300
#define BLEND_ONE_MINUS_SRC_COLOR            0x0301
#define BLEND_SRC_ALPHA                      0x0302
#define BLEND_ONE_MINUS_SRC_ALPHA            0x0303
#define BLEND_DST_ALPHA                      0x0304
#define BLEND_ONE_MINUS_DST_ALPHA            0x0305

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_3;
class GLToy_Renderable;
template < class T > class GLToy_RenderFunctor;
class GLToy_Vector_3;
class GLToy_Vector_4;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Render
{

public:

    static const GLToy_RenderFunctor< GLToy_Renderable > RenderableFunctor;

    static float GetFOV() { return s_fFOV; }

    static bool Initialise();
    static void Shutdown();

    static void BeginRender();
    static void Render();
    static void EndRender();

    // GL interface
    static u_int GetError();

    static void SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight );

    static void SetIdentityProjectionMatrix();
    static void SetPerspectiveProjectionMatrix( const u_int uViewportWidth, const u_int uViewportHeight );
    static void SetOrthogonalProjectionMatrix();

    static void SetIdentityViewMatrix();
    static void SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp );

    static void Translate( const GLToy_Vector_3& xTranslation );
    static void Rotate( const GLToy_Vector_3& xAxis, const float fAngle );
    static void Transform( const GLToy_Matrix_3& xMatrix );

    static void PushViewMatrix();
    static void PopViewMatrix();

    static void SubmitTexturedQuad2D( const float fXMin, const float fYMin, const float fXMax, const float fYMax, const float fUMin = 0.0f, const float fVMin = 0.0f, const float fUMax = 1.0f, const float fVMax = 1.0f );

    static void StartSubmittingTriangles();
    static void StartSubmittingQuads();
    static void StartSubmittingTriangleStrip();
    static void StartSubmittingTriangleFan();
    static void StartSubmittingPolygon();
    static void EndSubmit();

    static void SubmitVertex( const GLToy_Vector_3& xVertex );
    static void SubmitNormal( const GLToy_Vector_3& xNormal );
    static void SubmitColour( const GLToy_Vector_3& xColour );
    static void SubmitColour( const GLToy_Vector_4& xColour );
    static void SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit = 0 );

    static void Flush();

    static void DisableBlending();
    static void EnableBlending();
    static void SetBlendFunction( const u_int uSourceBlend, const u_int uDestinationBlend );

    static void DisableBackFaceCulling();
    static void EnableBackFaceCulling();
    static void SetCCWFaceWinding();
    static void SetCWFaceWinding();

    // shader functions
    static bool IsShader( const u_int uID );
    static u_int CreateFragmentShader();
    static u_int CreateVertexShader();
    static u_int CreateProgram();
    static void DeleteShader( u_int uShaderID );
    static void DeleteProgram( u_int uProgramID );
    static void ValidateProgram( u_int uProgramID );
    static void CompileShader( u_int uShaderID );
    static void LinkProgram( u_int uProgramID );
    static void UseProgram( u_int uProgramID );
    static void AttachShader( u_int uProgramID, u_int uShaderID );
    static void DetachShader( u_int uProgramID, u_int uShaderID );
    static void ShaderSource( u_int uShaderID, int iStringCount, char** ppszStrings, const int* piLengths );
    static void GetProgramInfoLog( u_int uProgramID, int iMaxLength, int* iLength, char* szInfoLog );
    static void GetShaderInfoLog( u_int uShaderID, int iMaxLength, int* iLength, char* szInfoLog );
    static u_int GetUniformID( u_int uProgramID, const char* szName );
    static u_int GetAttributeID( u_int uProgramID, const char* szName );
    static void BindAttributeID( u_int uProgramID, u_int uIndex, const char* szName );
    static void SetUniform( u_int uUniformID, int iValue );
    static void SetUniform( u_int uUniformID, int iValue1, int iValue2 );
    static void SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3 );
    static void SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 );
    static void SetUniform( u_int uUniformID, float fValue );
    static void SetUniform( u_int uUniformID, float fValue1, float fValue2 );
    static void SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3 );
    static void SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 );
    static void SetAttribute( u_int uAttributeID, int iValue );
    static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2 );
    static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3 );
    static void SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 );
    static void SetAttribute( u_int uAttributeID, float fValue );
    static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2 );
    static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3 );
    static void SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 );

private:

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Render();

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_BeginRender();
    static void Platform_EndRender();

    static float s_fFOV;
    static bool s_bDrawFPS;

};

#endif