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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Light_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Light_System::s_bRenderLightBoxes = false;

GLToy_HashMap< GLToy_Light* > GLToy_Light_System::s_xLights;
GLToy_HashMap< GLToy_Light_Point* > GLToy_Light_System::s_xPointLights;
GLToy_HashMap< GLToy_Light_Projector* > GLToy_Light_System::s_xProjectorLights;
GLToy_Array< const GLToy_Renderable* > GLToy_Light_System::s_xOtherLightSources;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Light_System::Initialise()
{
    GLToy_Console::RegisterCommand( "spawn.pointlight", SpawnPointLight_Console );

    GLToy_Console::RegisterVariable( "render.lightboxes", &s_bRenderLightBoxes );

    return true;
}

void GLToy_Light_System::Shutdown()
{
    s_xLights.DeleteAll();
    s_xOtherLightSources.Clear();
    s_xPointLights.Clear();
    s_xProjectorLights.Clear();
}

void GLToy_Light_System::AddPointLight( const GLToy_Hash uHash, const GLToy_Light_PointProperties& xProperties )
{
    GLToy_Light_Point* const pxPointLight = new GLToy_Light_Point( xProperties );
    s_xLights.AddNode( pxPointLight, uHash );
    s_xPointLights.AddNode( pxPointLight, uHash );
}

void GLToy_Light_System::AddProjectorLight( const GLToy_Hash uHash, const GLToy_Light_ProjectorProperties& xProperties )
{
    GLToy_Light_Projector* const pxProjector = new GLToy_Light_Projector( xProperties );
    s_xLights.AddNode( pxProjector, uHash );
    s_xProjectorLights.AddNode( pxProjector, uHash );
}

void GLToy_Light_System::DestroyLight( const GLToy_Hash uHash )
{
    GLToy_Light** ppxLight = s_xLights.FindData( uHash );
    if( ppxLight )
    {
        delete *ppxLight;
        s_xLights.Remove( uHash );
        s_xPointLights.Remove( uHash );
        s_xProjectorLights.Remove( uHash );
    }
}

void GLToy_Light_System::RegisterLightSource( const GLToy_Renderable* const pxLightSource )
{
    s_xOtherLightSources.Append( pxLightSource );
}

void GLToy_Light_System::SpawnPointLight_Console()
{
    GLToy_Light_PointProperties xProperties;
    xProperties.m_xPosition = GLToy_Camera::GetPosition();
    xProperties.m_xColour = GLToy_Vector_3( 1.0f, 1.0f, 1.0f );
    xProperties.m_uFlags = 0;
    xProperties.m_fSphereRadius = 100.0f;
    AddPointLight( GLToy_Random_Hash(), xProperties );
}

void GLToy_Light_System::Render()
{
    GLToy_Render::StartSamplingDepth();

    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "Light_Point" ) );
    if( pxShader )
    {
        pxShader->Bind();
        pxShader->SetUniform( "xDiffuseSampler", 1 );
        pxShader->SetUniform( "xNormalSampler", 2 );
        pxShader->SetUniform( "xDepthSampler", 3 );

        pxShader->SetUniform( "xCameraPosition", GLToy_Camera::GetPosition() );
        pxShader->SetUniform( "xViewVector", GLToy_Camera::GetDirection() );

        const GLToy_Vector_2 xSize( static_cast< float >( GLToy::GetWindowViewportWidth() ), static_cast< float >( GLToy::GetWindowViewportHeight() ) );
        const GLToy_Vector_2 xOneOverSize( 1.0f / xSize[ 0 ], 1.0f / xSize[ 1 ] );

        pxShader->SetUniform( "xSize", xSize );
        pxShader->SetUniform( "xOneOverSize", xOneOverSize );
    }

    GLToy_Render::DisableDepthTesting();
    GLToy_Render::DisableDepthWrites();

    s_xPointLights.Traverse( GLToy_IndirectRenderLightingFunctor< GLToy_Light_Point >() );

    GLToy_ShaderProgram* const pxProjectorShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "Light_Projector" ) );
    if( pxProjectorShader )
    {
        pxProjectorShader->Bind();
        pxProjectorShader->SetUniform( "xTextureSampler", 0 );
        pxProjectorShader->SetUniform( "xDiffuseSampler", 1 );
        pxProjectorShader->SetUniform( "xNormalSampler", 2 );
        pxProjectorShader->SetUniform( "xDepthSampler", 3 );

        pxProjectorShader->SetUniform( "xCameraPosition", GLToy_Camera::GetPosition() );
        pxProjectorShader->SetUniform( "xViewVector", GLToy_Camera::GetDirection() );

        const GLToy_Vector_2 xSize( static_cast< float >( GLToy::GetWindowViewportWidth() ), static_cast< float >( GLToy::GetWindowViewportHeight() ) );
        const GLToy_Vector_2 xOneOverSize( 1.0f / xSize[ 0 ], 1.0f / xSize[ 1 ] );

        pxProjectorShader->SetUniform( "xSize", xSize );
        pxProjectorShader->SetUniform( "xOneOverSize", xOneOverSize );
    }

    s_xProjectorLights.Traverse( GLToy_IndirectRenderLightingFunctor< GLToy_Light_Projector >() );

    GLToy_Render::StopSamplingDepth();
    GLToy_Render::UseProgram( 0 );

    GLToy_Render::EnableDepthTesting();

    s_xOtherLightSources.Traverse( GLToy_IndirectRenderLightingFunctor< const GLToy_Renderable >() );
    s_xOtherLightSources.Clear();

    if( s_bRenderLightBoxes )
    {
        GLToy_Render::UseProgram( 0 );

        GLToy_ConstIterate( GLToy_Light*, pxLight, s_xLights )
            pxLight->RenderDebug();
        GLToy_Iterate_End;
    }
}

// SE - TODO: hook up somewhere
void GLToy_Light_System::Update()
{
    s_xLights.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Light >() );
}

void GLToy_Light_Point::RenderDebug() const
{
    GLToy_AABB xBox( GetPosition(), m_xProperties.m_fSphereRadius, m_xProperties.m_fSphereRadius, m_xProperties.m_fSphereRadius );
    xBox.Render();
}

void GLToy_Light_Point::RenderLighting() const
{
    // TODO: cache this, looking up for each light is... stupid
    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "Light_Point" ) );
    if( pxShader )
    {
        pxShader->SetUniform( "fLightRadius", m_xProperties.m_fSphereRadius );
        pxShader->SetUniform( "xLightColour", m_xProperties.m_xColour );
        pxShader->SetUniform( "xLightPosition", m_xProperties.m_xPosition );
    }

    GLToy_AABB xBox( GetPosition(), m_xProperties.m_fSphereRadius, m_xProperties.m_fSphereRadius, m_xProperties.m_fSphereRadius );
    xBox.RenderFlat();
}
