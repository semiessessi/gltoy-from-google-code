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

#ifndef __GLTOY_TEXTURE_RENDERABLE_H_
#define __GLTOY_TEXTURE_RENDERABLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture_Renderable
{

public:

    GLToy_Texture_Renderable( const u_int uWidth = 512, const u_int uHeight = 512 )
    : m_uWidth( uWidth )
    , m_uHeight( uHeight )
    , m_iID( -1 )
    , m_iDepthBuffer( -1 )
    , m_uFrameBuffer( -1 )
    {
    }

    virtual ~GLToy_Texture_Renderable() {}

    u_int GetHeight() const { return m_uHeight; }
    u_int GetWidth() const { return m_uWidth; }

    GLToy_Inline void Create() { Platform_Create(); }
    GLToy_Inline void Destroy() { Platform_Destroy(); }
    GLToy_Inline void Bind( const u_int uTextureUnit = 0 ) const { Platform_Bind( uTextureUnit ); }
    GLToy_Inline void BeginRender() const { Platform_BeginRender(); }
    GLToy_Inline void EndRender() const { Platform_EndRender(); }

protected:

    void Platform_Create();
    void Platform_Destroy();
    void Platform_Bind( const u_int uTextureUnit ) const;
    void Platform_BeginRender() const;
    void Platform_EndRender() const;

    u_int m_uWidth;
    u_int m_uHeight;

    union
    {
        int m_iID;
        u_int m_uID;
    };

    union
    {
        int m_iDepthBuffer;
        u_int m_uDepthBuffer;
    };

    union
    {
        int m_iFrameBuffer;
        u_int m_uFrameBuffer;
    };

};

#endif