#ifndef __GLTOY_MODELFILE_H_
#define __GLTOY_MODELFILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <File/GLToy_File.h>

// GLToy
#include <Model/GLToy_Model.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ModelFile
: public GLToy_File
{

    typedef GLToy_File GLToy_Parent;

public:

    GLToy_ModelFile( const GLToy_String& szFilename )
    : GLToy_Parent( szFilename )
    , m_pxModel( NULL )
    {
    }

    virtual ~GLToy_ModelFile()
    {
        delete m_pxModel;
    }

    virtual GLToy_Model* LoadModel() const = 0;

    const GLToy_Model* GetModelPointer() const { return m_pxModel; }

protected:

    mutable GLToy_Model* m_pxModel;

};

#endif