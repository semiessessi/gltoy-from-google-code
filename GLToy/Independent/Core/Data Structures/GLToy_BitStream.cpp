/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/Data Structures/GLToy_BitStream.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - optimise
void GLToy_BitStream::WriteBits( const unsigned int uBits, const unsigned int uBitCount )
{
    GLToy_Assert( uBitCount < 33, "Bit count is too great to write: %u", uBitCount );
    for( unsigned int u = 0; u < uBitCount; ++u )
    {
        WriteBit( ( uBits >> u ) & 0x1 );
    }
}

// TODO - optimise
void GLToy_BitStream::ReadBits( unsigned int& uBits, const unsigned int uBitCount ) const
{
    GLToy_Assert( uBitCount < 33, "Bit count is too great to read: %u", uBitCount );
    uBits = 0;
    for( unsigned int u = 0; u < uBitCount; ++u )
    {
        uBits |= ( ReadBit() ? 1 : 0 ) << u;
    }
}

// TODO - optimise
void GLToy_BitStream::WriteData( const char* const pcData, const unsigned int uBitCount )
{
    for( unsigned int u = 0; u < ( uBitCount >> 3 ); ++u )
    {
        WriteChar( pcData[ u ] );
    }

    if( uBitCount & 7 )
    {
        WriteBits( pcData[ ( uBitCount >> 3 ) + 1 ], uBitCount & 7 );
    }
}

// TODO - optimise
void GLToy_BitStream::ReadData( char* pcOutput, const unsigned int uBitCount ) const
{
    for( unsigned int u = 0; u < ( uBitCount >> 3 ); ++u )
    {
        ReadChar( pcOutput[ u ] );
    }

    if( uBitCount & 7 )
    {
        unsigned int uBits;
        ReadBits( uBits, uBitCount & 7 );
        pcOutput[ ( uBitCount >> 3 ) + 1 ] = uBits & 0xFF;
    }
}

void GLToy_BitStream::WriteVector( const GLToy_Vector_3& xVector )
{
    xVector.WriteToBitStream( *this );
}

void GLToy_BitStream::ReadVector( GLToy_Vector_3& xVector ) const
{
    xVector.ReadFromBitStream( *this );
}

void GLToy_BitStream::Grow( const unsigned int uNumBytes = 1 )
{
    char* pxNewBuffer = new char[ m_uNumBytes + uNumBytes ];

    for( unsigned int u = m_uNumBytes; u < m_uNumBytes + uNumBytes; ++u )
    {
        pxNewBuffer[ u ] = 0;
    }

    for( unsigned int u = 0; u < m_uNumBytes; ++u )
    {
        pxNewBuffer[ u ] = m_pcData[ u ];
    }
    
    delete[] m_pcData;
    m_pcData = pxNewBuffer;

    m_uNumBytes += uNumBytes;
}

void GLToy_BitStream::WriteBit( bool bBit )
{
    if( !bBit )
    {
       ++m_uPosition;
       if( ( m_uPosition >> 3 ) >= m_uNumBytes )
       {
           Grow();
       }
       return;
    }

    unsigned int uBytePos = m_uPosition >> 3;
    unsigned int uBitPos = m_uPosition & 7;

    m_pcData[ uBytePos ] |= ( 1 << uBitPos );

    ++uBitPos;
    if( uBitPos > 7 )
    {
       ++uBytePos;
       uBitPos = 0;
    }

    ++m_uPosition;
    if( ( m_uPosition >> 3 ) >= m_uNumBytes )
    {
        Grow();
    }
}

bool GLToy_BitStream::ReadBit() const
{
    const unsigned int uBytePos = m_uReadPos >> 3;
    const unsigned int uBitPos = m_uReadPos & 7;
    ++m_uReadPos;

    return ( m_pcData[ uBytePos ] & ( 1 << uBitPos ) ) != 0;
}