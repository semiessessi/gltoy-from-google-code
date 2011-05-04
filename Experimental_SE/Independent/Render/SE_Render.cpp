/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/SE.h>

// This file's header
#include <Render/SE_Render.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Render/GLToy_Camera.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

// Experimental SE
#include <Render/SE_Star_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

SE_Star_Render* g_pxStarRender = NULL;

static void Console_TestStar( const GLToy_String& szString )
{
    g_pxStarRender = new SE_Star_Render( szString, GLToy_Vector_3( 0.0f, 0.0f, 1000000000.0f ) );
}

bool SE_Render::Initialise()
{
    GLToy_Camera::SetSpeed( 10000000.0f );
    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "SE_Star" ) );
	if( pxShader )
	{
		pxShader->SetUseNoise( true );
	}

    GLToy_Console::RegisterCommand( "test.star", Console_TestStar );

    // 1 billion km away...
    g_pxStarRender = new SE_Star_Render( "O4", GLToy_Vector_3( 0.0f, 0.0f, 1000000000.0f ) );
    return true;
}

void SE_Render::Shutdown()
{
    delete g_pxStarRender;
    g_pxStarRender = NULL;
}

void SE_Render::Render()
{
    if( g_pxStarRender )
    {
        g_pxStarRender->Render();
    }
}
