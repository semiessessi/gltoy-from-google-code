/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

    GLToy_Model* LoadModel() const
    {
        if( m_pxModel )
        {
            return m_pxModel;
        }

        return m_pxModel = CreateModelInstance();
    }

    virtual GLToy_Model* CreateModelInstance() const = 0;

    const GLToy_Model* GetModelPointer() const { return m_pxModel; }

protected:

    mutable GLToy_Model* m_pxModel;

};

#endif
