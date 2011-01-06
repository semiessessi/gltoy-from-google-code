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
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashMap< GLToy_Light* > GLToy_Light_System::s_xLights;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Light_System::Initialise()
{
    return true;
}

void GLToy_Light_System::Shutdown()
{
    s_xLights.DeleteAll();
}

void GLToy_Light_System::AddPointLight( const GLToy_Hash uHash, const GLToy_Light_PointProperties& xProperties )
{
    s_xLights.AddNode( new GLToy_Light_Point( xProperties ), uHash );
}

void GLToy_Light_System::AddProjectorLight( const GLToy_Hash uHash, const GLToy_Light_ProjectorProperties& xProperties )
{
    s_xLights.AddNode( new GLToy_Light_Projector( xProperties ), uHash );
}

void GLToy_Light_System::DestroyLight( const GLToy_Hash uHash )
{
    GLToy_Light** ppxLight = s_xLights.FindData( uHash );
    if( ppxLight )
    {
        delete *ppxLight;
        s_xLights.Remove( uHash );
    }
}

void GLToy_Light_System::Render()
{
    s_xLights.Traverse( GLToy_IndirectRenderFunctor< GLToy_Light >() );
}

// SE - TODO: hook up somewhere
void GLToy_Light_System::Update()
{
    s_xLights.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Light >() );
}
