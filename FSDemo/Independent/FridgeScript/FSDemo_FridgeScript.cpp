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

#include <Core/FSDemo.h>

// This file's header
#include <FridgeScript/FSDemo_FridgeScript.h>


// GLToy headers
#include <File/GLToy_ANSITextFile.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int FSDemo_FridgeScript::s_uContextID = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FSDemo_FridgeScript::Initialise()
{
    s_uContextID = FSCreateContext();
    return true;
}

void FSDemo_FridgeScript::Shutdown()
{
    FSDestroyContext( s_uContextID );
}

void FSDemo_FridgeScript::RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount )
{
    FSRegisterAPI( s_uContextID, szName, pfnCallback, uParameterCount );
}

FSDemo_FridgeScript* FSDemo_FridgeScript::CreateFromFile( const char* const szFilename )
{
    FSDemo_FridgeScript* pxFridgeScript = new FSDemo_FridgeScript();

    pxFridgeScript->CompileFromFile( szFilename );

    return pxFridgeScript;
}

FSDemo_FridgeScript::FSDemo_FridgeScript()
: m_uCodeID( 0 )
{   
}

FSDemo_FridgeScript::~FSDemo_FridgeScript()
{
}

void FSDemo_FridgeScript::CompileFromFile( const char* const szFilename )
{
    GLToy_ANSITextFile xFile = GLToy_ANSITextFile( szFilename );
    u_int uLength = xFile.GetSize();
    
    char* pcData = new char[ uLength ];
    xFile.GetRawString( pcData );

    m_uCodeID = FSCompile( s_uContextID, pcData );
}

void FSDemo_FridgeScript::Execute()
{
    FSExecute( s_uContextID, m_uCodeID );
}
