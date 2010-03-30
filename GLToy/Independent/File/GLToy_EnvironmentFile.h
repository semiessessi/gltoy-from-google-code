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