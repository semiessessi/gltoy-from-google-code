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
#include <Entity/Model/GLToy_Entity_ModelAnimated.h>

// GLToy
#include <Model/GLToy_Model.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Entity_ModelAnimated::GLToy_Entity_ModelAnimated( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_pxAnimStack( NULL )
{
}

GLToy_Entity_ModelAnimated::~GLToy_Entity_ModelAnimated()
{
    delete m_pxAnimStack;
}

void GLToy_Entity_ModelAnimated::Render() const
{
    if( !IsActive() )
    {
        return;
    }

    if( m_pxAnimStack )
    {
        m_pxAnimStack->Evaluate( m_pxModel );
    }

    if( m_pxModel )
    {
        m_pxModel->RenderWithPositionAndOrientation( GetPosition(), GetOrientation() );
    }
}

void GLToy_Entity_ModelAnimated::Update()
{
    if( !m_pxAnimStack && m_pxModel )
    {
        m_pxAnimStack = m_pxModel->CreateAnimationStack();
        PlayAnimation();
    }
    if( m_pxAnimStack )
    {
        m_pxAnimStack->Update();
    }
}

void GLToy_Entity_ModelAnimated::PlayAnimation( const u_int uAnimID, const float fTweenInTime, const float fTweenOutTime )
{
    if( m_pxAnimStack && m_pxAnimStack->SupportsAnimID( m_pxModel, uAnimID ) )
    {
        m_pxAnimStack->Push( uAnimID, fTweenInTime, fTweenOutTime );
    }
}
