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

/////////////////////////////////////////////////////////////////////////////////////////////
//  V I S I B I L I T Y   S Y S T E M
/////////////////////////////////////////////////////////////////////////////////////////////
//
//  This PVS currently comes from the Quake 2 toolset... it can come from anywhere though
//  It is however heavily inspired by Quakes, but dirtied with the friendlyness of GLToy
//
//  This does however mean that the BSP tree lives in the environment and is loaded there
//  the PVS accesses it through the environment system, but this allows me to add whatever
//  else I like to the PVS... lights, entities, sounds etc.
//
//  The general idea is that this should grow into a central system, driving lots of things
//  eventually all but the 2d stuff should be rendered through the visibility system, lights,
//  sounds and entities can be updated based on visibility - etc.
//
//  However, it also needs to handle the null case effectively...
// 
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_VISIBILITY_SYSTEM_H_
#define __GLTOY_VISIBILITY_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_BSPTree.h>
#include <Core/Data Structures/GLToy_List.h>
#include <Maths/GLToy_Ray.h>
#include <Maths/GLToy_Vector.h>
#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity;
class GLToy_Light;

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Visibility_System_VectorPairOverload( xReturnType, xFunction ) static GLToy_Inline xReturnType xFunction( const GLToy_Vector_3& xPosition1, const GLToy_Vector_3& xPosition2 )\
    {\
        const float fLimitingDistance = ( xPosition2 - xPosition1 ).Magnitude();\
        const GLToy_Ray xRay = GLToy_Ray::FromPositions( xPosition1, xPosition2 );\
        return xFunction( xRay, fLimitingDistance );\
    }

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Trace_Result
{

public:

    bool m_bHit;
    float m_fParameter;
    // TODO: although the raytrace functions at the bottom of this stuff can always get these they aren't propogated upwards yet
    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xNormal;
    GLToy_Hash m_uHitEntity;

};

class GLToy_Visibility_System
{

public:

    static void ClearPVS() { s_xPVS.Clear(); }
    static void RegisterCluster( const GLToy_Array< u_int >& xPVS, const GLToy_Array< u_int >& xLeaves );

    static void ClearPVSEntities();
    static void ClearPVSLights();

    static int GetCluster( const GLToy_Vector_3& xPosition );

    static bool LineOfSightTest( const GLToy_Ray& xRay, const float fLimitingDistance = GLToy_Maths::LargeFloat );
    static float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = GLToy_Maths::LargeFloat );
    static GLToy_Hash TraceEntity( const GLToy_Ray& xRay, const float fLimitingDistance = GLToy_Maths::LargeFloat );

    static GLToy_Trace_Result FullTrace( const GLToy_Ray& xRay, const float fLimitingDistance = GLToy_Maths::LargeFloat );

    GLToy_Visibility_System_VectorPairOverload( bool, LineOfSightTest )
    GLToy_Visibility_System_VectorPairOverload( float, Trace )
    GLToy_Visibility_System_VectorPairOverload( GLToy_Hash, TraceEntity )

    GLToy_Visibility_System_VectorPairOverload( GLToy_Trace_Result, FullTrace )

    static void RenderForward();
    static void RenderDeferred();
    static void RenderTransparent();

    static void Update();

private:
    
    ////////////////
    // PVS cluster
    ////////////////
    // NOTE: const array pointers are used for the static things, lists for the dynamics
    class PVS_Cluster
    {

        friend class GLToy_Visibility_System;

    public:
    
        PVS_Cluster()
        : m_pxPVS( NULL )
        //, m_pxPHS()
        , m_pxLeaves( NULL )
        , m_xEntities()
        , m_xLights()
        //, m_xSounds()
        , m_xBoundingSphere()
        {
        }

        PVS_Cluster( const GLToy_Array< u_int >& xPVS, const GLToy_Array< u_int >& xLeaves )
        : m_pxPVS( &xPVS )
        //, m_pxPHS()
        , m_pxLeaves( &xLeaves )
        , m_xEntities()
        , m_xLights()
        //, m_xSounds()
        , m_xBoundingSphere()
        {
        }

        PVS_Cluster operator =( const PVS_Cluster& xCluster )
        {
            m_pxPVS = xCluster.m_pxPVS;
            m_pxLeaves = xCluster.m_pxLeaves;
            m_xEntities = xCluster.m_xEntities;
            m_xLights = xCluster.m_xLights;
            m_xBoundingSphere = xCluster.m_xBoundingSphere;

            return *this;
        }

        const GLToy_Array< u_int >& GetLeaves() const { return *m_pxLeaves; }

    private:
                                                    // each PVS cluster has...
        const GLToy_Array< u_int >* m_pxPVS;        // a list of other clusters it can see
        //GLToy_Array< u_int > m_xPHS;                // a list of other clusters it can hear
        const GLToy_Array< u_int >* m_pxLeaves;     // a list of environment leaves
        GLToy_List< GLToy_Entity* > m_xEntities;    // a list of entites
        GLToy_List< GLToy_Light* > m_xLights;       // a list of lights
        //GLToy_List< GLToy_Sound?* > m_xSounds;      // a list of sounds

        GLToy_Sphere m_xBoundingSphere;             // a bounding sphere

    };

    static GLToy_Array< PVS_Cluster > s_xPVS;
};

#endif
