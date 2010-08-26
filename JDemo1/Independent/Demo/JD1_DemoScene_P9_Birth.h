// Thomas Young 25.08.10

#ifndef __JD1_DEMOSCENE_P9_BIRTH_H_
#define __JD1_DEMOSCENE_P9_BIRTH_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Demo/JD1_DemoScene_AVS.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_Pair.h>
#include <Render/Trans/JD1_Trans_Fadeout.h>
#include <Render/Texer/JD1_Texer_P9_Birth.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef GLToy_Pair< GLToy_Renderable*, GLToy_Updateable* > JD1_AVS_Component;

class JD1_DemoScene_P9_Birth : public JD1_DemoScene_AVS
{
    
    typedef JD1_DemoScene_AVS GLToy_Parent;

public:

    JD1_DemoScene_P9_Birth() : GLToy_Parent()
    , m_xBirth()
    , m_xFadeout( 1.0f )
    {
    }

    virtual ~JD1_DemoScene_P9_Birth()
    {
    }

    virtual void Initialise()
    {
        AppendComponent( static_cast< GLToy_Renderable* >( &m_xFadeout ), static_cast< GLToy_Updateable* >( &m_xFadeout ) );
        AppendComponent( static_cast< GLToy_Renderable* >( &m_xBirth ), static_cast< GLToy_Updateable* >( &m_xBirth ) );

        GLToy_Parent::Initialise();
    }

protected:

    JD1_Texer_P9_Birth m_xBirth;
    JD1_Trans_Fadeout m_xFadeout;

};

#endif
