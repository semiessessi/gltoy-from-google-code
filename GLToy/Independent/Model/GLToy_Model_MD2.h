#ifndef __GLTOY_MODEL_PLACEHOLDER_H_
#define __GLTOY_MODEL_PLACEHOLDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Model/GLToy_Model.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - this could be derived from model strip, but that seems a little silly...
// need to rething strip vs. general component (patch etc.) possibly.
struct GLToy_MD2_Triangle
{

    u_short m_ausVertices[ 3 ];
    u_short m_ausTexCoords[ 3 ];

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

    virtual void Render() const;

protected:

    virtual void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 ) {}
    virtual void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 ) {}
    virtual void AddStripFromIndices( const u_int* puIndices, const u_int uCount ) {}

    void SetTexture( GLToy_Texture* const pxTexture ) { m_pxTexture = pxTexture; }

    GLToy_Array< u_char > m_xNormalIndices;
    GLToy_Array< int > m_xGLCommands;

    GLToy_Array< GLToy_Vector_3 > m_xTexCoords;
    GLToy_Array< GLToy_MD2_Triangle > m_xTriangles;

    GLToy_Texture* m_pxTexture;

};

#endif