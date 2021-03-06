/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2009, 2010 Semi Essessi
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

#ifndef __GLTOY_MODEL_H_
#define __GLTOY_MODEL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/GLToy_Bounded.h>
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uGLTOY_MODEL_BADINDEX = 0xFFFFFFFF;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_3;
class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AnimationStack
: public GLToy_Updateable
{

public:

    virtual void Evaluate( class GLToy_Model* const pxModel ) const = 0;
    virtual bool SupportsAnimID( GLToy_Model* const pxModel, const u_int uAnimID ) const = 0;
    virtual void Push( const u_int uAnimID, const float fTweenInTime = 0.0f, const float fTweenOutTime = 0.0f, const bool bAnimatedTween = false ) = 0;
    virtual void Stop( const u_int uAnimID, const float fTweenOutTime = 0.0f, const bool bAnimatedTween = false ) = 0;

};

class GLToy_ModelStrip
: public GLToy_Renderable
, public GLToy_SerialisableArray< u_int >
{

    typedef GLToy_SerialisableArray< u_int > GLToy_Parent;

public:

    GLToy_ModelStrip();
    GLToy_ModelStrip( const GLToy_ModelStrip& xStrip );
    GLToy_ModelStrip( const u_int uVertex1, const u_int uVertex2, const u_int uVertex3 );
    GLToy_ModelStrip( const u_int uVertex1, const u_int uVertex2, const u_int uVertex3, const u_int uVertex4 );
    GLToy_ModelStrip( const u_int* puVertices, const u_int uCount );

    virtual ~GLToy_ModelStrip() {}

    virtual GLToy_ModelStrip& operator =( const GLToy_ModelStrip& xStrip );

    void SetVertexPointer( const GLToy_Vector_3* pxVertices ) { m_pxVertices = pxVertices; }

    virtual void Render() const;

protected:

    void SubmitVertices() const;
    virtual void SubmitVertex( const u_int uIndex ) const;
    
    const GLToy_Vector_3* m_pxVertices;

};

class GLToy_Model
: public GLToy_Bounded_AABB
, public GLToy_Renderable
, public GLToy_SerialisableIndirectArray< GLToy_ModelStrip >
{

    typedef GLToy_SerialisableIndirectArray< GLToy_ModelStrip > GLToy_Parent;

public:

    GLToy_Model();
    virtual ~GLToy_Model();

    void Reset();

    virtual void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 );
    virtual void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 );
    virtual void AddStripFromIndices( const u_int* puIndices, const u_int uCount );

    virtual GLToy_AnimationStack* CreateAnimationStack() const { return NULL; }

    virtual void InitialiseFirstFrameData() {}
    virtual void Render() const;
	virtual void RenderDeferred() const;
    virtual void RenderWithPositionAndOrientation( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation ) const;
	virtual void RenderDeferredWithPositionAndOrientation( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation ) const;

    static void LoadFromOBJFile( GLToy_Model* const pxModel, const GLToy_String& szFilename );

    // overrides
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void UpdateStripPointers();

protected:
    
    u_int GetVertexIndex( const GLToy_Vector_3& xVertex );

    GLToy_SerialisableArray< GLToy_Vector_3 > m_xVertices;

};

#endif
