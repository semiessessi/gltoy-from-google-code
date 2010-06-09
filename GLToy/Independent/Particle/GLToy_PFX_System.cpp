/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

#include <Core/GLToy.h>

// This file's header
#include <Particle/GLToy_PFX_System.h>

// GLToy
#include <Core/GLToy_UpdateFunctor.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_ANSITextFile.h>
#include <File/GLToy_File_System.h>
#include <Particle/GLToy_ParticleSource.h>
#include <Particle/GLToy_PFX.h>
#include <Render/GLToy_RenderFunctor.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_PFX* > GLToy_PFX_System::s_xPFX;
GLToy_HashTree< GLToy_PFXProperties > GLToy_PFX_System::s_xPFXProperties;
GLToy_HashTree< GLToy_ParticleSourceProperties > GLToy_PFX_System::s_xSourceProperties;
GLToy_HashTree< GLToy_ParticleProperties > GLToy_PFX_System::s_xParticleProperties;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_PFX_System::Initialise()
{
    if( !InitialisePFXProperties() )
    {
        return false;
    }

    if( !InitialiseSourceProperties() )
    {
        return false;
    }

    if( !InitialiseParticleProperties() )
    {
        return false;
    }

    return true;
}

void GLToy_PFX_System::Shutdown()
{
    s_xPFX.DeleteAll();
    s_xPFXProperties.Clear();
    s_xSourceProperties.Clear();
    s_xParticleProperties.Clear();
}

void GLToy_PFX_System::Render()
{
    s_xPFX.Traverse( GLToy_PointerRenderFunctor< GLToy_PFX* >() );
}

void GLToy_PFX_System::Update()
{
    s_xPFX.Traverse( GLToy_PointerUpdateFunctor< GLToy_PFX* >() );

    // remove PFX as soon as we can
    for( u_int u = 0; u < s_xPFX.GetCount(); ++u )
    {
        if( s_xPFX[ u ]->IsDone() )
        {
            GLToy_PFX* const pxPFX = s_xPFX[ u ];
            s_xPFX.Remove( pxPFX->GetHash() );
            delete pxPFX;
            u = 0;
        }
    }
}

GLToy_PFX* GLToy_PFX_System::CreatePFX( const GLToy_Hash uHash, const GLToy_Vector_3& xPosition )
{
    if( uHash == uGLTOY_BAD_HASH )
    {
        return NULL;
    }

    const GLToy_PFXProperties* const pxProperties = s_xPFXProperties.FindData( uHash );
    if( !pxProperties )
    {
        return NULL;
    }

    static u_int uCount = 0;
    ++uCount;
    GLToy_String szName;
    szName.SetToFormatString( "PFX%d", uCount );

    GLToy_PFX* const pxPFX = new GLToy_PFX( szName.GetHash(), xPosition, pxProperties->m_fLifetime );

    // create sources
    GLToy_ConstIterate( GLToy_Hash, xIterator, pxProperties )
    {
        const GLToy_ParticleSourceProperties* const pxSourceProperties = s_xSourceProperties.FindData( xIterator.Current() );
        if( pxSourceProperties )
        {
            GLToy_ParticleSource* const pxSource = new GLToy_ParticleSource( *pxSourceProperties, pxPFX );
            pxPFX->AddSource( pxSource );
        }
    }

    s_xPFX.AddNode( pxPFX, pxPFX->GetHash() );

    return pxPFX;
}

void GLToy_PFX_System::DestroyPFX( const GLToy_Hash uPFXHash )
{
    GLToy_PFX** ppxPFX = s_xPFX.FindData( uPFXHash );
    if( ppxPFX )
    {
        s_xPFX.Remove( uPFXHash );
        delete *ppxPFX;
    }
}

const GLToy_ParticleProperties* GLToy_PFX_System::GetParticleProperties( const GLToy_Hash uHash )
{
    return s_xParticleProperties.FindData( uHash );
}

bool GLToy_PFX_System::InitialisePFXProperties()
{
    s_xPFXProperties.Clear();

    GLToy_Array< GLToy_String > xPFXPaths = GLToy_File_System::PathsFromFilter( "PFX/", "*.pfx" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xPFXPaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 4 ); // remove "PFX/"
        szName.RemoveFromEnd( 4 ); // remove ".pfx"

        GLToy_DebugOutput( "   - Found PFX \"%S\".\r\n", szName.GetWideString() );
        
        GLToy_ANSITextFile xFile( xIterator.Current() );

        GLToy_String xData = xFile.GetString();

        GLToy_PFXProperties xPFXProperties;
        while( xData.GetLength() > 0 )
        {
            GLToy_String szValue = xData.RemoveFirstLine();
            if( szValue.Contains( L'=' ) )
            {
                GLToy_String szKey = szValue.RemoveUpTo( L'=' );
                if( szValue[ 0 ] == L'=' )
                {
                    szKey.RemoveAt( 0 );
                }

                if( szKey == "NewSource" )
                {
                    xPFXProperties.Append( szValue.GetHash() );
                }
                else if( szKey == "Lifetime" )
                {
                    xPFXProperties.m_fLifetime = szValue.ExtractFloat();
                }
            }
        }

        s_xPFXProperties.AddNode( xPFXProperties, szName.GetHash() );
    }

    return true;
}

bool GLToy_PFX_System::InitialiseSourceProperties()
{
    s_xSourceProperties.Clear();

    GLToy_Array< GLToy_String > xSourcePaths = GLToy_File_System::PathsFromFilter( "PFX/Sources/", "*.source" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xSourcePaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 12 ); // remove "PFX/Sources/"
        szName.RemoveFromEnd( 7 ); // remove ".source"

        GLToy_DebugOutput( "   - Found particle source \"%S\".\r\n", szName.GetWideString() );
        
        GLToy_ANSITextFile xFile( xIterator.Current() );

        GLToy_String xData = xFile.GetString();

        GLToy_ParticleSourceProperties xSourceProperties;;
        while( xData.GetLength() > 0 )
        {
            GLToy_String szValue = xData.RemoveFirstLine();
            if( szValue.Contains( L'=' ) )
            {
                GLToy_String szKey = szValue.RemoveUpTo( L'=' );
                if( szValue[ 0 ] == L'=' )
                {
                    szKey.RemoveAt( 0 );
                }

                if( szKey == "ReleaseRate" )
                {
                    xSourceProperties.m_fReleaseRate = szValue.ExtractFloat();
                }
                else if( szKey == "Lifetime" )
                {
                    xSourceProperties.m_fLifetime = szValue.ExtractFloat();
                }
                else if( szKey == "Particle" )
                {
                    xSourceProperties.m_uParticleHash = szValue.GetHash();
                }
            }
        }

        s_xSourceProperties.AddNode( xSourceProperties, szName.GetHash() );
    }

    return true;
}

bool GLToy_PFX_System::InitialiseParticleProperties()
{
    s_xParticleProperties.Clear();

    GLToy_Array< GLToy_String > xParticlePaths = GLToy_File_System::PathsFromFilter( "PFX/Particles/", "*.particle" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xParticlePaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 14 ); // remove "PFX/Particles/"
        szName.RemoveFromEnd( 9 ); // remove ".particle"

        GLToy_DebugOutput( "   - Found particle \"%S\".\r\n", szName.GetWideString() );
        
        GLToy_ANSITextFile xFile( xIterator.Current() );

        GLToy_String xData = xFile.GetString();

        GLToy_ParticleProperties xParticleProperties;
        while( xData.GetLength() > 0 )
        {
            GLToy_String szValue = xData.RemoveFirstLine();
            if( szValue.Contains( L'=' ) )
            {
                GLToy_String szKey = szValue.RemoveUpTo( L'=' );
                if( szValue[ 0 ] == L'=' )
                {
                    szKey.RemoveAt( 0 );
                }

                if( szKey == "Texture" )
                {
                    xParticleProperties.m_uTextureHash = szValue.GetHash();
                }
                else if( szKey == "Size" )
                {
                    xParticleProperties.m_fSize = szValue.ExtractFloat();
                }
                else if( szKey == "Lifetime" )
                {
                    xParticleProperties.m_fLifetime = szValue.ExtractFloat();
                }
                else if( szKey == "GrowthRate" )
                {
                    xParticleProperties.m_fGrowthRate = szValue.ExtractFloat();
                }
                else if( szKey == "FadeTime" )
                {
                    xParticleProperties.m_fFadeTime = szValue.ExtractFloat();
                }
                else if( szKey == "RotationRate" )
                {
                    xParticleProperties.m_fRotationRate = szValue.ExtractFloat();
                }
                else if( szKey == "RandomAngle" )
                {
                    xParticleProperties.m_bRandomAngle = !szValue.MeansFalse();
                }
            }
        }

        s_xParticleProperties.AddNode( xParticleProperties, szName.GetHash() );
    }

    return true;
}
