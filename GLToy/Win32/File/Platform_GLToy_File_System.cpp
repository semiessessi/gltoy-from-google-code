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
