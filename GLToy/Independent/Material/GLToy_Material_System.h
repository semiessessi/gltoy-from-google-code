/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#ifndef __GLTOY_MATERIAL_SYSTEM_H_
#define __GLTOY_MATERIAL_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashMap;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Material
: public GLToy_CopyableStruct< GLToy_Material >
{

    typedef GLToy_CopyableStruct< GLToy_Material > GLToy_Parent;

public:

    GLToy_Material( const GLToy_Hash uHash );

    GLToy_Hash GetHash() const { return m_uHash; }
    GLToy_Hash GetShaderHash() const { return m_uShader; }
    u_int GetWidth() const;
    u_int GetHeight() const;

    bool IsDeferred() const { return m_bDeferred; }
    bool IsForward() const { return !m_bDeferred && !m_bTransparent; }
    bool IsTransparent() const { return m_bTransparent; }
    bool IsReady() const;

    void Initialise();

    void SetShaderHash( const GLToy_Hash uHash ) { m_uShader = uHash; }

    void SetDeferred() { m_bDeferred = true; m_bTransparent = false; }
    void SetForward() { m_bDeferred = false; m_bTransparent = false; }
    void SetTransparent() { m_bDeferred = false; m_bTransparent = true; }

    void AddTexture( const GLToy_Hash uHash, const GLToy_String& szName ) { u_int u = 0; while( ( u < 4 ) && m_auTextures[ u ] ) { ++u; } if( u < 4 ) { m_auTextures[ u ] = uHash; m_aszTextureNames[ u ] = szName; } }

    void Bind();
	
	GLToy_Hash GetTextureHash( const u_int uID ) const { return m_auTextures[ uID ]; }

private:

    // only allow 4 textures per material for now
    GLToy_Hash m_auTextures[ 4 ];
    GLToy_String m_aszTextureNames[ 4 ];
    GLToy_Handle m_aiSamplerHandles[ 4 ];
    GLToy_Handle m_aiViewMatrixHandles[ 2 ];
    GLToy_Handle m_iFogParamsHandle;
    GLToy_Hash m_uShader;
    const GLToy_Hash m_uHash;

    union
    {

        u_int m_uFlags;
        
        struct
        {
            bool    m_bDeferred     : 1;
            bool    m_bTransparent  : 1;
            bool    m_bInitialised  : 1;
        };

    };

};

class GLToy_Material_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();

    static GLToy_Material* FindMaterial( const GLToy_Hash uHash );
    static GLToy_Material* LookUpMaterial( const GLToy_String& szName );
    static GLToy_Material* LookUpMaterialNoExt( const GLToy_String& szName );
    static void BindMaterial( const GLToy_Hash uHash );
    static void BindMaterial( const GLToy_String& szName );

    static void BindBlack();
    static void BindWhite();

private:

    static GLToy_HashMap< GLToy_Material > s_xMaterials;

    static GLToy_Material* s_pxBlackMaterial;
    static GLToy_Material* s_pxWhiteMaterial;

};

#endif
