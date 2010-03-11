#ifndef __GLTOY_ANSITEXTFILE_H_
#define __GLTOY_ANSITEXTFILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <File/GLToy_File.h>

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ANSITextFile : public GLToy_File
{

    typedef GLToy_File GLToy_Parent;

public:
    
    GLToy_ANSITextFile( const GLToy_String& szFilename );
    virtual ~GLToy_ANSITextFile();

    void GetRawString( char* szString ) const;
    GLToy_String GetString() const;
    virtual u_int GetSize() const { return m_uSize + 1; }

protected:

};

#endif