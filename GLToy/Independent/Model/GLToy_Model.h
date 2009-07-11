#ifndef __GLTOY_MODEL_H_
#define __GLTOY_MODEL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// GLToy
#include <Render/GLToy_Renderable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_3;

class GLToy_Model_Strip
{

public:

    u_int m_uVertexCount;
    u_int* m_puVertexIndices;

    GLToy_Model_Strip() : m_uVertexCount( 0 ), m_puVertexIndices( NULL ) {}
    virtual ~GLToy_Model_Strip();

protected:

};

class GLToy_Model
{

public:
    GLToy_Model() 
    : m_uVertexCount( 0 )
    , m_pxVertices( NULL )
    {}

protected:
    
    u_int m_uVertexCount;
    GLToy_Vector_3* m_pxVertices;

};

#endif