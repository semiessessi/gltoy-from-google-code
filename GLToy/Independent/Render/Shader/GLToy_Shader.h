/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

#ifndef __GLTOY_SHADER_H_
#define __GLTOY_SHADER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <Maths/GLToy_Vector.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ShaderProgram
{

public:

    GLToy_ShaderProgram( const u_int uID, const u_int uFSID, const u_int uVSID );
    virtual ~GLToy_ShaderProgram();

    bool IsReady() const { return m_uID != 0; }

    void Bind() const;

    void SetUseNoise( const bool bUseNoise );

    GLToy_Handle GetUniformHandle( const GLToy_String& szName ) const;

    void SetUniform( const GLToy_String& szName, const int iValue ) const;
    void SetUniform( const GLToy_String& szName, const float fValue ) const;
    void SetUniform( const GLToy_String& szName, const float fValue1, const float fValue2 ) const;
    void SetUniform( const GLToy_String& szName, const float fValue1, const float fValue2, const float fValue3 ) const;
    void SetUniform( const GLToy_String& szName, const float fValue1, const float fValue2, const float fValue3, const float fValue4 ) const;
    GLToy_ForceInline void SetUniform( const GLToy_String& szName, const GLToy_Vector_2& xValue ) const { SetUniform( szName, xValue[ 0 ], xValue[ 1 ] ); }
    GLToy_ForceInline void SetUniform( const GLToy_String& szName, const GLToy_Vector_3& xValue ) const { SetUniform( szName, xValue[ 0 ], xValue[ 1 ], xValue[ 2 ] ); }
    GLToy_ForceInline void SetUniform( const GLToy_String& szName, const GLToy_Vector_4& xValue ) const { SetUniform( szName, xValue[ 0 ], xValue[ 1 ], xValue[ 2 ], xValue[ 3 ] ); }

    void SetUniform( const GLToy_Handle iID, const int iValue ) const;
    void SetUniform( const GLToy_Handle iID, const float fValue ) const;
    void SetUniform( const GLToy_Handle iID, const float fValue1, const float fValue2 ) const;
    void SetUniform( const GLToy_Handle iID, const float fValue1, const float fValue2, const float fValue3 ) const;
    void SetUniform( const GLToy_Handle iID, const float fValue1, const float fValue2, const float fValue3, const float fValue4 ) const;
    GLToy_ForceInline void SetUniform( const GLToy_Handle iID, const GLToy_Vector_2& xValue ) const { SetUniform( iID, xValue[ 0 ], xValue[ 1 ] ); }
    GLToy_ForceInline void SetUniform( const GLToy_Handle iID, const GLToy_Vector_3& xValue ) const { SetUniform( iID, xValue[ 0 ], xValue[ 1 ], xValue[ 2 ] ); }
    GLToy_ForceInline void SetUniform( const GLToy_Handle iID, const GLToy_Vector_4& xValue ) const { SetUniform( iID, xValue[ 0 ], xValue[ 1 ], xValue[ 2 ], xValue[ 3 ] ); }

    void SetViewMatrix( const GLToy_String& szName ) const;
    void SetInverseViewMatrix( const GLToy_String& szName ) const;

    void SetViewMatrix( const GLToy_Handle iID ) const;
    void SetInverseViewMatrix( const GLToy_Handle iID ) const;

protected:

    u_int m_uID;
    u_int m_uFSID;
    u_int m_uVSID;
	bool m_bUseNoise;
};

#endif
