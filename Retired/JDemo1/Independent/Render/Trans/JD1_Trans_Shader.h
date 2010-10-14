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

#ifndef __JD1_TRANS_SHADER_H_
#define __JD1_TRANS_SHADER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/Trans/JD1_Trans.h>

// GLToy
#include <Core/Data Structures/GLToy_Pair.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class JD1_Trans_Shader
: public JD1_Trans
{

    typedef JD1_Trans GLToy_Parent;

public:

    JD1_Trans_Shader( const GLToy_Hash uShaderHash );

    virtual ~JD1_Trans_Shader() {}

    virtual void Render() const;

    virtual void SetUniforms() const {}

    void BindTexture( const GLToy_String& szUniformName, const GLToy_Hash uTextureHash, const u_int uTextureUnit = 0 );

protected:

    GLToy_Hash m_uShader;
    GLToy_Pair< GLToy_Hash, GLToy_String > m_axTextures[ 8 ];

};

#endif
