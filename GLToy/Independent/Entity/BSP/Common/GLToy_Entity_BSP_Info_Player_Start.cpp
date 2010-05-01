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
#include <Entity/BSP/Common/GLToy_Entity_BSP_Info_Player_Start.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>
#include <Render/GLToy_Camera.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Entity_BSP_Info_Player_Start::SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue )
{
    if( szKey == "origin" )
    {
        GLToy_Array< GLToy_String > xComponents = szValue.Split( ' ' );
        if( xComponents.GetCount() < 3 )
        {
            // TODO - error feedback
            return;
        }

        // convert from BSP basis and offset
        // TODO: something better
        SetPosition( GLToy_Vector_3( -( xComponents[ 1 ].ExtractFloat() ), xComponents[ 2 ].ExtractFloat(), xComponents[ 0 ].ExtractFloat() ) + GLToy_Vector_3( 0.0f, 48.0f, 0.0f ) );
        GLToy_Camera::SetPosition( GetPosition() );
        GLToy_Camera::SetControllerCamEnabled( true );
    }
}
