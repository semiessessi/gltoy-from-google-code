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

#include <Core/JD1.h>

// This file's header
#include <Render/Trans/JD1_Trans_Flow.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_Trans_Flow::SetUniforms() const
{
    const float fT = GLToy_Timer::GetTime();

    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( m_uShader );
    pxShader->SetUniform( "xPoint0", GLToy_Vector_2( GLToy_Maths::Cos( fT ), GLToy_Maths::Sin( fT ) ) );
    pxShader->SetUniform( "xPoint1", GLToy_Vector_2( 0.5f * GLToy_Maths::Cos( -fT ), 0.5f * GLToy_Maths::Sin( -fT ) ) );
    pxShader->SetUniform( "xPoint2", GLToy_Vector_2( GLToy_Maths::Cos( -0.989f * fT ), GLToy_Maths::Sin( -1.321f * fT ) ) );
    pxShader->SetUniform( "xPoint3", GLToy_Vector_2( 0.5f * GLToy_Maths::Cos( 0.337f * fT ), 0.5f * GLToy_Maths::Sin( 0.297f * fT ) ) );
}
