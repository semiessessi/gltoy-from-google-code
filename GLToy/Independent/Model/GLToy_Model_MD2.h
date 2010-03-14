#ifndef __GLTOY_MODEL_PLACEHOLDER_H_
#define __GLTOY_MODEL_PLACEHOLDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Model/GLToy_Model.h>

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
        Stand               = 0,
        Run                 = 1,
        Attack              = 2,
        Pain_1              = 3,
        Pain_2              = 4,
        Pain_3              = 5,
        Jump                = 6,
        Flip                = 7,
        Salute              = 8,
        Fallback            = 9,
        Wave                = 10,
        Point               = 11,
        Crouch_Stand        = 12,
        Crouch_Walk         = 13,
        Crouch_Attack       = 14,
        Crouch_Pain         = 15,
        Crouch_Death        = 16,
        Death_FallBack      = 17,
        Death_FallForward   = 18,
        Death_FallBackSlow  = 19,
        Boom                = 20,

        NumAnimIDs          = 21

    };

    GLToy_Model_MD2();

    virtual void Render() const;

protected:

    virtual void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 ) {}
    virtual void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 ) {}
    virtual void AddStripFromIndices( const u_int* puIndices, const u_int uCount ) {}

    GLToy_Array< u_char > m_xNormalIndices;
    GLToy_Array< int > m_xGLCommands;

    GLToy_Array< GLToy_Vector_3 > m_xTexCoords;
    GLToy_Array< GLToy_MD2_Triangle > m_xTriangles;

};

#endif