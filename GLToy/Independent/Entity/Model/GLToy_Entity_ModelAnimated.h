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

#ifndef __GLTOY_ENTITY_MODELANIMATED_H_
#define __GLTOY_ENTITY_MODELANIMATED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/Model/GLToy_Entity_ModelStatic.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AnimationStack;
class GLToy_Model;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity_ModelAnimated
: public GLToy_Entity_ModelStatic
{

    typedef GLToy_Entity_ModelStatic GLToy_Parent;

public:

    GLToy_Entity_ModelAnimated( const GLToy_Hash uHash, const u_int uType );
    virtual ~GLToy_Entity_ModelAnimated();

    virtual void Render() const;
	virtual void RenderDeferred() const;
    virtual void Update();

    virtual void SetModel( const GLToy_Hash uHash );
    virtual void SetModel( const GLToy_String& szName );

	void ForceAnimStackCreation();

	void StopAnimation( const u_int uAnimID = 0, const float fTweenOutTime = 0.0f );
    void PlayAnimation( const u_int uAnimID = 0, const float fTweenInTime = 0.0f, const float fTweenOutTime = 0.0f );

protected:

    GLToy_AnimationStack* m_pxAnimStack;

};

#endif
