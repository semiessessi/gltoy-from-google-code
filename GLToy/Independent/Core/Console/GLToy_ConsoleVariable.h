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