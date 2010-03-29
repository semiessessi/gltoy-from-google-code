/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Maths/GLToy_Quaternion.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Matrix_3 GLToy_Quaternion::GetOrientationMatrix()
{
    // this craps on the conventions but I think its more readable - sorry
    const float& w = m_fComponents[ 0 ];
    const float& x = m_fComponents[ 1 ];
    const float& y = m_fComponents[ 2 ];
    const float& z = m_fComponents[ 3 ];

    return GLToy_Matrix_3(

        w*w + x*x - y*y - z*z,      2*x*y - 2*z*w,              2*x*z + 2*w*y,
        2*x*y + 2*w*z,              w*w - x*x + y*y - z*z,      2*y*z - 2*w*x,
        2*x*z - 2*w*y,              2*y*z + 2*w*x,              w*w - x*x - y*y + z*z
        
        );

}