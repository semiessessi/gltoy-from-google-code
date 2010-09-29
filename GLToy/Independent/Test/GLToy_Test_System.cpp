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
#include <Test/GLToy_Test_System.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>

GLToy_Array< GLToy_Test_System::Test > GLToy_Test_System::s_xTests;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Test_System::RunTests()
{
    GLToy_DebugOutput_Release( "\r\nGLToy_Test_System::RunTests() - Starting tests\r\n" );
    u_int uFailCount = 0;
    GLToy_ConstIterate( Test, xIterator, &s_xTests )
    {
        const bool bResult = xIterator.Current().m_pfnTestFunction();
        GLToy_DebugOutput_Release( "  %S %s\r\n", xIterator.Current().m_szName.GetWideString(), bResult ? " - succeeded" : " - FAILED!!!" );
        if( !bResult )
        {
            ++uFailCount;
        }
    }
    
    if( uFailCount > 0 )
    {
        GLToy_DebugOutput_Release( "GLToy_Test_System::RunTests() - %d out of %d tests failed!\r\n", uFailCount, s_xTests.GetCount() );
        return false;
    }
    
    GLToy_DebugOutput_Release( "\r\nGLToy_Test_System::RunTests() - All tests passed\r\n" );
    return true;
}

void GLToy_Test_System::RegisterTest( bool ( *pfnTest )(), const GLToy_String& szName )
{
    Test xTest = { pfnTest, szName };
    s_xTests.Append( xTest );
}