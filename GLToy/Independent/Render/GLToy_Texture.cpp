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
    if( m_uWidth != 0 ) // already loaded?
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
    m_uWidth = m_uHeight = 0;
}

void GLToy_Texture::InitialiseFromData( const u_int* const puData, const u_int uWidth, const u_int uHeight )
{
    if( !puData )
    {
        GLToy_Assert( puData != NULL, "Trying to create texture \"%S\" from NULL data pointer", m_szName.GetWideString() );
    }

    m_uWidth = uWidth;
    m_uHeight = uHeight;

    GLToy_ConstPointerArray< u_int > xData( puData, uWidth * uHeight );
    CopyFrom( &xData );

    Create();
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
	xTexturePaths.Append( GLToy_File_System::PathsFromFilter( "Textures/", "*.wal" ) );

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
        void operator ()( GLToy_Texture* const xTexture )
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

void GLToy_Texture_System::BindTexture( const GLToy_String& szName )
{
    BindTexture( szName.GetHash() );
}

void GLToy_Texture_System::BindTexture( const GLToy_Hash uHash )
{
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( pxTexture )
    {
		if( !pxTexture->IsReadyForUse() )
		{
			pxTexture->Create();
		}

        pxTexture->Bind();
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

            pxTexture->Bind();
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

void GLToy_Texture_System::BindWhite()
{
    if( s_pxWhiteTexture )
    {
        s_pxWhiteTexture->Bind();
    }
}

void GLToy_Texture_System::CreateTextureFromRGBAData( const GLToy_String& szName, u_int *const puData, const u_int uWidth, const u_int uHeight )
{
    const u_int uHash = szName.GetHash();
    GLToy_Texture* pxTexture = FindTexture( uHash );
    if( pxTexture )
    {
        // texture already exists
        return;
    }

    s_xTextures.AddNode( GLToy_Texture( szName ), uHash );

    pxTexture = FindTexture( uHash );

    GLToy_Assert( pxTexture != NULL, "Something went really wrong and we have a null pointer where we shoudln't!" );

    pxTexture->InitialiseFromData( puData, uWidth, uHeight );
}