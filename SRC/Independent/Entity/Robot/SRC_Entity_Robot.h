/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SRC_ENTITY_PROJECTILE_H_
#define __SRC_ENTITY_PROJECTILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Entity/Object/SRC_Entity_Object.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class SRC_Entity_Robot
: public SRC_Entity_Object
{

    typedef SRC_Entity_Object GLToy_Parent;

public:

    SRC_Entity_Robot( const GLToy_Hash uHash, const u_int uType );
    virtual ~SRC_Entity_Robot();

    void Spawn( const GLToy_Vector_3& xPosition );

    virtual void Render() const;
    virtual void Update();

    static const GLToy_Array< SRC_Entity_Robot* >& GetRobotList() { return s_xInstanceList; }

    static void SpawnRobot_Console();

protected:

    GLToy_Vector_2 m_xTargetPosition;
    bool m_bMoving;
    bool m_bOldMouseData;

private:

    static GLToy_Array< SRC_Entity_Robot* > s_xInstanceList;

};

#endif
