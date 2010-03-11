/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Texture.h>

// GLToy
#include <Core/Data Structures/GLToy_BinaryTree.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_BinaryTree< GLToy_Texture, GLToy_Hash > GLToy_Texture_System::s_xTextures;

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
    Platform_Create();
}

void GLToy_Texture::Destroy()
{
    Platform_Destroy();

    m_iID = -1;
}

void GLToy_Texture::Bind( const u_int uTextureUnit ) const
{
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
    s_xTextures.AddNode( GLToy_Texture( "black", 0xFF000000 ), GLToy_Hash_Constant( "black" ) );
    s_xTextures.AddNode( GLToy_Texture( "transparent", 0x00000000 ), GLToy_Hash_Constant( "transparent" ) );

    CreateTexture( "white" );
    CreateTexture( "black" );
    CreateTexture( "transparent" );

    GLToy_Array< GLToy_String > xTexturePaths = GLToy_File_System::PathsFromFilter( "Textures/", "*.bmp" );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.jpg" ) );
    xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.png" ) );

    GLToy_ConstIterate( GLToy_String, xIterator, &xTexturePaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 9 ); // remove "textures/"
        
        GLToy_DebugOutput( "   - Found texture %S.\r\n", xName.GetWideString() );

        s_xTextures.AddNode( GLToy_Texture( xName ), xName.GetHash() );
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

GLToy_Texture* GLToy_Texture_System::LookUpTexture( const GLToy_String& xName )
{
    return s_xTextures.FindData( xName.GetHash() );
}

void GLToy_Texture_System::CreateTexture( const GLToy_String& xName )
{
    GLToy_Texture* pxTexture = LookUpTexture( xName );
    if( pxTexture )
    {
        if( !pxTexture->IsDataLoaded() )
        {
            pxTexture->LoadFromFile();
        }

        pxTexture->Create();
    }
}

void GLToy_Texture_System::DestroyTexture( const GLToy_String& xName )
{
    GLToy_Texture* pxTexture = LookUpTexture( xName );
    if( pxTexture && pxTexture->IsReadyForUse() )
    {
        pxTexture->Destroy();
    }
}

void GLToy_Texture_System::BindTexture( const GLToy_String& xName )
{
    GLToy_Texture* pxTexture = LookUpTexture( xName );
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