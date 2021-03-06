/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/X.h>

// This file's header
#include <Entity/X_EntityTypes.h>

// GLToy
#include <Entity/GLToy_Entity.h>
#include <Entity/GLToy_Entity_System.h>

// X
#include <Entity/Collectible/X_Entity_Collectible.h>
#include <Entity/Enemy/X_Entity_Enemy.h>
#include <Entity/Player/X_Entity_Player.h>
#include <Entity/Projectile/X_Entity_Projectile.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Entity* X_CreateEntity( const GLToy_Hash uHash, const u_int uType )
{
    switch( uType )
    {
        case X_ENTITY_PLAYER:				return new X_Entity_Player( uHash, uType );
        case X_ENTITY_ENEMY:				return new X_Entity_Enemy( uHash, uType );
        case X_ENTITY_PROJECTILE:			return new X_Entity_Projectile( uHash, uType );
		case X_ENTITY_COLLECTIBLE:			return new X_Entity_Collectible( uHash, uType );

        default:
        {
            break;
        }
    }

    return NULL;
}

