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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Maths/GLToy_Plane.h>

// GLToy
#include <Compression/GLToy_Compression.h>
#include <Core/Data Structures/GLToy_BitStream.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Plane::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    u_short usNormal;
    u_int uDistanceBits;

    xStream >> usNormal;
    xStream.ReadBits( uDistanceBits, 24 );

    m_xNormal = GLToy_Decompress::UnitVector_2Bytes( usNormal );
    m_fDistance = GLToy_Decompress::Float_3Bytes( uDistanceBits );
}

void GLToy_Plane::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << GLToy_Compress::UnitVector_2Bytes( m_xNormal );
    xStream.WriteBits( GLToy_Compress::Float_3Bytes( m_fDistance ), 24 );
}