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

#include <Core/GLToy.h>

// This file's header
#include <Core/GLToy_Assert.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// I M P L E M E N T A T I O N
/////////////////////////////////////////////////////////////////////////////////////////////
// Note: this uses that horrible va_list business
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_GLToy_Assert( const char* szTitle, const char* szMessage )
{
    MessageBoxA( 0, szMessage, szTitle, MB_OK | MB_ICONEXCLAMATION );
    return MessageBoxA( 0, "Break?", szTitle, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES;
}
