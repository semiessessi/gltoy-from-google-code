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

// This file's headers
#include <Sound/GLToy_Sound_System.h>
#include <Sound/Platform_GLToy_Sound_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Sound_System::Initialise()
{
	return Platform_GLToy_Sound_System::Initialise();
}

void GLToy_Sound_System::Update()
{
	Platform_GLToy_Sound_System::Update();
}

void GLToy_Sound_System::Shutdown()
{
	Platform_GLToy_Sound_System::Shutdown();
}

GLToy_Handle GLToy_Sound_System::CreateVoice( const GLToy_Hash uWaveHash )
{
	return Platform_GLToy_Sound_System::CreateVoice( uWaveHash );
}

GLToy_Sound_Voice* GLToy_Sound_System::GetVoice( const GLToy_Handle iHandle )
{
	return Platform_GLToy_Sound_System::GetVoice( iHandle );
}

void GLToy_Sound_System::DestroyVoice( const GLToy_Handle iHandle )
{
	return Platform_GLToy_Sound_System::DestroyVoice( iHandle );
}

//eof
