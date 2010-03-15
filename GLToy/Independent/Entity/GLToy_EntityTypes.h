#ifndef __GLTOY_ENTITYTYPES_H_
#define __GLTOY_ENTITYTYPES_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

enum GLToy_EntityType
{
    ENTITY_NULL,
    ENTITY_MODELSTATIC,

    NUM_ENTITY_TYPES
};

GLToy_EntityType GLToy_EntityTypeFromString( const GLToy_String& szString );

#endif