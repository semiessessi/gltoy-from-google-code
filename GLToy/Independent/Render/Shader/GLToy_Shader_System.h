/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi
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

#ifndef __GLTOY_SHADER_SYSTEM_H_
#define __GLTOY_SHADER_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashMap;
class GLToy_ShaderProgram;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Shader_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void BindShaderProgram( const GLToy_Hash uHash );
    static void BindShaderProgram( const GLToy_String& szName );

    static GLToy_ShaderProgram* FindShader( const GLToy_Hash uHash );
    static GLToy_ShaderProgram* LookUpShader( const GLToy_String& szName );

	static void CreateShaderFromStrings( const GLToy_String& szName, const GLToy_String& szFragmentShader, const GLToy_String& szVertexShader );

    static void ReloadShaders() { Shutdown(); Initialise(); }

private:

    static GLToy_HashMap< GLToy_ShaderProgram* > s_xPrograms;

};

#endif
