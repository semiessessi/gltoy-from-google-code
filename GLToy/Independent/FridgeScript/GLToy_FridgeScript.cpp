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

#ifdef GLTOY_USE_FRIDGESCRIPT

// This file's header
#include <FridgeScript/GLToy_FridgeScript.h>


// GLToy headers
#include <File/GLToy_TextFile.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int GLToy_FridgeScript::s_uContextID = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_FridgeScript::Initialise()
{
    s_uContextID = FSCreateContext();
    return true;
}

void GLToy_FridgeScript::Shutdown()
{
    FSDestroyContext( s_uContextID );
}

void GLToy_FridgeScript::RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount )
{
    FSRegisterAPI( s_uContextID, szName, pfnCallback, uParameterCount );
}

GLToy_FridgeScript* GLToy_FridgeScript::CreateFromFile( const char* const szFilename )
{
    GLToy_FridgeScript* pxFridgeScript = new GLToy_FridgeScript();

    pxFridgeScript->CompileFromFile( szFilename );

    return pxFridgeScript;
}

GLToy_FridgeScript::GLToy_FridgeScript()
: m_uCodeID( 0 )
{   
}

GLToy_FridgeScript::~GLToy_FridgeScript()
{
}

void GLToy_FridgeScript::CompileFromFile( const char* const szFilename )
{
    GLToy_TextFile xFile = GLToy_TextFile( szFilename );
    u_int uLength = xFile.GetSize();
    
    char* pcData = new char[ uLength ];
    xFile.GetRawString( pcData );

    m_uCodeID = FSCompile( s_uContextID, pcData );
}

void GLToy_FridgeScript::Execute()
{
    FSExecute( s_uContextID, m_uCodeID );
}

#endif
