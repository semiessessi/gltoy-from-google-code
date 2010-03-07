#ifndef __GLTOY_MODEL_H_
#define __GLTOY_MODEL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Array.h>
#include <Render/GLToy_Renderable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uGLTOY_MODEL_BADINDEX = 0xFFFFFFFF;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_3;
class GLToy_Vector_3;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ModelStrip
: public GLToy_Array< u_int >
, public GLToy_Renderable
{

    typedef GLToy_Array< u_int > PARENT_DATA;
    typedef GLToy_Renderable PARENT_RENDER;

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
: public GLToy_IndirectArray< GLToy_ModelStrip >
, public GLToy_Renderable
{

    typedef GLToy_IndirectArray< GLToy_ModelStrip > PARENT_DATA;
    typedef GLToy_Renderable PARENT_RENDER;

public:

    GLToy_Model();
    virtual ~GLToy_Model();

    void Reset();

    virtual void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 );
    virtual void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 );
    virtual void AddStripFromIndices( const u_int* puIndices, const u_int uCount );

    virtual void Render() const;
    virtual void RenderWithPositionAndOrientation( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation ) const;

    static void LoadFromOBJFile( GLToy_Model* const pxModel ,const char* const szFilename );

protected:
    
    void UpdateStripPointers();
    
    u_int GetVertexIndex( const GLToy_Vector_3& xVertex );

    u_int m_uVertexCount;
    GLToy_Vector_3* m_pxVertices;

};

#endif