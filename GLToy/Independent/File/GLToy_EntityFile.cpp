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
#include <File/GLToy_EntityFile.h>

// GLToy
#include <Entity/GLToy_Entity.h>
#include <Entity/GLToy_Entity_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EntitySaveFunctor
: public GLToy_ConstFunctor< GLToy_Entity* >
{

public:

    GLToy_EntitySaveFunctor( GLToy_BitStream& xBitStream )
    : m_xStream( xBitStream )
    {
    }

    void operator()( GLToy_Entity* const* const pxEntity )
    {
        ( *pxEntity )->WriteToBitStream( m_xStream );
    }

protected:

    GLToy_BitStream& m_xStream;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EntityFile::LoadEntities() const
{
    GLToy_BitStream xStream;
    ReadToBitStream( xStream );

    if( xStream.GetBitsWritten() == 0 )
    {
        GLToy_Assert( xStream.GetBitsWritten() != 0, "No data in entity file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }

    u_int uHeader;
    xStream >> uHeader;

    if( uHeader != GLToy_HeaderBytes( "ENTS" ) )
    {
        GLToy_Assert( uHeader == GLToy_HeaderBytes( "ENTS" ), "Wrong header for entity file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }

    GLToy_Entity_System::DestroyEntities();

    while( xStream.HasNMoreBytes( 5 ) )
    {
        u_char ucType;
        GLToy_Hash uHash;

        xStream >> ucType;
        xStream >> uHash;

        GLToy_Entity* pxEntity = GLToy_Entity_System::CreateEntity( uHash, static_cast< GLToy_EntityType >( ucType ) );

        if( pxEntity )
        {
            pxEntity->ReadFromBitStream( xStream );
        }
        else
        {
            GLToy_Assert( pxEntity != NULL, "Fatal error whilst loading entity file \"%S\" - can not continue processing stream", GetFilename().GetWideString() );
            return;
        }
    }

    GLToy_DebugOutput_Release( "Loaded entity file \"%S\" successfully", m_szFilename.GetWideString() );
}

void GLToy_EntityFile::Save( const GLToy_String& szFilename )
{
    GLToy_BitStream xStream;

    xStream << GLToy_HeaderBytes( "ENTS" );

    GLToy_Entity_System::Traverse( GLToy_EntitySaveFunctor( xStream ) );

    GLToy_EntityFile( szFilename ).WriteFromBitStream( xStream );

    GLToy_DebugOutput_Release( "Saved entity file \"%S\" successfully", szFilename.GetWideString() );
}
