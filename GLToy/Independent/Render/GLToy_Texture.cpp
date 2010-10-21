/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

// This file's header
#include <Render/GLToy_Texture.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_File_System.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_Texture > GLToy_Texture_System::s_xTextures;
GLToy_Texture* GLToy_Texture_System::s_pxBlackTexture = NULL;
GLToy_Texture* GLToy_Texture_System::s_pxWhiteTexture = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture::LoadFromFile()
{
    if( IsDataLoaded() ) // already loaded?
    {
        return;
    }

    Unload();

    if( m_szName.EndsWith( ".tga" ) )
    {
        LoadTGA();
        return;
    }

    // TODO - load .wal
    if( m_szName.EndsWith( ".wal" ) )
    {
        GLToy_UI_System::ShowErrorDialog( ".wal textures are not yet supported" );
        return;
    }

    Platform_LoadFromFile();
}

void GLToy_Texture::Create()
{
    if( IsReadyForUse() )
    {
        return;
    }

    if( !IsDataLoaded() )
    {
        LoadFromFile();
    }

    Platform_Create();

    Unload(); // texture created, might as well free memory..
}

void GLToy_Texture::Destroy()
{
    Platform_Destroy();

    m_iID = -1;
}

void GLToy_Texture::Bind( const u_int uTextureUnit ) const
{
    GLToy_Assert( IsReadyForUse(), "Texture \"%S\" being bound before it has been created", m_szName.GetWideString() );

    Platform_Bind( uTextureUnit );
}

void GLToy_Texture::Unload()
{
    Clear();
}

bool GLToy_Texture_System::Initialise()
{
    if( !Platform_Initialise() )
    {
        return false;
    }

    s_xTextures.Clear();

    s_xTextures.AddNode( GLToy_Texture( "white", 0xFFFFFFFF ), GLToy_Hash_Constant( "white" ) );
    s_pxWhiteTexture = s_xTextures.FindData( GLToy_Hash_Constant( "white" ) );
    s_xTextures.AddNode( GLToy_Texture( "black", 0xFF000000 ), GLToy_Hash_Constant( "black" ) );
    s_pxBlackTexture = s_xTextures.FindData( GLToy_Hash_Constant( "black" ) );
    s_xTextures.AddNode( GLToy_Texture( "transparent", 0x00000000 ), GLToy_Hash_Constant( "transparent" ) );

    CreateTexture( "white" );
    CreateTexture( "black" );
    CreateTexture( "transparent" );

    GLToy_Array< GLToy_String > xTexturePaths = GLToy_File_System::PathsFromFilter( "Textures/", "*.bmp" );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.jpg" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.pcx" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.png" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.tga" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.wal" ) );

    GLToy_ConstIterate( GLToy_String, xIterator, &xTexturePaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 9 ); // remove "textures/"
        
        // this got annoying...
        // GLToy_DebugOutput( "   - Found texture \"%S\".\r\n", szName.GetWideString() );

        s_xTextures.AddNode( GLToy_Texture( szName ), szName.GetHash() );
    }

    return true;
}

void GLToy_Texture_System::Shutdown()
{
	GLToy_QuickFunctorInstance(
		TextureShutdownFunctor,
		GLToy_Texture,
		pxTexture,
		if( pxTexture->IsReadyForUse() )
        {
            pxTexture->Destroy();
        }

        if( pxTexture->IsDataLoaded() )
        {
            pxTexture->Unload();
        },
		xFunctor );

    s_xTextures.Traverse( xFunctor );

    s_xTextures.Clear();

    Platform_Shutdown();
}

GLToy_Texture* GLToy_Texture_System::FindTexture( const GLToy_Hash uHash )
{
    return s_xTextures.FindData( uHash );
}

GLToy_Texture* GLToy_Texture_System::LookUpTexture( const GLToy_String& szName )
{    
    return FindTexture( szName.GetHash() );
}

GLToy_Texture* GLToy_Texture_System::LookUpTextureNoExt( const GLToy_String& szName )
{    
    GLToy_Texture* pxTexture = FindTexture( szName.GetHash() );

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".wal" );
    }

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".tga" );
    }

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".png" );
    }

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".jpg" );
    }

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".bmp" );
    }

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".pcx" );
    }

    if( !pxTexture )
    {
        pxTexture = LookUpTexture( szName + ".ptx" );
    }

    return pxTexture;
}

void GLToy_Texture_System::CreateTexture( const GLToy_String& szName )
{
    CreateTexture( szName.GetHash() );
}

void GLToy_Texture_System::CreateTexture( const GLToy_Hash uHash )
{
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( pxTexture )
    {
        if( !pxTexture->IsDataLoaded() )
        {
            pxTexture->LoadFromFile();
        }

        if( !pxTexture->IsReadyForUse() )
        {
            pxTexture->Create();
        }
    }
}

void GLToy_Texture_System::DestroyTexture( const GLToy_String& szName )
{
    DestroyTexture( szName.GetHash() );
}

void GLToy_Texture_System::DestroyTexture( const GLToy_Hash uHash )
{
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( pxTexture && pxTexture->IsReadyForUse() )
    {
        pxTexture->Destroy();
    }
}

void GLToy_Texture_System::BindTexture( const GLToy_String& szName, const u_int uTextureUnit )
{
    BindTexture( szName.GetHash() );
}

void GLToy_Texture_System::BindTexture( const GLToy_Hash uHash, const u_int uTextureUnit )
{
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( pxTexture )
    {
        if( !pxTexture->IsReadyForUse() )
        {
            pxTexture->Create();
        }

        pxTexture->Bind( uTextureUnit );
    }
    else
    {
        pxTexture = FindTexture( GLToy_Hash_Constant( "Generic/Invalid.png" ) );
        if( pxTexture )
        {
            if( !pxTexture->IsReadyForUse() )
            {
                pxTexture->Create();
            }

            pxTexture->Bind( uTextureUnit );
        }
    }
}

void GLToy_Texture_System::Reset()
{
    GLToy_Iterate( GLToy_Texture, xIterator, &s_xTextures )
    {
        xIterator.Current().Destroy();
        xIterator.Current().Unload();
    }
}

void GLToy_Texture_System::BindBlack( const u_int uTextureUnit )
{
    if( s_pxBlackTexture )
    {
        s_pxBlackTexture->Bind( uTextureUnit );
    }
}

void GLToy_Texture_System::BindWhite( const u_int uTextureUnit )
{
    if( s_pxWhiteTexture )
    {
        s_pxWhiteTexture->Bind( uTextureUnit );
    }
}

void GLToy_Texture_System::CreateTextureFromRGBAData( const GLToy_Hash uHash, u_int *const puData, const u_int uWidth, const u_int uHeight )
{
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( !pxTexture )
    {
        // texture doesn't already exist so create it
        s_xTextures.AddNode( GLToy_Texture( "" ), uHash );
        pxTexture = FindTexture( uHash );
    }

    GLToy_Assert( pxTexture != NULL, "Something went really wrong and we have a null pointer where we shoudln't!" );

    const u_int uSize = uWidth * uHeight;
    pxTexture->Resize( uSize );

    for( u_int u = 0; u < uSize; ++u )
    {
        ( *pxTexture )[ u ] = puData[ u ];
    }

    pxTexture->m_uWidth = uWidth;
    pxTexture->m_uHeight = uHeight;
}

void GLToy_Texture_System::CreateTextureFromRGBData( const GLToy_Hash uHash, u_char *const pucData, const u_int uWidth, const u_int uHeight )
{
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( !pxTexture )
    {
        // texture doesn't already exist so create it
        s_xTextures.AddNode( GLToy_Texture( "" ), uHash );
        pxTexture = FindTexture( uHash );
    }

    GLToy_Assert( pxTexture != NULL, "Something went really wrong and we have a null pointer where we shoudln't!" );

    const u_int uSize = uWidth * uHeight;
    pxTexture->Resize( uSize );

    for( u_int u = 0; u < uSize; ++u )
    {
        ( *pxTexture )[ u ] = pucData[ 3 * u ] | ( static_cast< u_int >( pucData[ 3 * u + 1 ] ) << 8 ) | ( static_cast< u_int >( pucData[ 3 * u + 2 ] ) << 16 ) | 0xFF000000;
    }

    pxTexture->m_uWidth = uWidth;
    pxTexture->m_uHeight = uHeight;
}
