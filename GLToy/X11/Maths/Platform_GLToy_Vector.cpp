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
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_GLToy_Vector::Initialise()
{
    return true;
}

GLToy_Vector_3 Platform_GLToy_Vector::Add(const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2)
{
    return GLToy_Vector_3( xVector1[0] + xVector2[0], xVector1[1] + xVector2[1], xVector1[2] + xVector2[2] );
}

GLToy_Vector_3 Platform_GLToy_Vector::Sub(const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2)
{
    return GLToy_Vector_3( xVector1[0] - xVector2[0], xVector1[1] - xVector2[1], xVector1[2] - xVector2[2] );
}

GLToy_Vector_3 Platform_GLToy_Vector::Mul( const float fFloat, const GLToy_Vector_3& xVector )
{
    return GLToy_Vector_3( fFloat * xVector[0], fFloat * xVector[1], fFloat * xVector[2] );
}

