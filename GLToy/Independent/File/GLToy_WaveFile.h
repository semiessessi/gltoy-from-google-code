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

#ifndef __GLTOY_WAVEFILE_H_
#define __GLTOY_WAVEFILE_H_

#ifndef GLTOY_DEMO

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <File/GLToy_File.h>

// CRT
#include <stdio.h>

// SE - THIS NEEDS TO GO - IT BREAKS PLATFORMNESS...!!!

// Windows
#include <windows.h>

// SE - ugly hack to fix GLToy_Final_SCU
#undef CreateDialog

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_WaveFile : public GLToy_File
{

	public:
    
		GLToy_WaveFile( const GLToy_String& szFilename );
		virtual ~GLToy_WaveFile();
		
		bool Load();

		const WAVEFORMATEX& GetWaveFormat();
		void GetBuffer( void*& pBuffer, unsigned int& uBufferSize );

	protected:

		bool ParseChunk( FILE* pxFile );

		u_int m_uBufferSize;
		void* m_pBuffer;

		WAVEFORMATEX m_xFormat;
};

#endif

#endif