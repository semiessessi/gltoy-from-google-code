/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_File_System.h>

// C/C++
#include <io.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< GLToy_String > GLToy_File_System::Platform_PathsFromFilter( const GLToy_String& szBasePath, const GLToy_String& szFilter, const bool bRecursive )
{
    GLToy_Array< GLToy_String > xPaths;

    char* szSearchString = ( GLToy_String( "./" ) + szBasePath + szFilter ).CreateANSIString();

	_finddata_t xFindData;
	intptr_t xFilePtr;

	xFilePtr = _findfirst( szSearchString, &xFindData );

	if( xFilePtr != -1L )
	{
	    do
	    {
		    if( xFindData.name[ 0 ] == '.' )
		    {
			    // we always get back "." and ".."
			    continue;
		    }

		    if( bRecursive &&( xFindData.attrib & _A_SUBDIR ) )
		    {
			    if( bRecursive )
			    {
				    GLToy_String szSubPath = szBasePath;
                    szSubPath += xFindData.name;
                    szSubPath += "/";

                    xPaths.Append( Platform_PathsFromFilter( szSubPath, szFilter, true ) );
			    }
		    }
		    else
		    {
			    xPaths.Append( szBasePath + xFindData.name );
		    }
	    }
	    while ( _findnext( xFilePtr, &xFindData ) == 0 );
    }

    delete[] szSearchString;
    return xPaths;
}