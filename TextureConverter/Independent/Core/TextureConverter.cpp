/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#include <Core/GLToy.h>

// GLToy
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

#include <Render/GLToy_Texture_Procedural.h>

// C
#include <stdio.h>

// External
#include "../../../stb_image/stb_image.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_PLATFORM_WIN32
    #pragma comment( lib, "GLToy" )
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

int GLToy_ForceCDecl main( const int iArgumentCount, const char* const* const pszArguments )
{
    //GLToy::SilentEntryPoint();

    printf( "GLToy Texture Converter\r\n" );

    // parse parameters
    bool bShowHelp = false;
    bool bGenerateNormalMap = true;//( iArgumentCount == 3 );

    if( iArgumentCount <= 2 )
    {
        bShowHelp = true;
    }

    if( bShowHelp )
    {
        printf( "Usage:\r\n" );
        printf( "  TextureConverter <input-file> <output-file>\r\n" );
        printf( "\r\n" );
        printf( "By default this will try to convert a heightmap to a normal map...\r\nmore options to come later\r\n" );

        return 0;
    }

    const char* const szInPath = pszArguments[ iArgumentCount - 2 ];
    const char* const szOutPath = pszArguments[ iArgumentCount - 1 ];

    printf( "Loading input texture %s...\r\n", szInPath );

    u_int uWidth = 0;
    u_int uHeight = 0;
    int iComponents = 0;
    u_char* pucData = stbi_load(
        szInPath,
        reinterpret_cast< int* >( &uWidth ),
        reinterpret_cast< int* >( &uHeight ),
        &iComponents,
        STBI_rgb_alpha );

    GLToy_Texture_Procedural xTexture;

    if( bGenerateNormalMap )
    {
        printf( "Generating a normal map...\r\n", szInPath );
        xTexture.AppendHeightmapToNormals();
    }

    u_int* puNewData = xTexture.CreateRGBAFromBaseTexture( reinterpret_cast< u_int* >( pucData ), uWidth, uHeight );

    stbi_image_free( pucData );

    printf( "Writing output file %s...\r\n", szOutPath );

    GLToy_Texture_System::SaveTextureTGA( szOutPath, puNewData, uWidth, uHeight );

    delete[] puNewData;

    printf( "Done.\r\n\r\n" );

    //GLToy::SilentShutdown();

    return 0;
}

bool GLToy::Project_Initialise()
{
	return true;
}

void GLToy::Project_Shutdown()
{
}

void GLToy::Project_Update()
{
}

bool GLToy_Render::Project_Initialise()
{
	return true;
}

void GLToy_Render::Project_Shutdown()
{
}

void GLToy_Render::Project_Render()
{
}