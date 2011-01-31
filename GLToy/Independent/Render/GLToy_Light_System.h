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

#ifndef __GLTOY_LIGHT_SYSTEM_H_
#define __GLTOY_LIGHT_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Updateable.h>
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Renderable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class DataType, class KeyType > class GLToy_BinaryTree;
template < class DataType > class GLToy_HashMap;
class GLToy_ShaderProgram;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// SE - NOTE: there is a lot of forward thinking and design I just can't be bothered to explain...
// just trust it will get finished quickly :)

// evil macro }:)
#define GLToy_LightPropertiesDefinition\
    GLToy_Vector_3  m_xPosition;\
    GLToy_Vector_3  m_xColour;\
    union\
    {\
        u_int m_uFlags;\
        struct\
        {\
            bool            m_bSpecular;\
            bool            m_bShadow;\
            bool            m_bGlow;\
            bool            m_bLensFlare;\
        };\
    }

struct GLToy_LightProperties
{

    GLToy_LightPropertiesDefinition;

};

struct GLToy_Light_PointProperties
: public GLToy_CopyableStruct< GLToy_Light_PointProperties >
{

    GLToy_LightPropertiesDefinition;

    float           m_fSphereRadius;

};

struct GLToy_Light_ProjectorProperties
: public GLToy_CopyableStruct< GLToy_Light_ProjectorProperties >
{

    GLToy_LightPropertiesDefinition;

    GLToy_Matrix_3  m_xOrientation;
    float           m_fAngleX;
    float           m_fAngleY;
    GLToy_Hash      m_uTexture;

};

#undef GLToy_LightPropertiesDefinition

class GLToy_Light
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

    virtual void Render() const {}
    virtual void RenderDebug() const {}
    virtual void Update() {}

    const GLToy_Vector_3& GetColour() const { return m_pxProperties->m_xColour; }
    const GLToy_Vector_3& GetPosition() const { return m_pxProperties->m_xPosition; }

    bool EmitsSpecular() const { return m_pxProperties->m_bSpecular; }
    bool EmitsShadows() const { return m_pxProperties->m_bShadow; }
    bool EmitsGlows() const { return m_pxProperties->m_bGlow; }
    bool EmitsLensFlares() const { return m_pxProperties->m_bLensFlare; }

    void SetPosition( const GLToy_Vector_3& xPosition ) { m_pxProperties->m_xPosition = xPosition; }

protected:

    GLToy_Light( void* const pxProperties )
    : m_pxProperties( reinterpret_cast< GLToy_LightProperties* const >( pxProperties ) )
    {
    }

private:

    GLToy_LightProperties* const m_pxProperties;

};

class GLToy_Light_Point
: public GLToy_Light
{

    typedef GLToy_Light GLToy_Parent;

public:

    GLToy_Light_Point( const GLToy_Light_PointProperties& xProperties )
    : GLToy_Parent( &m_xProperties )
    , m_xProperties( xProperties )
    {
    }

    virtual void RenderDebug() const;
    virtual void RenderLighting() const;

private:
    
    GLToy_Light_PointProperties m_xProperties;
    
};

class GLToy_Light_Projector
: public GLToy_Light
{
    typedef GLToy_Light GLToy_Parent;

public:

    GLToy_Light_Projector( const GLToy_Light_ProjectorProperties& xProperties )
    : GLToy_Parent( &m_xProperties )
    , m_xProperties( xProperties )
    {
    }

private:
    
    GLToy_Light_ProjectorProperties m_xProperties;

};

class GLToy_GlobalLight_Directional
{

public:

	GLToy_GlobalLight_Directional( const GLToy_Vector_3& xDirection, const GLToy_Vector_3& xColour )
	: m_xDirection( xDirection )
	, m_xColour( xColour )
	{
	}

	const GLToy_Vector_3& GetDirection() const { return m_xDirection; }
	const GLToy_Vector_3& GetColour() const { return m_xColour; }

	void RenderLighting() const;

private:

	const GLToy_Vector_3 m_xDirection;
	const GLToy_Vector_3 m_xColour;

};

class GLToy_Light_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Reset() { Shutdown(); Initialise(); }

    static void Render();
    static void Update();

	static void AddGlobalDirectionalLight( const GLToy_Vector_3& xDirection, const GLToy_Vector_3& xColour );
    static void AddPointLight( const GLToy_Hash uHash, const GLToy_Light_PointProperties& xProperties );
    static void AddProjectorLight( const GLToy_Hash uHash, const GLToy_Light_ProjectorProperties& xProperties );
    static void DestroyLight( const GLToy_Hash uHash );
    static GLToy_Light* FindLight( const GLToy_Hash uHash );

	static GLToy_ShaderProgram* GetCurrentShader() { return s_pxCurrentShader; }

    static void RegisterLightSource( const GLToy_Renderable* const pxLightSource );

    static void TestDirectionalLight_Console();
	static void SpawnPointLight_Console();

private:

    static bool s_bRenderLightBoxes;
	static GLToy_ShaderProgram* s_pxCurrentShader;

    static GLToy_HashMap< GLToy_Light* > s_xLights;
    static GLToy_HashMap< GLToy_Light_Point* > s_xPointLights;
    static GLToy_HashMap< GLToy_Light_Projector* > s_xProjectorLights;
    static GLToy_Array< const GLToy_Renderable* > s_xOtherLightSources;
	static GLToy_Array< const GLToy_GlobalLight_Directional* > s_xDirectionalLights;

};

#endif
