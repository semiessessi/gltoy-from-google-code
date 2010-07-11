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

#include <Core/JD1.h>

// This file's header
#include <Sound/Platform_JD1_Sound_System.h>

// GLToy
#include <Render/GLToy_Camera.h>

// OpenAL
#include <al.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

union JD1_Sound_HandleUnion
{
    ALuint u;
    GLToy_Handle i;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_JD1_Sound_System::Initialise()
{
	return true;
}

void Platform_JD1_Sound_System::Shutdown()
{
}

void Platform_JD1_Sound_System::Update()
{
    alListenerfv( AL_POSITION, GLToy_Camera::GetPosition().GetFloatPointer() );
    alListenerfv( AL_VELOCITY, GLToy_Maths::ZeroVector3.GetFloatPointer() ); // TODO: some kind of velocity would be nice...
    
    const GLToy_Vector_3& xDirection = GLToy_Camera::GetDirection();
    const GLToy_Vector_3& xUp = GLToy_Camera::GetUp();
    const float afOrientation[ 6 ] =
    {
        xDirection[ 0 ], xDirection[ 1 ], xDirection[ 2 ],
        xUp[ 0 ], xUp[ 1 ], xUp[ 2 ]
    };

    alListenerfv( AL_ORIENTATION, afOrientation );
}

GLToy_Handle Platform_JD1_Sound_System::CreateSoundHandle()
{
    JD1_Sound_HandleUnion xBuffer;

    alGenBuffers( 1, &(xBuffer.u) );

    return xBuffer.i;
}

GLToy_Handle Platform_JD1_Sound_System::CreateSourceHandle()
{
    JD1_Sound_HandleUnion xSource;

    alGenSources( 1, &(xSource.u) );

    return xSource.i;
}

void Platform_JD1_Sound_System::DestroySoundHandle( const GLToy_Handle iHandle )
{
    if( !GLToy_IsValidHandle( iHandle ) )
    {
        GLToy_Assert( GLToy_IsValidHandle( iHandle ), "Bad handle!" );
        return;
    }

    alDeleteBuffers( 1, reinterpret_cast< const ALuint* >( &iHandle ) );
}

void Platform_JD1_Sound_System::DestroySourceHandle( const GLToy_Handle iHandle )
{
    if( !GLToy_IsValidHandle( iHandle ) )
    {
        GLToy_Assert( GLToy_IsValidHandle( iHandle ), "Bad handle!" );
        return;
    }

    alDeleteSources( 1, reinterpret_cast< const ALuint* >( &iHandle ) );
}