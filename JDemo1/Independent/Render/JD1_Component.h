
// Thomas Young - 26.08.10

#ifndef JD1_COMPONENT_H_
#define JD1_COMPONENT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// CRT
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class JD1_Component
{
    friend class JD1_Component_Helper;

    protected:

        JD1_Component()
        : m_afMegabuffer( 0 )
        {
        }

        virtual ~JD1_Component()
        {
            delete []m_afMegabuffer;
        }

        float& megabuf( int iIndex ) const
        {
            if( !m_afMegabuffer )
            {
                m_afMegabuffer = new float[1024 * 1024];
                memset( m_afMegabuffer, 0, sizeof( float ) * 1024 * 1024 );
            }
            return m_afMegabuffer[iIndex];
        }

        static float& gmegabuf( int iIndex )
        {
            if( !s_afGMegabuffer )
            {
                s_afGMegabuffer = new float[1024 * 1024];
                memset( s_afGMegabuffer, 0, sizeof( float ) * 1024 * 1024 );
            }
            return s_afGMegabuffer[iIndex];
        }

        static float& reg( int iReg )
        {
            if( !s_afRegisters )
            {
                s_afRegisters = new float[100];
                memset( s_afRegisters, 0, sizeof(float) * 100 );
            }
            return s_afRegisters[0];
        }


        static float* s_afGMegabuffer;
        static float* s_afRegisters;
        
        mutable float* m_afMegabuffer;
    
};

class JD1_Component_Helper
{
    public:
        
        ~JD1_Component_Helper()
        {
            delete []JD1_Component::s_afGMegabuffer;
            delete []JD1_Component::s_afRegisters;
        }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S
/////////////////////////////////////////////////////////////////////////////////////////////

#define reg00 reg(0)
#define reg01 reg(1)
#define reg02 reg(2)
#define reg03 reg(3)
#define reg04 reg(4)
#define reg05 reg(5)
#define reg06 reg(6)
#define reg07 reg(7)
#define reg08 reg(8)
#define reg09 reg(9)
#define reg10 reg(10)
#define reg11 reg(11)
#define reg12 reg(12)
#define reg13 reg(13)
#define reg14 reg(14)
#define reg15 reg(15)
#define reg16 reg(16)
#define reg17 reg(17)
#define reg18 reg(18)
#define reg19 reg(19)
#define reg20 reg(20)
#define reg21 reg(21)
#define reg22 reg(22)
#define reg23 reg(23)
#define reg24 reg(24)
#define reg25 reg(25)
#define reg26 reg(26)
#define reg27 reg(27)
#define reg28 reg(28)
#define reg29 reg(29)
#define reg30 reg(30)
#define reg31 reg(31)
#define reg32 reg(32)
#define reg33 reg(33)
#define reg34 reg(34)
#define reg35 reg(35)
#define reg36 reg(36)
#define reg37 reg(37)
#define reg38 reg(38)
#define reg39 reg(39)
#define reg40 reg(40)
#define reg41 reg(41)
#define reg42 reg(42)
#define reg43 reg(43)
#define reg44 reg(44)
#define reg45 reg(45)
#define reg46 reg(46)
#define reg47 reg(47)
#define reg48 reg(48)
#define reg49 reg(49)
#define reg50 reg(50)
#define reg51 reg(51)
#define reg52 reg(52)
#define reg53 reg(53)
#define reg54 reg(54)
#define reg55 reg(55)
#define reg56 reg(56)
#define reg57 reg(57)
#define reg58 reg(58)
#define reg59 reg(59)
#define reg60 reg(60)
#define reg61 reg(61)
#define reg62 reg(62)
#define reg63 reg(63)
#define reg64 reg(64)
#define reg65 reg(65)
#define reg66 reg(66)
#define reg67 reg(67)
#define reg68 reg(68)
#define reg69 reg(69)
#define reg70 reg(70)
#define reg71 reg(71)
#define reg72 reg(72)
#define reg73 reg(73)
#define reg74 reg(74)
#define reg75 reg(75)
#define reg76 reg(76)
#define reg77 reg(77)
#define reg78 reg(78)
#define reg79 reg(79)
#define reg80 reg(80)
#define reg81 reg(81)
#define reg82 reg(82)
#define reg83 reg(83)
#define reg84 reg(84)
#define reg85 reg(85)
#define reg86 reg(86)
#define reg87 reg(87)
#define reg88 reg(88)
#define reg89 reg(89)
#define reg90 reg(90)
#define reg91 reg(91)
#define reg92 reg(92)
#define reg93 reg(93)
#define reg94 reg(94)
#define reg95 reg(95)
#define reg96 reg(96)
#define reg97 reg(97)
#define reg98 reg(98)
#define reg99 reg(99)

#endif // JD1_COMPONENT_H_

// eof
