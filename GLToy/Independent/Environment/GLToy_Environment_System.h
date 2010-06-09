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

#ifndef __GLTOY_ENVIRONMENT_SYSTEM_H_
#define __GLTOY_ENVIRONMENT_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashTree;
class GLToy_Environment;
class GLToy_EnvironmentFile;
class GLToy_Ray;
class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

enum GLToy_EnvironmentType
{
    ENV_PLANE = 0,
    ENV_LIGHTMAPPED = 1,

    NUM_ENV_TYPES
};

class GLToy_Environment_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();
    static void Render();

    static void Render2D();

    static float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = -1.0f );

    static void LoadEnvironmentFile( const GLToy_String& szName );
    static void SaveEnvironmentFile( const GLToy_String& szName );

    static void CreateTestEnvironment();
    static GLToy_Environment* CreateEnvironmentFromType( const GLToy_EnvironmentType eType );

    static const GLToy_Environment* GetCurrentEnvironment() { return s_pxCurrentEnvironment; }
    static void SetRender( const bool bRender ) { s_bRender = bRender; };

    static void SwitchEnvironment( GLToy_Environment* const pxEnv );

    static void ClearCurrentEnvironment();
    static void SetCurrentEnvironment( GLToy_Environment* const pxEnv );

    static GLToy_HashTree< GLToy_EnvironmentFile* >& GetEnvironmentFileTree() { return s_xEnvironments; }

    static bool IsRenderingLightmap() { return s_bRenderLightmap; }
    static bool IsRenderingLightmapOnly() { return s_bRenderLightmapOnly; }

    static bool IsBSPQuadRes() { return s_bBSPQuadRes; }
    static bool IsDebugRender() { return s_bDebugRender; }

private:

    static bool s_bRender;
    static bool s_bRenderLightmap;
    static bool s_bRenderLightmapOnly;
    static bool s_bBSPQuadRes;
    static bool s_bDebugRender;
    static GLToy_Environment* s_pxCurrentEnvironment;

    static GLToy_HashTree< GLToy_EnvironmentFile* > s_xEnvironments;

};

#endif
