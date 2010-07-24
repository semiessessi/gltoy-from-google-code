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
#include <Entity/BSP/Common/GLToy_Entity_BSP_Func_Button.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Entity_BSP_Func_Button::Trigger( const GLToy_Hash uTriggerHash )
{
    GLToy_Entity* pxEntity = GLToy_Entity_System::FindEntity( m_uTarget );
    if( pxEntity )
    {
        pxEntity->Trigger( uTriggerHash );
    }
    else
    {
        GLToy_Assert( pxEntity != NULL, "Entity 0x%X tried triggering non-existant entity 0x%X", GetHash(), m_uTarget );
    }
}

void GLToy_Entity_BSP_Func_Button::Hurt( const GLToy_Hash uTriggerHash, const float fAmount )
{
    GLToy_Parent::Hurt( uTriggerHash, fAmount );

    if( IsDead() )
    {
        Trigger( uTriggerHash );
    }
}

void GLToy_Entity_BSP_Func_Button::SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue )
{
    if( szKey == "origin" )
    {
        GLToy_Array< GLToy_String > xComponents = szValue.Split( ' ' );
        if( xComponents.GetCount() < 3 )
        {
            // TODO - error feedback
            return;
        }

        SetPosition( GLToy_Vector_3( xComponents[ 0 ].ExtractFloat(), xComponents[ 1 ].ExtractFloat(), xComponents[ 2 ].ExtractFloat() ) );
    }
    else if( szKey == "target" )
    {
        m_uTarget = szValue.GetHash();
    }
    else if( szKey == "health" )
    {
        SetHealth( szValue.ExtractFloat() );
    }
    else if( szKey == "angle" )
    {
        // m_fAngle = szValue.ExtractFloat();
    }
    else if( szKey == "wait" )
    {
        // m_fWait = szValue.ExtractFloat();
    }
}
