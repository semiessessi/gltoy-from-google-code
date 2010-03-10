#ifndef __GLTOY_FILESYSTEM_H_
#define __GLTOY_FILESYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_File_System
{

public:

    static GLToy_Array< GLToy_String > PathsFromFilter( const GLToy_String& szBasePath, const GLToy_String& szFilter, const bool bRecursive = true );

private:
    
    static GLToy_Array< GLToy_String > Platform_PathsFromFilter( const GLToy_String& szBasePath, const GLToy_String& szFilter, const bool bRecursive );

};

#endif