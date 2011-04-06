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
#ifdef GLTOY_DEBUG
    , dbg_szString( m_pxData )
#endif
    {
        Append( 0 );
    }

    GLToy_String( const char* const szString )
    : GLToy_Parent()
#ifdef GLTOY_DEBUG
    , dbg_szString( m_pxData )
#endif
    {
        for( u_int u = 0; szString[ u ]; ++u )
        {
            Append( szString[ u ] );
        }
        Append( 0 );
    }

    GLToy_String( const u_char* const szString )
    : GLToy_Parent()
#ifdef GLTOY_DEBUG
    , dbg_szString( m_pxData )
#endif
    {
        for( u_int u = 0; szString[ u ]; ++u )
        {
            Append( szString[ u ] );
        }
        Append( 0 );
    }

    GLToy_String( const wchar_t* const wszString )
    : GLToy_Parent()
#ifdef GLTOY_DEBUG
    , dbg_szString( m_pxData )
#endif
    {
        for( u_int u = 0; wszString[ u ]; ++u )
        {
            Append( wszString[ u ] );
        }
        Append( 0 );
    }

    GLToy_String( const GLToy_String& xString )
    : GLToy_Parent( xString )
#ifdef GLTOY_DEBUG
    , dbg_szString( m_pxData )
#endif
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

        GLToy_ConstIterate( wchar_t, wcChar, *this )
            pcNewString[ xIterator.Index() ]
                = wcChar > 0xFF
                ? '?'
                : static_cast< char >( wcChar );
        GLToy_Iterate_End;

        return pcNewString;
    }

    GLToy_Inline bool operator ==( const GLToy_String& xString ) const
    {
        GLToy_ConstIterate( wchar_t, wcChar, *this )
            if( wcChar != xString[ xIterator.Index() ] )
            {
                return false;
            }
        GLToy_Iterate_End;

        return true;
    }

    GLToy_Inline bool operator !=( const GLToy_String& xString ) const
    {
        GLToy_ConstIterate( wchar_t, wcChar, *this )
            if( wcChar != xString[ xIterator.Index() ] )
            {
                return true;
            }
        GLToy_Iterate_End;

        return false;
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

    GLToy_Inline GLToy_String operator +( const u_int uValue ) const
    {
        GLToy_String xReturnValue;
        xReturnValue.SetToFormatString( "%S%d", GetWideString(), uValue );
        return xReturnValue;
    }

    GLToy_Inline GLToy_String operator +( const int iValue ) const
    {
        GLToy_String xReturnValue;
        xReturnValue.SetToFormatString( "%S%d", GetWideString(), iValue );
        return xReturnValue;
    }

    GLToy_Inline GLToy_String operator +( const float fValue ) const
    {
        GLToy_String xReturnValue;
        xReturnValue.SetToFormatString( "%S%f", GetWideString(), fValue );
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
            if( xString[ u ] != GLToy_Parent::m_pxData[ u ] )
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
            if( xString[ u ] != GLToy_Parent::m_pxData[ u + uOffset ] )
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
        while( IsDigit( GLToy_Parent::m_pxData[ uPosition + u ] ) )
        {
            uRet *= 10;
            uRet += ( GLToy_Parent::m_pxData[ uPosition + u ] - L'0' );
            ++u;
        }

        return uRet;
    }

    GLToy_Inline u_int ExtractSignedInt( const u_int uPosition = 0 ) const
    {
        u_int uRet = 0;
        u_int u = 0;
        int iSign = 1;
        if( m_pxData[ uPosition ] == L'-' )
        {
            iSign = -1;
            ++u;
        }

        while( IsDigit( GLToy_Parent::m_pxData[ uPosition + u ] ) )
        {
            uRet *= 10;
            uRet += ( GLToy_Parent::m_pxData[ uPosition + u ] - L'0' );
            ++u;
        }

        return iSign * static_cast< int >( uRet );
    }

    float ExtractFloat( const u_int uPosition = 0 ) const;

    GLToy_Inline GLToy_String RemoveFirstWord()
    {
        GLToy_String xReturnValue;
        u_int u = 0;
        while( ( GLToy_Parent::m_pxData[ u ] != L' ' )
            && ( GLToy_Parent::m_pxData[ u ] != L'\n' )
            && ( GLToy_Parent::m_pxData[ u ] != L'\r' )
            && ( GLToy_Parent::m_pxData[ u ] != L'\t' )
            && GLToy_Parent::m_pxData[ u ] )
        {
            xReturnValue += GLToy_Parent::m_pxData[ u ];
            ++u;
        }

        if( GLToy_Parent::m_pxData[ u ] == '\r' && GLToy_Parent::m_pxData[ u + 1 ] == '\n' )
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
        while( ( GLToy_Parent::m_pxData[ u ] != wcChar ) && GLToy_Parent::m_pxData[ u ] )
        {
            xReturnValue += GLToy_Parent::m_pxData[ u ];
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
        while( ( GLToy_Parent::m_pxData[ u ] != L'\n' )
            && ( GLToy_Parent::m_pxData[ u ] != L'\r' )
            && GLToy_Parent::m_pxData[ u ] )
        {
            xReturnValue += GLToy_Parent::m_pxData[ u ];
            ++u;
        }

        if( GLToy_Parent::m_pxData[ u ] == '\r' && GLToy_Parent::m_pxData[ u + 1 ] == '\n' )
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
        GLToy_ConstIterate( wchar_t, wcCharacter, *this )
            if( wcCharacter == wcChar )
            {
                return true;
            }
        GLToy_Iterate_End;

        return false;
    }

	GLToy_Inline bool Contains( const GLToy_String& szString ) const
	{
		if( GetCount() < szString.GetCount() )
		{
			return false;
		}

		u_int uPos = 0;
		GLToy_ConstIterate( wchar_t, wcChar, *this )
			if( wcChar == szString[ uPos ] )
			{
				++uPos;
				if( uPos == ( szString.GetLength() - 1 ) )
				{
					return true;
				}
			}
			else
			{
				uPos = 0;
			}
		GLToy_Iterate_End;

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
            if( !IsWhiteSpace( GLToy_Parent::m_pxData[ u ] ) )
            {
                return false;
            }
        }

        return true;
    }

    GLToy_Inline void TrimLeadingWhiteSpace()
    {
        while( IsWhiteSpace( GLToy_Parent::m_pxData[ 0 ] ) )
        {
            RemoveAt( 0 );
        }
    }

    GLToy_Inline void TrimTrailingWhiteSpace()
    {
        while( IsWhiteSpace( GLToy_Parent::m_pxData[ GetLength() - 1 ] ) )
        {
            RemoveFromEnd( 1 );
        }
    }

    GLToy_Inline void TrimWhiteSpace()
    {
        TrimLeadingWhiteSpace();
        TrimTrailingWhiteSpace();
    }

    GLToy_Inline void RemoveWhiteSpace()
    {
        for( u_int u = 0; u < GetLength(); ++u )
        {
            if( IsWhiteSpace( GLToy_Parent::m_pxData[ u ] ) )
            {
                RemoveAt( u );
                --u;
            }
        }
    }

    GLToy_String Right( const unsigned int uLength ) const
    {
        return GLToy_String( &( GetDataPointer()[ GetLength() - uLength ] ) );
    }

    GLToy_String Left( const unsigned int uLength ) const
    {
        GLToy_String szReturnValue( *this );
        szReturnValue.RemoveFromEnd( GetLength() - uLength );
        return szReturnValue;
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

#ifdef GLTOY_DEBUG
    wchar_t*& dbg_szString;
#endif

};

#endif
