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

#ifndef __GLTOY_MODEL_MD2_H_
#define __GLTOY_MODEL_MD2_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Stack.h>
#include <Core/GLToy_Updateable.h>
#include <Model/GLToy_Model.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - this could be derived from model strip, but that seems a little silly...
// need to rethink strip vs. general component (patch etc.) possibly.
struct GLToy_MD2_Triangle
{

    u_short m_ausVertices[ 3 ];
    u_short m_ausUVs[ 3 ];

};

struct GLToy_MD2_Animation
{

    u_int   m_uFirstFrame;
    u_int   m_uLastFrame;
    u_int   m_uFPS;

};

class GLToy_Model_MD2
: public GLToy_Model
{
    friend class GLToy_MD2_AnimationStack;
    friend class GLToy_MD2_AnimationState;
    friend class GLToy_MD2File;

    typedef GLToy_Model GLToy_Parent;

public:

    enum AnimID
    {
        ANIMID_STAND,
        ANIMID_RUN,
        ANIMID_ATTACK,
        ANIMID_PAIN_A,
        ANIMID_PAIN_B,
        ANIMID_PAIN_C,
        ANIMID_JUMP,
        ANIMID_FLIP,
        ANIMID_SALUTE,
        ANIMID_FALLBACK,
        ANIMID_WAVE,
        ANIMID_POINT,
        ANIMID_CROUCH_STAND,
        ANIMID_CROUCH_WALK,
        ANIMID_CROUCH_ATTACK,
        ANIMID_CROUCH_PAIN,
        ANIMID_CROUCH_DEATH, 
        ANIMID_DEATH_FALLBACK,
        ANIMID_DEATH_FALLFORWARD,
        ANIMID_DEATH_FALLBACKSLOW,
        ANIMID_BOOM,

        NUM_ANIMIDS

    };

    GLToy_Model_MD2();

    virtual GLToy_AnimationStack* CreateAnimationStack() const;

    virtual void InitialiseFirstFrameData();
    virtual void Render() const;

protected:

    virtual void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 ) {}
    virtual void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 ) {}
    virtual void AddStripFromIndices( const u_int* puIndices, const u_int uCount ) {}

    void SetFrameSize( const u_int uFrameSize );
    u_int GetFrameCount() const { return m_xVertices.GetCount() / m_uFrameSize; }
    void SetTexture( GLToy_Texture* const pxTexture ) { m_pxTexture = pxTexture; }

    GLToy_Array< u_char > m_xNormalIndices;
    GLToy_Array< int > m_xGLCommands;

    GLToy_Array< GLToy_Vector_3 > m_xUVs;
    GLToy_Array< GLToy_MD2_Triangle > m_xTriangles;

    GLToy_Array< GLToy_Vector_3 > m_xWorkingVertices;
    GLToy_Array< GLToy_Vector_3 > m_xWorkingNormals;

    u_int m_uFrameSize;
    GLToy_Texture* m_pxTexture;

};

class GLToy_MD2_AnimationState
: public GLToy_Updateable
{

    friend class GLToy_MD2_AnimationStack;

public:

    GLToy_MD2_AnimationState();
    GLToy_MD2_AnimationState( const GLToy_Model_MD2::AnimID eAnimID, const float fTweenInTime = 0.0f, const float fTweenOutTime = 0.0f, const bool bAnimatedTween = false );

    virtual ~GLToy_MD2_AnimationState() {}

    virtual void Update();

    GLToy_Inline bool IsDone() const { return m_fTimer >= m_fEndTime; }

    void Evaluate( GLToy_Model_MD2* const pxModel ) const;

protected:

    GLToy_Model_MD2::AnimID m_eAnimID;
    float m_fTimer;
    float m_fTweenInTime;
    float m_fTweenOutTime;
    float m_fEndTime;
    bool m_bLoop;
    bool m_bAnimatedTween;

};

class GLToy_MD2_AnimationStack
: protected GLToy_Array< GLToy_MD2_AnimationState >
, public GLToy_AnimationStack
{

    friend class GLToy_ConstIterator< GLToy_MD2_AnimationState >;

    typedef GLToy_Array< GLToy_MD2_AnimationState > GLToy_Parent;

public:

    GLToy_MD2_AnimationStack()
    : GLToy_Parent()
    {
    }

    virtual void Update();

    virtual void Evaluate( GLToy_Model* const pxModel ) const;
    virtual bool SupportsAnimID( GLToy_Model* const pxModel, const u_int uAnimID ) const;
    virtual void Push( const u_int uAnimID, const float fTweenInTime = 0.0f, const float fTweenOutTime = 0.0f, const bool bAnimatedTween = false );
    virtual void Stop( const u_int uAnimID, const float fTweenOutTime = 0.0f, const bool bAnimatedTween = false );

protected:

};

#endif
