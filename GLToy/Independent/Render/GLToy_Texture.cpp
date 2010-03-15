/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Texture.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_Texture > GLToy_Texture_System::s_xTextures;
GLToy_Texture* GLToy_Texture_System::s_pxWhiteTexture = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture::LoadFromFile()
{
    Unload();
    Platform_LoadFromFile();
}

void GLToy_Texture::Create()
{
    GLToy_Assert( IsDataLoaded(), "Texture \"%S\" being created before it has been loaded", m_szName.GetWideString() );

    Platform_Create();
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
    m_uWidth = m_uHeight = 0;
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
    s_xTextures.AddNode( GLToy_Texture( "transparent", 0x00000000 ), GLToy_Hash_Constant( "transparent" ) );

    CreateTexture( "white" );
    CreateTexture( "black" );
    CreateTexture( "transparent" );

    GLToy_Array< GLToy_String > xTexturePaths = GLToy_File_System::PathsFromFilter( "Textures/", "*.bmp" );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.jpg" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.pcx" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.png" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.tga" ) );

    GLToy_ConstIterate( GLToy_String, xIterator, &xTexturePaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 9 ); // remove "textures/"
        
        GLToy_DebugOutput( "   - Found texture \"%S\".\r\n", szName.GetWideString() );

        s_xTextures.AddNode( GLToy_Texture( szName ), szName.GetHash() );
    }

    return true;
}

void GLToy_Texture_System::Shutdown()
{
    class GLToy_TextureShutdownFunctor
    : public GLToy_Functor< GLToy_Texture >
    {
    public:
        void operator ()( GLToy_Texture* xTexture )
        {
            if( xTexture->IsReadyForUse() )
            {
                xTexture->Destroy();
            }

            if( xTexture->IsDataLoaded() )
            {
                xTexture->Unload();
            }
        }
    };

    s_xTextures.Traverse( GLToy_TextureShutdownFunctor() );

    s_xTextures.Clear();

    Platform_Shutdown();
}

GLToy_Texture* GLToy_Texture_System::FindTexture( const GLToy_Hash uHash )
{
    return s_xTextures.FindData( uHash );
}

GLToy_Texture* GLToy_Texture_System::LookUpTexture( const GLToy_String& szName )
{
    GLToy_Texture* pxTexture = s_xTextures.FindData( szName.GetHash() );
    GLToy_Assert( pxTexture != NULL, "Failed to look up texture \"%S\"", szName.GetWideString() );
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

        pxTexture->Create();
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

void GLToy_Texture_System::BindTexture( const GLToy_String& szName )
{
    GLToy_Texture* pxTexture = LookUpTexture( szName );
    if( pxTexture && pxTexture->IsReadyForUse() )
    {
        pxTexture->Bind();
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

void GLToy_Texture_System::BindWhite()
{
    if( s_pxWhiteTexture )
    {
        s_pxWhiteTexture->Bind();
    }
}