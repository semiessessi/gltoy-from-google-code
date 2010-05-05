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
