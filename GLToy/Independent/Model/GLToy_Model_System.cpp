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
#include <Model/GLToy_Model_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashMap.h>
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

GLToy_HashMap< GLToy_ModelFile* > GLToy_Model_System::s_xModels;
GLToy_Model_Placeholder GLToy_Model_System::s_xPlaceholder;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Model_System::Initialise()
{
    s_xModels.Clear();

    GLToy_Array< GLToy_String > xMD2Paths = GLToy_File_System::PathsFromFilter( "Models/", "*.md2" );

    GLToy_ConstIterate( GLToy_String, szPath, xMD2Paths )
        GLToy_String szName = szPath;
        szName.RemoveAt( 0, 7 ); // remove "Models/"
        szName.RemoveFromEnd( 4 ); // remove .md2
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", szName.GetWideString() );

        s_xModels.AddNode( new GLToy_MD2File( szPath ), szName.GetHash() );
    GLToy_Iterate_End;

    GLToy_Array< GLToy_String > x3DSPaths = GLToy_File_System::PathsFromFilter( "Models/", "*.3ds" );

    GLToy_ConstIterate( GLToy_String, szPath, x3DSPaths )
        GLToy_String szName = szPath;
        szName.RemoveAt( 0, 7 ); // remove "Models/"
        szName.RemoveFromEnd( 4 ); // remove extension
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", szName.GetWideString() );

        s_xModels.AddNode( new GLToy_3DSFile( szPath ), szName.GetHash() );
    GLToy_Iterate_End;

    GLToy_Array< GLToy_String > xOBJPaths = GLToy_File_System::PathsFromFilter( "Models/", "*.obj" );

    GLToy_ConstIterate( GLToy_String, szPath, xOBJPaths )
        GLToy_String szName = szPath;
        szName.RemoveAt( 0, 7 ); // remove "Models/"
        szName.RemoveFromEnd( 4 ); // remove extension
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", szName.GetWideString() );

        s_xModels.AddNode( new GLToy_OBJFile( szPath ), szName.GetHash() );
    GLToy_Iterate_End;

    GLToy_Array< GLToy_String > xLWOPaths = GLToy_File_System::PathsFromFilter( "Models/", "*.lwo" );

    GLToy_ConstIterate( GLToy_String, szPath, xLWOPaths )
        GLToy_String szName = szPath;
        szName.RemoveAt( 0, 7 ); // remove "Models/"
        szName.RemoveFromEnd( 4 ); // remove extension
        
        GLToy_DebugOutput( "   - Found model \"%S\".\r\n", szName.GetWideString() );

        s_xModels.AddNode( new GLToy_LWOFile( szPath ), szName.GetHash() );
    GLToy_Iterate_End;

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
