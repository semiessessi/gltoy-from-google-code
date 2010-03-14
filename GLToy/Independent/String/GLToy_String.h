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
        Append( 0 );
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

    virtual void Clear()
    {
        GLToy_Parent::Clear();
        Append( 0 );
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
        GLToy_Parent::operator =( xString );
        return *this;
    }

    GLToy_Inline GLToy_String operator +( const GLToy_String& xString ) const
    {
        GLToy_String xReturnValue( *this );
        xReturnValue += xString;
        return xReturnValue;
    }

    GLToy_Inline GLToy_String& operator +=( const GLToy_String& xString )
    {
        RemoveNull();
        Append( xString );
        return *this;
    }

    GLToy_Inline GLToy_String& operator +=( const wchar_t wcChar )
    {
        End() = wcChar;
        Append( 0 );
        return *this;
    }

    GLToy_Inline GLToy_Hash GetHash() const { return GLToy_GetHash( GetWideString() ); }

    GLToy_Inline bool BeginsWith( const GLToy_String& xString ) const
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

    GLToy_Inline u_int ExtractUnsignedInt( const u_int uPosition = 0 ) const
    {
        u_int uRet = 0;
        u_int u = 0;
        while( IsDigit( m_pxData[ uPosition + u ] ) )
        {
            uRet *= 10;
            uRet += ( m_pxData[ uPosition + u ] - L'0' );
            ++u;
        }

        return uRet;
    }

    GLToy_String RemoveFirstWord()
    {
        GLToy_String xReturnValue;
        u_int u = 0;
        while( ( m_pxData[ u ] != L' ' )
            && ( m_pxData[ u ] != L'\n' )
            && ( m_pxData[ u ] != L'\r' )
            && ( m_pxData[ u ] != L'\t' )
            && m_pxData[ u ] )
        {
            xReturnValue += m_pxData[ u ];
            ++u;
        }

        RemoveAt( 0, u + 1 );

        if( m_uCount == 0 )
        {
            Append( 0 );
        }

        return xReturnValue;
    }

    GLToy_String RemoveFirstLine()
    {
        GLToy_String xReturnValue;
        u_int u = 0;
        while( ( m_pxData[ u ] != L'\n' )
            && ( m_pxData[ u ] != L'\r' )
            && m_pxData[ u ] )
        {
            xReturnValue += m_pxData[ u ];
            ++u;
        }

        if( m_pxData[ u ] == '\r ' && m_pxData[ u + 1 ] == '\n ' )
        {
            ++u;
        }

        RemoveAt( 0, u + 1 );

        if( m_uCount == 0 )
        {
            Append( 0 );
        }

        return xReturnValue;
    }

    GLToy_Inline bool Contains( wchar_t wcChar ) const
    {
        GLToy_ConstIterate( wchar_t, xIterator, this )
        {
            if( xIterator.Current() == wcChar )
            {
                return true;
            }
        }

        return false;
    }

    // TODO - can't pass by reference due to va_args - I'm sure there is a portable way around it though
    void SetToFormatString( const GLToy_String szFormatString, ... );

protected:

    void RemoveNull()
    {
        GLToy_Parent::RemoveFromEnd();
    }

    GLToy_Inline bool IsDigit( const wchar_t wcChar ) const
    {
        switch( wcChar )
        {
            default:
            {
                return false;
            }

            case L'0':
            case L'1':
            case L'2':
            case L'3':
            case L'4':
            case L'5':
            case L'6':
            case L'7':
            case L'8':
            case L'9':
            {
                return true;
            }
        }
    }
};

#endif