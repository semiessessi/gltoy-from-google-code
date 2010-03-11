#ifndef __GLTOY_STRING_H_
#define __GLTOY_STRING_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Array.h>

// GLToy
#include <Core/GLToy_Hash.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String
: public GLToy_SerialisableArray< wchar_t >
{

    typedef GLToy_SerialisableArray< wchar_t > GLToy_Parent;

public:

    GLToy_String()
    : GLToy_Parent()
    {
    }

    GLToy_String( const char* const szString )
    : GLToy_Parent()
    {
        for( u_int u = 0; szString[ u ]; ++u )
        {
            Append( szString[ u ] );
        }
        Append( 0 );
    }

    GLToy_String( const wchar_t* const wszString )
    : GLToy_Parent()
    {
        for( u_int u = 0; wszString[ u ]; ++u )
        {
            Append( wszString[ u ] );
        }
        Append( 0 );
    }

    GLToy_String( const GLToy_String& xString )
    : GLToy_Parent( xString )
    {
    }

    GLToy_Inline u_int GetLength() const { return GetCount() - 1; }
    GLToy_Inline wchar_t* GetWideString() { return GetDataPointer(); }
    GLToy_Inline const wchar_t* GetWideString() const { return GetDataPointer(); }

    GLToy_Inline char* CreateANSIString() const
    {
        char* pcNewString = new char[ GetCount() ];

        GLToy_ConstIterate( wchar_t, xIterator, this )
        {
            pcNewString[ xIterator.Index() ]
                = xIterator.Current() > 0xFF
                ? '?'
                : static_cast< char >( xIterator.Current() );
        }

        return pcNewString;
    }

    GLToy_Inline GLToy_String& operator =( const GLToy_String& xString )
    {
        CopyFrom( &xString );
        return *this;
    }

    GLToy_Inline GLToy_String operator +( const GLToy_String& xString ) const
    {
        GLToy_String xReturnValue( *this );
        xReturnValue.RemoveNull();
        xReturnValue.Append( xString );
        return xReturnValue;
    }

    GLToy_Inline GLToy_String& operator +=( const GLToy_String& xString )
    {
        RemoveNull();
        Append( xString );
        return *this;
    }

    GLToy_Inline GLToy_Hash GetHash() const { return GLToy_GetHash( GetWideString() ); }

    GLToy_Inline bool BeginsWith( const GLToy_String& xString )
    {
        for( u_int u = 0; u < xString.GetLength(); ++u )
        {
            if( xString[ u ] != m_pxData[ u ] )
            {
                return false;
            }
        }

        return true;
    }

    virtual void RemoveFromEnd( const u_int uAmount = 1 )
    {
        GLToy_Parent::RemoveFromEnd( uAmount + 1 );
        GLToy_Parent::Append( 0 );
    }

protected:

    void RemoveNull()
    {
        GLToy_Parent::RemoveFromEnd();
    }

};

#endif