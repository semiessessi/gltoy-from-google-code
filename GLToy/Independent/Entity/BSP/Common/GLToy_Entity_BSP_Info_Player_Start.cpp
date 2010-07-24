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
