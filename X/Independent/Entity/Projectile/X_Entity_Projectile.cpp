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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/Projectile/X_Entity_Projectile.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// X
#include <Entity/Enemy/X_Entity_Enemy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fSPEED = 3.0f;
static const float fSIZE = 0.01f;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Projectile::X_Entity_Projectile( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
    m_xBoundingSphere.SetRadius( fSIZE );
	m_xDirection[1] = 1.0f;
}

void X_Entity_Projectile::Update()
{
    GLToy_Vector_3 xPosition = GetPosition();
    xPosition += m_xDirection * ( fSPEED * GLToy_Timer::GetFrameTime() );

    SetPosition( xPosition );

    if( xPosition[ 1 ] > 1.5f )
    {
        Destroy();
    }

    static X_Entity_Projectile* ls_pxThis;
    ls_pxThis = this;
    // check for collisions:
    GLToy_QuickFunctor( CollisionFunctor, X_Entity_Enemy*, ppxEnemy,

        if( ppxEnemy && !( *ppxEnemy )->IsDead() && ( *ppxEnemy )->GetBoundingSphere().IntersectsWithSphere( ls_pxThis->m_xBoundingSphere ) )
        {
            ( *ppxEnemy )->Hurt( 0, 10.0f );
			ls_pxThis->Destroy();
        }
    );

    X_Entity_Enemy::GetList().Traverse( CollisionFunctor() );

    GLToy_Parent::Update();
}

void X_Entity_Projectile::Render() const
{
    const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_3( 0.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] + fSIZE, xPosition[ 2 ] );  
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] + fSIZE, xPosition[ 2 ] );  
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] ); 
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] );

    GLToy_Render::EndSubmit();
}
