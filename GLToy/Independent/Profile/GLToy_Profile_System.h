/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#ifndef __GLTOY_PROFILE_SYSTEM_H
#define __GLTOY_PROFILE_SYSTEM_H

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLTOY_DEFAULT_PROFILE_NAME "Default"

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream;
class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Profile_System
{


public:

    static void SwitchProfile( const GLToy_String& szProfileName );

    static void Load( const GLToy_Hash uHash, GLToy_BitStream& xData ) { Platform_Load( uHash, xData ); }
    static void Save( const GLToy_Hash uHash, const GLToy_BitStream& xData ) { Platform_Save( uHash, xData ); }

    static void LoadGlobal( const GLToy_Hash uHash, GLToy_BitStream& xData ) { Platform_LoadGlobal( uHash, xData ); }
    static void SaveGlobal( const GLToy_Hash uHash, const GLToy_BitStream& xData ) { Platform_SaveGlobal( uHash, xData ); }

    static bool Initialise() { return Platform_Initialise(); }
    static void Shutdown() {}

private:

    static bool Platform_Initialise();

    static void Platform_Load( const GLToy_Hash uHash, GLToy_BitStream& xData );
    static void Platform_Save( const GLToy_Hash uHash, const GLToy_BitStream& xData );

    static void Platform_LoadGlobal( const GLToy_Hash uHash, GLToy_BitStream& xData );
    static void Platform_SaveGlobal( const GLToy_Hash uHash, const GLToy_BitStream& xData );

    static GLToy_String s_szCurrentProfileName;
};

#endif
