#ifndef __GLTOY_FONT_H_
#define __GLTOY_FONT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class DataType, class KeyType > class GLToy_BinaryTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Font
{

public:

    GLToy_Font( const GLToy_String& szName, const u_int uSize = 16 )
    : m_szName( szName )
    , m_uSize( uSize )
    {
    }

    virtual ~GLToy_Font() {}

    GLToy_Inline GLToy_Hash GetHash() const { return m_szName.GetHash(); }
    GLToy_Inline const GLToy_String& GetName() const { return m_szName; }
    GLToy_Inline u_int GetSize() const { return m_uSize; }

    virtual void RenderCharacter( const wchar_t ) const = 0;

protected:

    GLToy_String m_szName;
    u_int m_uSize;

};

class GLToy_Font_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static GLToy_Font* FindFont( const GLToy_Hash uHash );
    static GLToy_Font* LookUpFont( const GLToy_String& xName );

private:

    static GLToy_BinaryTree< GLToy_Font*, GLToy_Hash > s_xFonts;

};

#endif