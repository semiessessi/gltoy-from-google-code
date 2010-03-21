#ifndef __GLTOY_ENVIRONMENTFILE_H_
#define __GLTOY_ENVIRONMENTFILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <File/GLToy_File.h>

// GLToy
#include <String/GLToy_String.h>

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

    GLToy_String GetName() const;

    virtual void LoadEnvironment() const;

    void LoadBSP29( const GLToy_BitStream& xStream ) const;
    void LoadBSP30( const GLToy_BitStream& xStream ) const;
    void LoadBSP38( const GLToy_BitStream& xStream ) const;
    void LoadBSP46( const GLToy_BitStream& xStream ) const;

    static void Save( const GLToy_String& szFilename );

protected:

};

#endif