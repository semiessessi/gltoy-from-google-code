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
#include <Model/GLToy_Model_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_File_System.h>
#include <File/GLToy_3DSFile.h>
#include <File/GLToy_LWOFile.h>
#include <File/GLToy_MD2File.h>
#include <File/GLToy_OBJFile.h>
#include <File/GLToy_ModelFile.h>
#include <Model/GLToy_Model.h>
#include <Model/GLToy_Model_Placeholder.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_ModelFile* > GLToy_Model_System::s_xModels;
GLToy_Model_Placeholder GLToy_Model_System::s_xPlaceholder;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Model_System::Initialise()
{
    s_xModels.Clear();

    GLToy_Array< GLToy_String > xMD2Paths = GLToy_File_System::PathsFromFilter( "Models/", "*.md2" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xMD2Paths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 7 ); // remove "Models/"
        xName.RemoveFromEnd( 4 ); // remove .md2
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", xName.GetWideString() );

        s_xModels.AddNode( new GLToy_MD2File( xIterator.Current() ), xName.GetHash() );
    }

    GLToy_Array< GLToy_String > x3DSPaths = GLToy_File_System::PathsFromFilter( "Models/", "*.3ds" );

    GLToy_ConstIterate( GLToy_String, xIterator, &x3DSPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 7 ); // remove "Models/"
        xName.RemoveFromEnd( 4 ); // remove extension
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", xName.GetWideString() );

        s_xModels.AddNode( new GLToy_3DSFile( xIterator.Current() ), xName.GetHash() );
    }

    GLToy_Array< GLToy_String > xOBJPaths = GLToy_File_System::PathsFromFilter( "Models/", "*.obj" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xOBJPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 7 ); // remove "Models/"
        xName.RemoveFromEnd( 4 ); // remove extension
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", xName.GetWideString() );

        s_xModels.AddNode( new GLToy_OBJFile( xIterator.Current() ), xName.GetHash() );
    }

    GLToy_Array< GLToy_String > xLWOPaths = GLToy_File_System::PathsFromFilter( "Models/", "*.lwo" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xLWOPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 7 ); // remove "Models/"
        xName.RemoveFromEnd( 4 ); // remove extension
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", xName.GetWideString() );

        s_xModels.AddNode( new GLToy_LWOFile( xIterator.Current() ), xName.GetHash() );
    }

    return true;
}

void GLToy_Model_System::Shutdown()
{
    s_xModels.DeleteAll();
}

GLToy_Model* GLToy_Model_System::LoadModel( const GLToy_Hash uHash )
{
    GLToy_ModelFile** ppxModelFile = s_xModels.FindData( uHash );
    return ppxModelFile ? ( *ppxModelFile )->LoadModel() : &s_xPlaceholder;
}

GLToy_Model* GLToy_Model_System::LoadModel( const GLToy_String& xName )
{
    return LoadModel( xName.GetHash() );
}
