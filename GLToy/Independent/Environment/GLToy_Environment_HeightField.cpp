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
#include <Environment/GLToy_Environment_HeightField.h>

// GLToy
#include <Environment/GLToy_Environment_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Environment_HeightField::ReadFromBitStream( const GLToy_BitStream& xStream )
{
}

void GLToy_Environment_HeightField::WriteToBitStream( GLToy_BitStream& xStream ) const
{
}

void GLToy_Environment_HeightField::Initialise()
{
    //m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsHeightField( GLToy_Hash_Constant( "Environment" ), GLToy_HeightField( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ) );
}

void GLToy_Environment_HeightField::Shutdown()
{
}

void GLToy_Environment_HeightField::Render() const
{
}

void GLToy_Environment_HeightField::Update()
{
}

u_int GLToy_Environment_HeightField::GetType() const
{
    return ENV_HEIGHTFIELD;
}

float GLToy_Environment_HeightField::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
	// TODO: implement
	return 0.0f;
}
