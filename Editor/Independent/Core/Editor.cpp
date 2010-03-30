/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of Editor.
//
// Editor is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Editor is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with Editor.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/Editor.h>

// GLToy
#include <Core/State/GLToy_State_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Editor::Initialise()
{
    if( GLToy_State_System::ChangeState( GLToy_Hash_Constant( "EditorFrontEnd" ) ) )
    {
        return true;
    }

    GLToy_Assert( false, "Failed to enter state \"EditorFrontEnd\"." );

    GLToy_DebugOutput_Release( "Fatal error - could not enter edtior front end state!" );
    
    return false;
}

void Editor::Shutdown()
{
}