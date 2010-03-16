#ifndef __GLTOY_CONSOLECOMMAND_H_
#define __GLTOY_CONSOLECOMMAND_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ConsoleCommand
{

public:

    GLToy_ConsoleCommand( const GLToy_String& szName )
    : m_szName( szName )
    {
    }

    virtual ~GLToy_ConsoleCommand() {}

    GLToy_Inline const GLToy_String& GetName() const { return m_szName; }

    virtual void Execute( const GLToy_String& szParameterString ) = 0;

protected:

    static bool BoolFromParameters( const GLToy_String& szParameterString );

    GLToy_String m_szName;

};

class GLToy_ConsoleCommand_Void
: public GLToy_ConsoleCommand
{

    typedef GLToy_ConsoleCommand GLToy_Parent;

public:

    GLToy_ConsoleCommand_Void( const GLToy_String& szName, void ( *pfnFunction )() )
    : GLToy_Parent( szName )
    , m_pfnFunction( pfnFunction )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString )
    {
        m_pfnFunction();
    }

protected:

    void ( *m_pfnFunction )();

};

class GLToy_ConsoleCommand_Bool
: public GLToy_ConsoleCommand
{

    typedef GLToy_ConsoleCommand GLToy_Parent;

public:

    GLToy_ConsoleCommand_Bool( const GLToy_String& szName, void ( *pfnFunction )( const bool ) )
    : GLToy_Parent( szName )
    , m_pfnFunction( pfnFunction )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString );

protected:

    void ( *m_pfnFunction )( const bool );

};

class GLToy_ConsoleCommand_Uint
: public GLToy_ConsoleCommand
{

    typedef GLToy_ConsoleCommand GLToy_Parent;

public:

    GLToy_ConsoleCommand_Uint( const GLToy_String& szName, void ( *pfnFunction )( const u_int ) )
    : GLToy_Parent( szName )
    , m_pfnFunction( pfnFunction )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString );

protected:

    void ( *m_pfnFunction )( const u_int );

};

class GLToy_ConsoleCommand_String
: public GLToy_ConsoleCommand
{

    typedef GLToy_ConsoleCommand GLToy_Parent;

public:

    GLToy_ConsoleCommand_String( const GLToy_String& szName, void ( *pfnFunction )( const GLToy_String& ) )
    : GLToy_Parent( szName )
    , m_pfnFunction( pfnFunction )
    {
    }

    virtual void Execute( const GLToy_String& szParameterString )
    {
        m_pfnFunction( szParameterString );
    }

protected:

    void ( *m_pfnFunction )( const GLToy_String& );

};

#endif