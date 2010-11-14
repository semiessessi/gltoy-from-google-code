/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SRC_ENTITY_MOVEABLE_PLANK_H_
#define __SRC_ENTITY_MOVEABLE_PLANK_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/Object/Moveable/SRC_Entity_Moveable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class SRC_Entity_Moveable_Plank
: public SRC_Entity_Moveable
{

    typedef SRC_Entity_Moveable GLToy_Parent;

public:

    SRC_Entity_Moveable_Plank( const GLToy_Hash uHash, const u_int uType );
    virtual ~SRC_Entity_Moveable_Plank() {}

    void Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity );

    virtual void Render() const;

};

#endif
