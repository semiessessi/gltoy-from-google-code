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

// This file's header
#include <Model/GLToy_Model_3DS.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_3DS::GLToy_Model_3DS()
: m_xObjects()
, m_pxTexture( NULL )
{
}

void GLToy_Model_3DS::Render() const
{
    if( m_pxTexture )
    {
        m_pxTexture->Bind();
    }
    else
    {
        GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "Generic/Invalid.png" ) );
    }

    GLToy_ConstIterate( GLToy_3DS_Object, xIterator, &m_xObjects )
    {
        GLToy_Render::StartSubmittingTriangles();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xVertexIterator, &( xIterator.Current().m_xIndices ) )
        {
            GLToy_Render::SubmitVertex( xIterator.Current().m_xVertices[ xVertexIterator.Current() ] );
        }

        GLToy_Render::EndSubmit();
    }
}