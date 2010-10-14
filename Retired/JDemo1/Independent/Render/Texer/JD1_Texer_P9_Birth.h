// Thomas Young 25.08.10

#ifndef __JD1_TEXER_BIRTH_H_
#define __JD1_TEXER_BIRTH_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/Texer/JD1_Texer.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class JD1_Texer_P9_Birth : public JD1_Texer
{

    typedef JD1_Texer GLToy_Parent;

public:

    JD1_Texer_P9_Birth();
    virtual ~JD1_Texer_P9_Birth();

    virtual void Initialise();
    virtual void PerFrame();
    virtual void OnBeat();
    virtual void PerPoint( const float i, const float v, float& x, float& y, float& z, float& red, float& green, float& blue, bool& skip, float& sizex, float& sizey ) const;

    // ignore...
    virtual void PerPoint( const float i, const float v, float& x, float& y, float& red, float& green, float& blue, bool& skip, float& linesize ) const {}
};

#endif
