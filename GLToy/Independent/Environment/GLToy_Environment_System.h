/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
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