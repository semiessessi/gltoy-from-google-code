/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_ENTITYTYPES_H_
#define __GLTOY_ENTITYTYPES_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO: change GLToy_EntityType to use u_short for storage
// in hindsight 255 entity types is too little if I want to support all the BSPs
enum GLToy_EntityType
{
    ENTITY_NULL,
    
    ENTITY_MODELSTATIC,
    ENTITY_MODELANIMATED,
    
    ENTITY_SPRITE,

    ENTITY_PHYSICSBOX,

    ENTITY_BSP_FUNC_BUTTON,
    ENTITY_BSP_INFO_PLAYER_START,
    ENTITY_BSP_TRIGGER_COUNTER,

    ENTITY_QUAKE2_TARGET_CHANGELEVEL,

    NUM_ENTITY_TYPES
};

GLToy_EntityType GLToy_EntityTypeFromString( const GLToy_String& szString );

#endif
