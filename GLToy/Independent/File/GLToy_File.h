#ifndef __GLTOY_FILE_H_
#define __GLTOY_FILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_File
{

public:
    
    GLToy_File( const GLToy_String& szFilename );
    virtual ~GLToy_File() {}

    virtual void ReadToBitStream( GLToy_BitStream& xStream ) const;
    virtual void WriteFromBitStream( const GLToy_BitStream& xStream );

    virtual u_int GetSize() const { return m_uSize; }
    const GLToy_String& GetFilename() const { return m_szFilename; }

protected:

    GLToy_String m_szFilename;
    u_int m_uSize;

    void GetAllData( void* const pPointer ) const;

};

#endif