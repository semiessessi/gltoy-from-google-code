#ifndef __GLTOY_UPDATEABLE_H_
#define __GLTOY_UPDATEABLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Updateable
{

public:

    GLToy_Updateable() {}
    virtual ~GLToy_Updateable() {}

    virtual void Update() = 0;
};

#endif