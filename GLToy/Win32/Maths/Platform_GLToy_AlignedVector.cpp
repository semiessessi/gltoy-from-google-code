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

// This file's headers
#include <Maths/GLToy_AlignedVector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_AlignedVector_Base::Platform_Copy4( const GLToy_AlignedVector_Data& xData )
{
}

void GLToy_AlignedVector_Base::Platform_Copy4( const float* pfData )
{
}

void GLToy_AlignedVector_Base::Platform_Set1( const float fValue )
{
	( *m_pxComponents )[ 0 ] = fValue;
}

void GLToy_AlignedVector_Base::Platform_Set2( const float fValue1, const float fValue2 )
{
	( *m_pxComponents )[ 0 ] = fValue1;
	( *m_pxComponents )[ 1 ] = fValue2;
}

void GLToy_AlignedVector_Base::Platform_Set3( const float fValue1, const float fValue2, const float fValue3 )
{
	( *m_pxComponents )[ 0 ] = fValue1;
	( *m_pxComponents )[ 1 ] = fValue2;
	( *m_pxComponents )[ 2 ] = fValue3;
}

void GLToy_AlignedVector_Base::Platform_Set4( const float fValue1, const float fValue2, const float fValue3, const float fValue4 )
{
	( *m_pxComponents )[ 0 ] = fValue1;
	( *m_pxComponents )[ 1 ] = fValue2;
	( *m_pxComponents )[ 2 ] = fValue3;
	( *m_pxComponents )[ 3 ] = fValue4;
}

void GLToy_AlignedVector_Base::Platform_Add4( const GLToy_AlignedVector_Data& xData )
{
}

void GLToy_AlignedVector_Base::Platform_Sub4( const GLToy_AlignedVector_Data& xData )
{
}

float GLToy_AlignedVector_Base::Platform_Dot2( const GLToy_AlignedVector_Data& xData ) const
{
	return xData[ 0 ] * ( *m_pxComponents )[ 0 ]
		+ xData[ 1 ] * ( *m_pxComponents )[ 1 ];
}

float GLToy_AlignedVector_Base::Platform_Dot3( const GLToy_AlignedVector_Data& xData ) const
{
	return xData[ 0 ] * ( *m_pxComponents )[ 0 ]
		+ xData[ 1 ] * ( *m_pxComponents )[ 1 ]
		+ xData[ 2 ] * ( *m_pxComponents )[ 2 ];
}

float GLToy_AlignedVector_Base::Platform_Dot4( const GLToy_AlignedVector_Data& xData ) const
{
	return xData[ 0 ] * ( *m_pxComponents )[ 0 ]
		+ xData[ 1 ] * ( *m_pxComponents )[ 1 ]
		+ xData[ 2 ] * ( *m_pxComponents )[ 2 ]
		+ xData[ 3 ] * ( *m_pxComponents )[ 3 ];
}
