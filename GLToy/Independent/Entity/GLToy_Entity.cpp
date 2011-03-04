/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Compression/GLToy_Compression.h>
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Material/GLToy_Material_System.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Entity::ReadFromBitStream( const GLToy_BitStream& xStream )
{   
    // entity file does this for us
    //u_char ucType;
    //xStream >> ucType;
    //xStream >> m_uHash;
    xStream >> m_bActive;
}

void GLToy_Entity::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    // entity file may load these, but write them here
    // TODO - remove this asymmetry
    xStream << static_cast< u_char >( m_uType );
    xStream << m_uHash;
    xStream << m_bActive;
}

void GLToy_Entity_AABB::RenderAABB() const
{
    m_xBoundingBox.Render();
}

void GLToy_Entity_Oriented_AABB::RenderOBB() const
{
    RenderAABB();
}

void GLToy_Entity_AABB::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_xBoundingBox;
}

void GLToy_Entity_AABB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xBoundingBox;
}

void GLToy_Entity_Oriented_AABB::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    u_int uOri = 0;
    xStream >> uOri;

    SetOrientation( GLToy_Decompress::OrthonormalMatrix_4Bytes( uOri ) );
}

void GLToy_Entity_Oriented_AABB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << GLToy_Compress::OrthonormalMatrix_4Bytes( GetOrientation() );
}

void GLToy_Entity_Sphere::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_xBoundingSphere;
}

void GLToy_Entity_Sphere::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xBoundingSphere;
}

void GLToy_Entity_AABB::BoundsFromMaterial( const GLToy_Hash uMaterialHash, const float fBaseSize )
{
	const GLToy_Material* const pxMaterial = GLToy_Material_System::FindMaterial( uMaterialHash );
	if( !pxMaterial )
	{
		m_xBoundingBox.SetRadius( fBaseSize );
		return;
	}
	
	const GLToy_Texture* const pxTexture = GLToy_Texture_System::FindTexture( pxMaterial->GetTextureHash( 0 ) );
	if( !pxTexture )
	{
		m_xBoundingBox.SetRadius( fBaseSize );
		return;
	}
	
	u_int uMinX = pxTexture->GetWidth() - 1, uMaxX = 0, uMinY = pxTexture->GetHeight() - 1, uMaxY = 0;
	for( u_int u = 0; u < pxTexture->GetWidth(); ++u )
	for( u_int v = 0; v < pxTexture->GetHeight(); ++v )
	{
		// does it have non-zero alpha?
		if( pxTexture->GetPixel( u, v ) & 0xFF000000 )
		{
			uMinX = ( u < uMinX ) ? u : uMinX;
			uMaxX = ( u > uMinX ) ? u : uMaxX;
			uMinY = ( v < uMinY ) ? v : uMinY;
			uMaxY = ( v > uMinY ) ? v : uMaxY;
		}
	}
	
	const float fCentreX = static_cast< float >( pxTexture->GetWidth() ) * 0.5f;
	const float fCentreY = static_cast< float >( pxTexture->GetHeight() ) * 0.5f;
	
	const float fDistanceX = GLToy_Maths::Max( fCentreX - static_cast< float >( uMinX ), static_cast< float >( uMaxX ) - fCentreX ) / fCentreX;
	const float fDistanceY = GLToy_Maths::Max( fCentreY - static_cast< float >( uMinY ), static_cast< float >( uMaxY ) - fCentreY ) / fCentreY;
	
	m_xBoundingBox.SetHalfExtents( GLToy_Vector_3( fDistanceX * fBaseSize, fDistanceY * fBaseSize, fBaseSize ) );
}