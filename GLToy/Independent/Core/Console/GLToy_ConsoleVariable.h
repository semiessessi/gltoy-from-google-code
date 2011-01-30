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

#ifndef __GLTOY_CONSOLEVARIABLE_H_
#define __GLTOY_CONSOLEVARIABLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Console/GLToy_ConsoleCommand.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ConsoleVariable_Bool
: public GLToy_ConsoleCommand_Bool
{
    typedef GLToy_ConsoleCommand_Bool GLToy_Parent;

public:
    
    GLToy_ConsoleVariable_Bool( const GLToy_String& szName, bool* pbVariable )
    : GLToy_Parent( szName, NULL )
    , m_pbVariable( pbVariable )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString );

protected:

    bool* m_pbVariable;

};

class GLToy_ConsoleVariable_Uint
: public GLToy_ConsoleCommand_Uint
{
    typedef GLToy_ConsoleCommand_Uint GLToy_Parent;

public:
    
    GLToy_ConsoleVariable_Uint( const GLToy_String& szName, u_int* puVariable )
    : GLToy_Parent( szName, NULL )
    , m_puVariable( puVariable )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString );

protected:

    u_int* m_puVariable;

};

class GLToy_ConsoleVariable_Float
: public GLToy_ConsoleCommand_Float
{
    typedef GLToy_ConsoleCommand_Float GLToy_Parent;

public:
    
    GLToy_ConsoleVariable_Float( const GLToy_String& szName, float* pfVariable )
    : GLToy_Parent( szName, NULL )
    , m_pfVariable( pfVariable )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString );

protected:

    float* m_pfVariable;

};

class GLToy_ConsoleVariable_String
: public GLToy_ConsoleCommand_String
{
    typedef GLToy_ConsoleCommand_String GLToy_Parent;

public:
    
    GLToy_ConsoleVariable_String( const GLToy_String& szName, GLToy_String* pszVariable )
    : GLToy_Parent( szName, NULL )
    , m_pszVariable( pszVariable )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString );

protected:

    GLToy_String* m_pszVariable;

};

#endif
