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
#include <File/GLToy_File_System.h>

// C/C++
#include <dirent.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< GLToy_String > GLToy_File_System::Platform_PathsFromFilter( const GLToy_String& szBasePath, const GLToy_String& szFilter, const bool bRecursive )
{
	// GLToy_DebugOutput( GLToy_String( "GLToy_File_System::Platform_PathsFromFilter szBasePath = " ) + szBasePath + " szFilter = " + szFilter + "\r\n" );
    GLToy_Array< GLToy_String > xPaths;

    // TODO: implement for Linux
	const char* const szANSIPath = szBasePath.CreateANSIString();
	DIR* xDirectory = opendir( szANSIPath );
	delete[] szANSIPath;

	if( !xDirectory )
	{
        return xPaths;
    }

	// TODO: improve this
	// for now only support two types of filter - an exact match and those with a single leading *
	const bool bExact = !szFilter.Contains( '*' );
	const bool bEnding = !bExact && szFilter.BeginsWith( "*" );

	dirent* pxEntry;
	if( bExact )
	{
		do
		{
			pxEntry = readdir( xDirectory );
			if( !pxEntry )
			{
				break;
			}

			const char* const szName = pxEntry->d_name;
			if( !szName )
			{
				continue;
			}

			// is the entry a directory?
			DIR* xSubDirectory = opendir( szName );
			if( xSubDirectory
	    		&& ( GLToy_String( ".." ) != szName )
				&& ( GLToy_String( "." ) != szName ) )
			{
				GLToy_String szSubPath = szBasePath;
				szSubPath += szName;
				szSubPath += "/";

				xPaths.Append( Platform_PathsFromFilter( szSubPath, szFilter, true ) );
			}
			else if( szFilter == szName )
			{
				xPaths.Append( szBasePath + szName );
			}
		}
		while( true );
	}
	else if( bEnding )
	{
		GLToy_String szEnding = szFilter;
		szEnding.RemoveAt( 0 );
		do
		{
			pxEntry = readdir( xDirectory );
			if( !pxEntry )
			{
				break;
			}

			const char* const szName = pxEntry->d_name;
			if( !szName )
			{
				continue;
			}

			// is the entry a directory?
			DIR* xSubDirectory = opendir( szName );
			if( xSubDirectory
				&& ( GLToy_String( ".." ) != szName )
				&& ( GLToy_String( "." ) != szName ) )
			{
				GLToy_String szSubPath = szBasePath;
				szSubPath += szName;
				szSubPath += "/";

				xPaths.Append( Platform_PathsFromFilter( szSubPath, szFilter, true ) );
			}
			else if( GLToy_String( szName ).EndsWith( szEnding ) )
			{
				xPaths.Append( szBasePath + szName );
			}
		}
		while( true );		
	}

	closedir( xDirectory );

    return xPaths;
}
