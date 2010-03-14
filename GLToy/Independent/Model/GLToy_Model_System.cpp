/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_File_System.h>
#include <File/GLToy_MD2File.h>
#include <File/GLToy_ModelFile.h>
#include <Model/GLToy_Model.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_ModelFile* > GLToy_Model_System::s_xModels;

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

    return true;
}

void GLToy_Model_System::Shutdown()
{
    s_xModels.DeleteAll();
    s_xModels.Clear();
}

GLToy_Model* GLToy_Model_System::LoadModel( const GLToy_Hash uHash )
{
    GLToy_ModelFile** ppxModelFile = s_xModels.FindData( uHash );
    return ppxModelFile ? ( *ppxModelFile )->LoadModel() : NULL;
}

GLToy_Model* GLToy_Model_System::LoadModel( const GLToy_String& xName )
{
    return LoadModel( xName.GetHash() );
}