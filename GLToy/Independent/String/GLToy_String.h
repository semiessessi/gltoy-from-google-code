/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_STRING_H_
#define __GLTOY_STRING_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Array.h>

// GLToy
#include <Core/GLToy_Hash.h>

// C/C++
#include <stdarg.h>

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

    GLToy_Inline bool operator ==( const GLToy_String& xString ) const
    {
        GLToy_ConstIterate( wchar_t, xIterator, this )
        {
            if( xIterator.Current() != xString[ xIterator.Index() ] )
            {
                return false;
            }
        }

        return true;
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

    GLToy_Inline bool EndsWith( const GLToy_String& xString ) const
    {
        const u_int uOffset = GetLength() - xString.GetLength();
        for( u_int u = 0; u < xString.GetLength(); ++u )
        {
            if( xString[ u ] != m_pxData[ u + uOffset ] )
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

    GLToy_Inline GLToy_String RemoveFirstWord()
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

        if( m_pxData[ u ] == '\r' && m_pxData[ u + 1 ] == '\n' )
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

    GLToy_Inline GLToy_String RemoveUpTo( const wchar_t wcChar )
    {
        if( IsEmpty() )
        {
            return "";
        }

        GLToy_String xReturnValue;
        u_int u = 0;
        while( ( m_pxData[ u ] != wcChar ) && m_pxData[ u ] )
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

    GLToy_Inline GLToy_String RemoveFirstLine()
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

        if( m_pxData[ u ] == '\r' && m_pxData[ u + 1 ] == '\n' )
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

    GLToy_Array< GLToy_String > Split( const wchar_t wcSplitter ) const
    {
        GLToy_Array< GLToy_String > xReturnValue;

        if( IsEmpty() )
        {
            return xReturnValue;
        }

        GLToy_String szWorkingString( *this );

        while( !szWorkingString.IsEmpty() )
        {
            xReturnValue.Append( szWorkingString.RemoveUpTo( wcSplitter ) );

            if( szWorkingString[ 0 ] == wcSplitter ) // it could be NULL, or we might be buggy...
            {
                szWorkingString.RemoveAt( 0 );
            }
        }

        // remove crap from end if there is any...
        while( ( xReturnValue.GetCount() > 0 ) && xReturnValue.End().IsEmpty() )
        {
            xReturnValue.RemoveFromEnd();
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

	GLToy_Inline bool MeansFalse() const
	{
		return ( ( *this ) == "False" )
			|| ( ( *this ) == "No" )
			|| ( ( *this ) == "0" );
	}

    GLToy_Inline bool IsEmpty() const
    {
        return GetCount() < 2;
    }

    GLToy_Inline bool IsWhiteSpace() const
    {
        for( u_int u = 0; u < GetLength(); ++u )
        {
            if( !IsWhiteSpace( m_pxData[ u ] ) )
            {
                return false;
            }
        }

        return true;
    }

    GLToy_Inline void TrimLeadingWhiteSpace()
    {
        while( IsWhiteSpace( m_pxData[ 0 ] ) )
        {
            RemoveAt( 0 );
        }
    }

    GLToy_Inline void RemoveWhiteSpace()
    {
        for( u_int u = 0; u < GetLength(); ++u )
        {
            if( IsWhiteSpace( m_pxData[ u ] ) )
            {
                RemoveAt( u );
                --u;
            }
        }
    }

    // TODO - can't pass by reference due to va_args - I'm sure there is a portable way around it though
    void SetToFormatString( const GLToy_String szFormatString, ... );
    void SetToFormatString( const GLToy_String& szFormatString, const va_list& xArgumentList );

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

    GLToy_Inline bool IsWhiteSpace( const wchar_t wcChar ) const
    {
        switch( wcChar )
        {
            default:
            {
                return false;
            }

            case L' ':
            case L'\r':
            case L'\n':
            case L'\t':
            {
                return true;
            }
        }
    }
};

#endif