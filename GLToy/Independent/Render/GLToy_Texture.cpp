/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Texture.h>

// GLToy
#include <Core/Data Structures/GLToy_BinaryTree.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_BinaryTree< GLToy_Texture, GLToy_Hash > GLToy_Texture_System::s_xTextures;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture::LoadFromFile( const GLToy_String& szFilename )
{
    Unload();
    Platform_LoadFromFile( szFilename );
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

    return true;
}

void GLToy_Texture_System::Shutdown()
{
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
        pxTexture->Create();
    }
}

void GLToy_Texture_System::DestroyTexture( const GLToy_String& xName )
{
    GLToy_Texture* pxTexture = LookUpTexture( xName );
    if( pxTexture )
    {
        pxTexture->Destroy();
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