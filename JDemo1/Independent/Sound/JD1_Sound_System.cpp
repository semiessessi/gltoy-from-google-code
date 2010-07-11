/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2009, 2010 Semi Essessi
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

#include <Core/JD1.h>

// This file's headers
#include <Sound/JD1_Sound_System.h>
#include <Sound/Platform_JD1_Sound_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_File_System.h>

// JD1
#include <File/JD1_SoundFile.h>
#include <File/JD1_WaveFile.h>
#include <Sound/JD1_Sound.h>
#include <Sound/JD1_Sound_Source.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< JD1_SoundFile* > JD1_Sound_System::s_xSounds;
GLToy_Array< JD1_Sound_Source* > JD1_Sound_System::s_xSources;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool JD1_Sound_System::Initialise()
{
    GLToy_Console::RegisterCommand( "test.sound", TestSound_Console );

	s_xSounds.Clear(); // really? GLToy_Model_System etc. do this as well
	s_xSources.Clear();

    GLToy_Array< GLToy_String > xWavePaths = GLToy_File_System::PathsFromFilter( "Sounds/", "*.wav" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xWavePaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 7 ); // remove "Sounds/"
        xName.RemoveFromEnd( 4 ); // remove .wav
        
        GLToy_DebugOutput( "   - Found sound \"%S\".\r\n", xName.GetWideString() );

        s_xSounds.AddNode( new JD1_WaveFile( xIterator.Current() ), xName.GetHash() );
    }

	return Platform_JD1_Sound_System::Initialise();
}

void JD1_Sound_System::Shutdown()
{
	Platform_JD1_Sound_System::Shutdown();

	s_xSounds.DeleteAll();
	s_xSources.DeleteAll();
}

void JD1_Sound_System::Update()
{
	Platform_JD1_Sound_System::Update();
}

GLToy_Handle JD1_Sound_System::CreateSource( const GLToy_Hash uHash, const GLToy_Vector_3& xPosition, const bool bRelative, const bool bLooped )
{
    return uGLTOY_INVALID_HANDLE;
}

GLToy_Handle JD1_Sound_System::PlayMusic( const GLToy_Hash uHash, const JD1_Sound_Transition eTransitionType )
{
    return uGLTOY_INVALID_HANDLE;
}

void JD1_Sound_System::Stop( const GLToy_Handle iHandle, const JD1_Sound_Transition eTransitionType )
{
}

JD1_Sound* JD1_Sound_System::LoadSound( const GLToy_Hash uHash )
{
    JD1_SoundFile** ppxSoundFile = s_xSounds.FindData( uHash );
    if( !ppxSoundFile )
    {
        return NULL;
    }

    return (*ppxSoundFile)->LoadSound();
}

GLToy_Handle JD1_Sound_System::CreateSoundHandle()
{
    return Platform_JD1_Sound_System::CreateSoundHandle();
}

GLToy_Handle JD1_Sound_System::CreateSourceHandle()
{
    return Platform_JD1_Sound_System::CreateSourceHandle();
}

void JD1_Sound_System::DestroySoundHandle( const GLToy_Handle iHandle )
{
    Platform_JD1_Sound_System::DestroySoundHandle( iHandle );
}

void JD1_Sound_System::DestroySourceHandle( const GLToy_Handle iHandle )
{
    Platform_JD1_Sound_System::DestroySourceHandle( iHandle );
}

void JD1_Sound_System::TestSound_Console( const GLToy_String& szName )
{
    const JD1_Sound* const pxSound = LoadSound( szName.GetHash() );

    if( !pxSound )
    {
        return;
    }

    Platform_JD1_Sound_System::TestSound( pxSound->GetHandle() );
}
