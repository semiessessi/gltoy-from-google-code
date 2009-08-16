#ifndef __GLTOY_MODEL_H_
#define __GLTOY_MODEL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/GLToy_Renderable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uGLTOY_MODEL_BADINDEX = 0xFFFFFFFF;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_3;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ModelStrip : public GLToy_Renderable
{

    typedef GLToy_Renderable PARENT;

public:

    GLToy_ModelStrip();
    GLToy_ModelStrip( const GLToy_ModelStrip& xStrip );
    GLToy_ModelStrip( u_int uVertex1, u_int uVertex2, u_int uVertex3 );
    GLToy_ModelStrip( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4 );
    virtual ~GLToy_ModelStrip();

    virtual GLToy_ModelStrip& operator =( const GLToy_ModelStrip& xStrip );

    void SetVertexPointer( const GLToy_Vector_3* pxVertices ) { m_pxVertices = pxVertices; }

    virtual void Render();

protected:
    
    u_int m_uVertexCount;
    u_int* m_puVertexIndices;
    const GLToy_Vector_3* m_pxVertices;

};

class GLToy_Model : public GLToy_Renderable
{

    typedef GLToy_Renderable PARENT;

public:

    GLToy_Model();
    virtual ~GLToy_Model();

    void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 );
    void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 );

    virtual void Render();

protected:
    
    void UpdateStripPointers();
    
    u_int GetVertexIndex( const GLToy_Vector_3& xVertex );

    u_int m_uVertexCount;
    GLToy_Vector_3* m_pxVertices;

    u_int m_uStripCount;
    GLToy_ModelStrip** m_pxStrips;

};

#endif