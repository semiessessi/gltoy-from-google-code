#ifndef __GLTOY_ENVIRONMENTFILE_H_
#define __GLTOY_ENVIRONMENTFILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <File/GLToy_File.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EnvironmentFile
: public GLToy_File
{

    typedef GLToy_File GLToy_Parent;

public:
    
    GLToy_EnvironmentFile( const GLToy_String& szFilename )
    : GLToy_Parent( szFilename )
    {
    }

    virtual ~GLToy_EnvironmentFile()
    {
    }

    virtual void LoadEnvironment() const;

    static void Save( const GLToy_String& szFilename );

protected:

};

#endif