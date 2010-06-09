/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

#ifndef __GLTOY_PFX_SYSTEM_H_
#define __GLTOY_PFX_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T > class GLToy_HashTree;
class GLToy_PFX;
class GLToy_PFXProperties;
struct GLToy_ParticleProperties;
struct GLToy_ParticleSourceProperties;
class GLToy_Vector_3;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_PFX_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();
    static void Update();

    static GLToy_PFX* CreatePFX( const GLToy_Hash uHash, const GLToy_Vector_3& xPosition );
    static void DestroyPFX( const GLToy_Hash uPFXHash );
    static const GLToy_ParticleProperties* GetParticleProperties( const GLToy_Hash uHash );

private:

    static bool InitialisePFXProperties();
    static bool InitialiseSourceProperties();
    static bool InitialiseParticleProperties();

    static GLToy_HashTree< GLToy_PFX* > s_xPFX;
    static GLToy_HashTree< GLToy_PFXProperties > s_xPFXProperties;
    static GLToy_HashTree< GLToy_ParticleSourceProperties > s_xSourceProperties;
    static GLToy_HashTree< GLToy_ParticleProperties > s_xParticleProperties;

};

#endif
