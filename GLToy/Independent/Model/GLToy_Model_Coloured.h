#ifndef __GLTOY_MODEL_COLOURED_H_
#define __GLTOY_MODEL_COLOURED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Model/GLToy_Model.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ModelStrip_Coloured : public GLToy_ModelStrip
{

    typedef GLToy_ModelStrip PARENT;

public:

    GLToy_ModelStrip_Coloured();
    GLToy_ModelStrip_Coloured( const GLToy_ModelStrip_Coloured& xStrip );
    GLToy_ModelStrip_Coloured( u_int uVertex1, u_int uVertex2, u_int uVertex3, const GLToy_Vector_3& xColour );
    GLToy_ModelStrip_Coloured( u_int uVertex1, u_int uVertex2, u_int uVertex3, u_int uVertex4, const GLToy_Vector_3& xColour );
    virtual ~GLToy_ModelStrip_Coloured();

    virtual GLToy_ModelStrip_Coloured& operator =( const GLToy_ModelStrip_Coloured& xStrip );

    virtual void Render();

protected:
    
    GLToy_Vector_3 m_xColour;

};

class GLToy_Model_Coloured : public GLToy_Model
{

    typedef GLToy_Model PARENT;

public:

    GLToy_Model_Coloured();
    virtual ~GLToy_Model_Coloured();

    void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xColour );
    void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4, const GLToy_Vector_3& xColour );

};

#endif