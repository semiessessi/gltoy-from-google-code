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

void GLToy_Entity_ModelAnimated::RenderDeferred() const
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
        m_pxModel->RenderDeferredWithPositionAndOrientation( GetPosition(), GetOrientation() );
    }
}

void GLToy_Entity_ModelAnimated::ForceAnimStackCreation()
{
	if( !m_pxAnimStack )
	{
		m_pxAnimStack = m_pxModel->CreateAnimationStack();
	}
}

void GLToy_Entity_ModelAnimated::Update()
{
    if( !m_pxAnimStack && m_pxModel )
    {
        ForceAnimStackCreation();
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
