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
#include <Entity/Goal/SRC_Entity_Goal.h>

// SRC
#include <Entity/Robot/SRC_Entity_Robot.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO: decide on scale etc. for goal so that this works well - also if squares are good enough
// otherwise this can be split out into a base (Goal) and derived (Goal_Square) so we can add more

SRC_Entity_Goal::SRC_Entity_Goal( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_xCentre( GLToy_Maths::ZeroVector2 )
, m_xExtents( 1.0f, 1.0f )
{
}

void SRC_Entity_Goal::SetArea( const GLToy_Vector_2& xCentre, const GLToy_Vector_2& xSize )
{
    m_xCentre = xCentre;
    m_xExtents = xSize * 0.5f;
}

void SRC_Entity_Goal::Update()
{
    // check for robots inside the goal...
    const GLToy_Array< SRC_Entity_Robot* >* const pxList = &( SRC_Entity_Robot::GetRobotList() );
    GLToy_ConstIterate( SRC_Entity_Robot*, xIterator, pxList )
    {
        SRC_Entity_Robot* const pxRobot = xIterator.Current();
        if( !pxRobot )
        {
            continue;
        }

        const GLToy_Vector_2 xDistances = m_xCentre - GLToy_Vector_2( pxRobot->GetPosition()[ 0 ], pxRobot->GetPosition()[ 2 ] );
        if( ( GLToy_Maths::Abs( xDistances[ 0 ] ) < m_xExtents[ 0 ] )
            && ( GLToy_Maths::Abs( xDistances[ 1 ] ) < m_xExtents[ 1 ] ) )
        {
            // TODO: goal reached logic
        }
    }
}