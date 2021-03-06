/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2011 Semi Essessi
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

#ifndef __GLTOY_RENDER_METRICS_H_
#define __GLTOY_RENDER_METRICS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Render_Metrics
{

public:

    static const u_int uGLTOY_IDEAL_TRIANGLE_LIMIT = 50000;
    static const u_int uGLTOY_IDEAL_LIGHT_LIMIT = 50;
    static const u_int uGLTOY_IDEAL_MATERIAL_SWITCH_LIMIT = 200;

    static void IncrementTriangleCount( const u_int uTriangles = 1 ) { s_uTriangleCount += uTriangles; }
    static void IncrementLightCount( const u_int uLights = 1 ) { s_uLightCount += uLights; }
    static void IncrementMaterialSwitchCount() { ++s_uMaterialSwitchCount; }

    static void ResetCounters()
    {
        s_fTriangleTime += GLToy_Timer::GetFrameTime();
        s_uTrianglePeak = GLToy_Maths::Max( s_uTriangleCount, s_uTrianglePeak );
        const bool bDone = ( s_fTriangleTime > 5.0f );
        if( ( s_uTriangleCount == s_uTrianglePeak ) || bDone )
        {
            if( bDone )
            {
                s_uTrianglePeak = s_uTriangleCount;
            }
            s_fTriangleTime = 0.0f;
        }

        s_uTriangleCount = s_uLightCount = s_uMaterialSwitchCount = 0;
    }

    static u_int GetTriangleCount() { return s_uTriangleCount; }
    static u_int GetTrianglePeak() { return s_uTrianglePeak; }
    static u_int GetLightCount() { return s_uLightCount; }

private:

    static u_int s_uTriangleCount;
    static u_int s_uLightCount;
    static u_int s_uMaterialSwitchCount;

    static u_int s_uTrianglePeak;
    static float s_fTriangleTime;

};

#endif
