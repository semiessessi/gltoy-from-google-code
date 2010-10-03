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
//#include <io.h>
//#include <stdio.h>

// Win32
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< GLToy_String > GLToy_File_System::Platform_PathsFromFilter( const GLToy_String& szBasePath, const GLToy_String& szFilter, const bool bRecursive )
{

    GLToy_Array< GLToy_String > xPaths;

    char* szSearchString = ( GLToy_String( "./" ) + szBasePath + szFilter ).CreateANSIString();
    char* szRecurseString = ( GLToy_String( "./" ) + szBasePath + "*" ).CreateANSIString();

	// for the demo configuration _findfirst etc doesn't work
	// why not just not use these functions altogether...
    /*_finddata_t*/ WIN32_FIND_DATA xFindData;
    /*intptr_t*/ HANDLE xFilePtr;

    if( bRecursive )
    {
        xFilePtr = /*_findfirst*/FindFirstFile( szRecurseString, &xFindData );

        if( xFilePtr != /*-1L*/INVALID_HANDLE_VALUE  )
        {
            do
            {
                if( xFindData./*name*/cFileName[ 0 ] == '.' )
                {
                    // we always get back "." and ".."
                    continue;
                }

                if( xFindData./*attrib & _A_SUBDIR*/dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                {
                    GLToy_String szSubPath = szBasePath;
                    szSubPath += xFindData./*name*/cFileName;
                    szSubPath += "/";

                    xPaths.Append( Platform_PathsFromFilter( szSubPath, szFilter, true ) );
                }
            }
            while ( /*_findnext*/FindNextFile( xFilePtr, &xFindData )/* == 0*/ );
        }
    }

    xFilePtr = /*_findfirst*/FindFirstFile( szSearchString, &xFindData );

    if( xFilePtr != /*-1L*/INVALID_HANDLE_VALUE  )
    {
        do
        {
            if( xFindData./*name*/cFileName[ 0 ] == '.' )
            {
                // we always get back "." and ".."
                continue;
            }

            if( !( xFindData./*attrib & _A_SUBDIR*/dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
            {
                xPaths.Append( szBasePath + xFindData./*name*/cFileName );
            }
        }
        while ( /*_findnext*/FindNextFile( xFilePtr, &xFindData )/* == 0*/ );
    }

    delete[] szSearchString;
    delete[] szRecurseString;
    return xPaths;
}
