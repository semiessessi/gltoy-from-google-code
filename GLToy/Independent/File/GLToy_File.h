#ifndef __GLTOY_FILE_H_
#define __GLTOY_FILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_File
{

public:
    
    GLToy_File( const char* const szFilename );
    virtual ~GLToy_File();

    u_int GetSize() const { return m_uSize; }

protected:

    char* m_szFilename;
    u_int m_uSize;

    void GetAllData( void* const pPointer ) const;

};

#endif