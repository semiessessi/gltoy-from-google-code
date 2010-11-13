/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
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

    ENTITY_FIRST_PROJECT_TYPE,

    NUM_ENTITY_TYPES
};

GLToy_EntityType GLToy_EntityTypeFromString( const GLToy_String& szString );

#endif
