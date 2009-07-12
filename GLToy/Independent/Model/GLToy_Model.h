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

class GLToy_Model_Strip : public GLToy_Renderable
{

public:

	GLToy_Model_Strip();
	GLToy_Model_Strip( const GLToy_Model_Strip& xStrip );
    GLToy_Model_Strip( u_int uVertex1, u_int uVertex2, u_int uVertex3 );
	GLToy_Model_Strip( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4 );
    virtual ~GLToy_Model_Strip();

	virtual GLToy_Model_Strip& operator =( const GLToy_Model_Strip& xStrip );

	void SetVertexPointer( const GLToy_Vector_3* pxVertices ) { m_pxVertices = pxVertices; }

	virtual void Render();

protected:
	
	u_int m_uVertexCount;
    u_int* m_puVertexIndices;
	const GLToy_Vector_3* m_pxVertices;

};

class GLToy_Model : public GLToy_Renderable
{

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
	GLToy_Model_Strip* m_pxStrips;

};

#endif