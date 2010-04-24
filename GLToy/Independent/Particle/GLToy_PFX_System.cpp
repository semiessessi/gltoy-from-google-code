/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Particle/GLToy_PFX_System.h>

// GLToy
#include <Core/GLToy_UpdateFunctor.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_ANSITextFile.h>
#include <File/GLToy_File_System.h>
#include <Particle/GLToy_Particle.h>
#include <Particle/GLToy_ParticleSource.h>
#include <Particle/GLToy_PFX.h>
#include <Render/GLToy_RenderFunctor.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_List< GLToy_PFX* > GLToy_PFX_System::s_xPFX;
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
}

void GLToy_PFX_System::Render()
{
    s_xPFX.Traverse( GLToy_PointerRenderFunctor< GLToy_PFX* >() );
}

void GLToy_PFX_System::Update()
{
    s_xPFX.Traverse( GLToy_PointerUpdateFunctor< GLToy_PFX* >() );
}

u_int GLToy_PFX_System::CreatePFX( const GLToy_Hash uHash )
{
    return uGLTOY_BAD_PFXID;
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
                    // TODO - float
                    xSourceProperties.m_fReleaseRate = static_cast< float >( szValue.ExtractUnsignedInt() );
                }
                else if( szKey == "Particle" )
                {
                    // TODO - float
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

                //if( szKey == "NewSource" )
                //{
                //    xParticleProperties.Append( szValue.GetHash() );
                //}
            }
        }

        s_xParticleProperties.AddNode( xParticleProperties, szName.GetHash() );
    }

    return true;
}