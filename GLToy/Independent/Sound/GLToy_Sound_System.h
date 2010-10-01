/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

#ifndef __GLTOY_SOUND_SYSTEM_H_
#define __GLTOY_SOUND_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_Array;
template < class T > class GLToy_HashTree;
class GLToy_Vector_3;
class GLToy_Sound_Source;
class GLToy_SoundFile;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

enum GLToy_Sound_Transition
{
    GLToy_SOUND_CUT,
    GLToy_SOUND_WAIT,
    GLToy_SOUND_FADE
};

class GLToy_Sound_System
{

    friend class GLToy_Sound;
    friend class GLToy_WaveFile;

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();

    static GLToy_Handle CreateSource( const GLToy_Hash uHash, const GLToy_Vector_3& xPosition, const bool bRelative = false, const bool bLooped = false );
    static GLToy_Handle PlayMusic( const GLToy_Hash uHash, const GLToy_Sound_Transition eTransitionType = GLToy_SOUND_CUT );

    static void Stop( const GLToy_Handle iHandle, const GLToy_Sound_Transition eTransitionType = GLToy_SOUND_CUT );

private:

    static void TestSound_Console( const GLToy_String& szName );

    static GLToy_Handle CreateSoundHandle();
    static GLToy_Handle CreateSourceHandle();

    static void DestroySoundHandle( const GLToy_Handle iHandle );
    static void DestroySourceHandle( const GLToy_Handle iHandle );

    static GLToy_Sound* LoadSound( const GLToy_Hash uHash );

    static GLToy_HashTree< GLToy_SoundFile* > s_xSounds;
    static GLToy_Array< GLToy_Sound_Source* > s_xSources;

};

#endif
