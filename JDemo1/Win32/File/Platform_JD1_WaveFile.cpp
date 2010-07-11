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

#include <Core/GLToy.h>

// This file's header
#include <File/JD1_WaveFile.h>

// OpenAL
#include <al/alut.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_WaveFile::Platform_LoadSound( const GLToy_Handle iHandle ) const
{
    if( !GLToy_IsValidHandle( iHandle ) )
    {
        return;
    }
    
    ALenum eFormat;
    ALvoid* pData = NULL;
    ALsizei iSize = 0;
    ALsizei iFrequency = 0;
    ALboolean bLoop = AL_FALSE;
    char* const szFilename = GetFilename().CreateANSIString();

    // TODO: alutLoadWAVFile and alutUnloadWAV are deprecated...
    alutLoadWAVFile( szFilename, &eFormat, &pData, &iSize, &iFrequency, &bLoop );

    delete[] szFilename;

    const union
    {
        GLToy_Handle i;
        ALuint u;
    } xHandle = { iHandle };

    alBufferData( xHandle.u, eFormat, pData, iSize, iFrequency );
    alutUnloadWAV( eFormat, pData, iSize, iFrequency );
}