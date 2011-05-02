
/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Core/Solus.h"							// Main header

#include "Render/Solus_Render.h"				// My header

#include "Render/GLToy_Camera.h"				// GLToy
#include "Render/GLToy_Render.h"
#include "Material/GLToy_Material_System.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Solus_Render::Initialise()
{
	return true;
}

void Solus_Render::Shutdown()
{
}

void Solus_Render::Render()
{
	GLToy_Camera::SetPosition( GLToy_Vector_3( 0.0f, 0.0f, -1.0f ) );
	GLToy_Camera::SetOrientation( GLToy_Maths::IdentityMatrix3 );

	GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "UI/Splash.png" ) );

	GLToy_Render::StartSubmittingQuads();

	GLToy_Vector_3 xQuadPos( 0.0f, 0.0f, 0.0f );
	const float fQuadHalfSize = 1.0f;

	GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xQuadPos.x - fQuadHalfSize, xQuadPos.y + fQuadHalfSize, xQuadPos.z ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitVertex(xQuadPos.x + fQuadHalfSize, xQuadPos.y + fQuadHalfSize, xQuadPos.z ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
	GLToy_Render::SubmitVertex( xQuadPos.x + fQuadHalfSize, xQuadPos.y - fQuadHalfSize, xQuadPos.z );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
	GLToy_Render::SubmitVertex( xQuadPos.x - fQuadHalfSize, xQuadPos.y - fQuadHalfSize, xQuadPos.z );
	
	GLToy_Render::EndSubmit();
}

// eof
