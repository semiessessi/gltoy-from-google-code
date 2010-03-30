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
    char* szRecurseString = ( GLToy_String( "./" ) + szBasePath + "*" ).CreateANSIString();

	_finddata_t xFindData;
	intptr_t xFilePtr;

    if( bRecursive )
    {
        xFilePtr = _findfirst( szRecurseString, &xFindData );

	    if( xFilePtr != -1L )
	    {
	        do
	        {
		        if( xFindData.name[ 0 ] == '.' )
		        {
			        // we always get back "." and ".."
			        continue;
		        }

		        if( xFindData.attrib & _A_SUBDIR )
		        {
			        GLToy_String szSubPath = szBasePath;
                    szSubPath += xFindData.name;
                    szSubPath += "/";

                    xPaths.Append( Platform_PathsFromFilter( szSubPath, szFilter, true ) );
		        }
	        }
	        while ( _findnext( xFilePtr, &xFindData ) == 0 );
        }
    }

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

		    if( !( xFindData.attrib & _A_SUBDIR ) )
		    {
			    xPaths.Append( szBasePath + xFindData.name );
		    }
	    }
	    while ( _findnext( xFilePtr, &xFindData ) == 0 );
    }

    delete[] szSearchString;
    delete[] szRecurseString;
    return xPaths;
}