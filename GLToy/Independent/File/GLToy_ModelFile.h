/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
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

    virtual GLToy_Model* LoadModel() const = 0;

    const GLToy_Model* GetModelPointer() const { return m_pxModel; }

protected:

    mutable GLToy_Model* m_pxModel;

};

#endif
