/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

#ifndef __GLTOY_MODEL_COLOURED_H_
#define __GLTOY_MODEL_COLOURED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Model/GLToy_Model.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ModelStrip_Coloured
: public GLToy_ModelStrip
{

    typedef GLToy_ModelStrip GLToy_Parent;

public:

    GLToy_ModelStrip_Coloured();
    GLToy_ModelStrip_Coloured( const GLToy_ModelStrip_Coloured& xStrip );
    GLToy_ModelStrip_Coloured( u_int uVertex1, u_int uVertex2, u_int uVertex3, const GLToy_Vector_3& xColour );
    GLToy_ModelStrip_Coloured( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4, const GLToy_Vector_3& xColour );
    virtual ~GLToy_ModelStrip_Coloured();

    virtual GLToy_ModelStrip_Coloured& operator =( const GLToy_ModelStrip_Coloured& xStrip );

    virtual void Render() const;

    // overrides
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

protected:
    
    GLToy_Vector_3 m_xColour;

};

class GLToy_Model_Coloured
: public GLToy_Model
{

    typedef GLToy_Model GLToy_Parent;

public:

    GLToy_Model_Coloured();
    virtual ~GLToy_Model_Coloured();

    void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xColour );
    void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4, const GLToy_Vector_3& xColour );

    // overrides
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );

};

#endif