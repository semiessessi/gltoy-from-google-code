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

#ifndef __GLTOY_BITSTREAM_H_
#define __GLTOY_BITSTREAM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_DataStructure.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream
: public GLToy_DataStructure< bool >
{

public:

    GLToy_BitStream( const u_int uByteCount = 4 )
    : m_uPosition( 0 )
    , m_uReadPos( 0 )
    , m_uNumBytes( uByteCount )
    {
        m_pcData = new char[ uByteCount ];
        for( u_int u = 0; u < uByteCount; ++u )
        {
            m_pcData[ u ] = 0;
        }
    }

    GLToy_BitStream( char* pcData, const u_int uByteCount )
    : m_pcData( pcData )
    , m_uPosition( 0 )
    , m_uReadPos( 0 )
    , m_uNumBytes( uByteCount )
    {
    }

    virtual ~GLToy_BitStream()
    {
        delete[] m_pcData;
    }

    void operator <<( const bool bBool ) { WriteBool( bBool ); }
    void operator <<( const char cChar ) { WriteChar( cChar ); }
    void operator <<( const u_char ucChar ) { WriteUChar( ucChar ); }
    void operator <<( const short sShort ) { WriteShort( sShort ); }
    void operator <<( const u_short usShort ) { WriteUShort( usShort ); }
    void operator <<( const wchar_t wcChar ) { WriteBits( wcChar, 16 ); }
    void operator <<( const int iInt ) { WriteInt( iInt ); }
    void operator <<( const u_int uUint ) { WriteUInt( uUint ); }
    void operator <<( const float fFloat ) { WriteFloat( fFloat ); }
    void operator <<( const double dDouble ) { WriteDouble( dDouble ); }
    template< class T >
    void operator <<( const T& xSerialisable ) { xSerialisable.WriteToBitStream( *this ); }

    void operator >>( bool& bBool ) const { ReadBool( bBool ); }
    void operator >>( char& cChar ) const { ReadChar( cChar ); }
    void operator >>( u_char& ucChar ) const { ReadUChar( ucChar ); }
    void operator >>( short& sShort ) const { ReadShort( sShort ); }
    void operator >>( u_short& usShort ) const { ReadUShort( usShort ); }
    void operator >>( wchar_t& wcChar ) const { u_int uBits; ReadBits( uBits, 16 ); wcChar = uBits; }
    void operator >>( int& iInt ) const { ReadInt( iInt ); }
    void operator >>( u_int& uUint ) const { ReadUInt( uUint ); }
    void operator >>( float& fFloat ) const { ReadFloat( fFloat ); }
    void operator >>( double& dDouble ) const { ReadDouble( dDouble ); }
    template< class T >
    void operator >>( T& xSerialisable ) const { xSerialisable.ReadFromBitStream( *this ); }

    void WriteBool( const bool bBool ) { WriteBit( bBool ); }
    void WriteHalfByte( const char cValue ) { WriteBits( cValue, 4 ); }
    void WriteChar( const char cChar ) { WriteBits( cChar, 8 ); }
    void WriteUChar( const u_char ucChar ) { WriteBits( ucChar, 8 ); }
    void WriteShort( const short sShort ) { WriteBits( sShort, 16 ); }
    void WriteUShort( const u_short usShort ) { WriteBits( usShort, 16 ); }
    void WriteInt( const int iInt ) { WriteBits( iInt, 32 ); }
    void WriteUInt( const u_int uUint ) { WriteBits( uUint, 32 ); }
    void WriteFloat( const float fFloat ){ WriteData( reinterpret_cast< const char* >( &fFloat ), 32 ); }
    void WriteDouble( const double dDouble ){ WriteData( reinterpret_cast< const char* >( &dDouble ), 64 ); }
    void WriteBits( const u_int uBits, const u_int uBitCount );
    void WriteData( const char* const pcData, const u_int uBitCount );
    
    void ReadBool( bool& bBool ) const { bBool = ReadBit(); }
    void ReadHalfByte( char& cValue ) const { u_int uBits; ReadBits( uBits, 4 ); cValue = uBits; }
    void ReadChar( char& cChar ) const { u_int uBits; ReadBits( uBits, 8 ); cChar = uBits; }
    void ReadUChar( u_char& ucChar ) const { u_int uBits; ReadBits( uBits, 8 ); ucChar = uBits; }
    void ReadShort( short& sShort ) const { u_int uBits; ReadBits( uBits, 16 ); sShort = uBits; }
    void ReadUShort( u_short& usShort ) const { u_int uBits; ReadBits( uBits, 16 ); usShort = uBits; }
    void ReadInt( int& iInt ) const { u_int uBits; ReadBits( uBits, 32 ); iInt = uBits; }
    void ReadUInt( u_int& uUint ) const { ReadBits( uUint, 32 ); }
    void ReadFloat( float& fFloat ) const { ReadData( reinterpret_cast< char* >( &fFloat ), 32 ); }
    void ReadDouble( double& dDouble) const { ReadData( reinterpret_cast< char* >( &dDouble ), 64 ); }
    void ReadBits( u_int& uBits, const u_int uBitCount ) const;
    void ReadData( char* pcOutput, const u_int uBitCount ) const;

    void ByteAlignedWrite( char* const pcData, const u_int uNumBytes ) const;

    u_int GetBitsWritten() const { return m_uPosition; }
    u_int GetBytesWritten() const { return ( m_uPosition + 7 ) >> 3; }

    char* GetData() { return m_pcData; }
    const char* GetData() const { return m_pcData; }
    void ResetReadPosition() const { m_uReadPos = 0; }
    void SetReadByte( const u_int uByte ) const { m_uReadPos = uByte << 3; }
    void SeekEnd() { m_uReadPos = m_uNumBytes << 3; m_uPosition = m_uReadPos; }

    void IgnoreNBytes( const u_int uN ) const { m_uReadPos += uN << 3; }
    bool HasNMoreBytes( const u_int uN ) const { return ( m_uReadPos + ( uN << 3 ) ) <= m_uPosition; }
    bool IsDoneReading() const { return m_uReadPos >= m_uPosition; }

    // overrides for data structure
    virtual u_int GetCount() const { return GetBitsWritten(); }
    virtual u_int GetMemoryUsage() const { return GetBytesWritten() + sizeof( *this ); }

    virtual bool IsFlat() const { return false; }

    void CopyFromByteArray( const char* const pcData, const u_int uByteCount )
    {
        delete m_pcData;
        m_pcData = new char[ uByteCount ];
        for( u_int u = 0; u < uByteCount; ++u )
        {
            m_pcData[ u ] = pcData[ u ];
        }

        SetFromByteArray( m_pcData, uByteCount );
    }

    void SetFromByteArray( char* pcData, const u_int uByteCount )
    {
        if( pcData != m_pcData )
        {
            delete m_pcData;
            m_pcData = pcData;
        }
        m_uReadPos = 0;
        m_uNumBytes = uByteCount;
        m_uPosition = m_uNumBytes << 3;
    }

    // the bit stream can never be written to by index operators...
    virtual bool& operator []( const int iIndex )
    {
        static bool ls_bDummy = false;
        ls_bDummy = ( m_pcData[ m_uPosition >> 3 ] & ( 1 << ( m_uPosition & 0x7 ) ) ) != 0;
        return ls_bDummy;
    }

    virtual const bool& operator []( const int iIndex ) const
    {
        static bool ls_bDummy = false;
        ls_bDummy = ( m_pcData[ m_uPosition >> 3 ] & ( 1 << ( m_uPosition & 0x7 ) ) ) != 0;
        return ls_bDummy;
    }

    // TODO - flesh these out if they would be useful
    // CopyFrom as well
    virtual void Traverse( GLToy_Functor< bool >& xFunctor ) {}
    virtual void Traverse( GLToy_ConstFunctor< bool >& xFunctor ) const {}

protected:

    void Grow( u_int uNumBytes );
    void WriteBit( bool bBit );
    bool ReadBit() const;

    char* m_pcData;
    u_int m_uPosition;
    mutable u_int m_uReadPos;
    u_int m_uNumBytes;

};

#endif
