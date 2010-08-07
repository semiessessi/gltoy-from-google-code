/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

#ifndef __PLATFORM_GLTOY_MATHS_H_
#define __PLATFORM_GLTOY_MATHS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Platform_GLToy_Maths
{

    friend class GLToy_Maths;
    friend class Platform_GLToy_Vector;

public:

    static bool Initialise();
    static void Update();

    static float Sqrt( const float fValue );

private:

    static union CPUFeatures
    {
        struct
        {
            unsigned int m_uEax;
            unsigned int m_uEbx;
            unsigned int m_uEcx;
            unsigned int m_uEdx;
        };

        struct
        {
            unsigned int m_uSteppingId               : 4;
            unsigned int m_uModelID                  : 4;
            unsigned int m_uFamilyID                 : 4;
            unsigned int m_uP4Family                 : 2;
            unsigned int m_uReserved1                : 2;
            unsigned int m_uExtendedModelID          : 4;
            unsigned int m_uExtendedFamilyID         : 8;
            unsigned int m_uReserved2                : 4;

            unsigned int m_uBrandIndex               : 8;
            unsigned int m_uCFLUSHLineSize           : 8;
            unsigned int m_uMaximumAddressableIDs    : 8;
            unsigned int m_uInitialAPICID            : 8;

            bool m_bSSE3                                : 1;
            bool m_bReserved3                           : 1;
            bool m_bDTES64                              : 1;
            bool m_bMONITOR                             : 1;
            bool m_bDSCPL                               : 1;
            bool m_bVMX                                 : 1;
            bool m_bSMX                                 : 1;
            bool m_bEST                                 : 1;
            bool m_bTM2                                 : 1;
            bool m_bSSSE3                               : 1;
            bool m_bCNXTID                              : 1;
            bool m_bReserved4                           : 1;
            bool m_bReserved5                           : 1;
            bool m_bCMPXCHG16B                          : 1;
            bool m_bXTPRUpdateControl                   : 1;
            bool m_bPDCM                                : 1;
            bool m_bReserved6                           : 1;
            bool m_bReserved7                           : 1;
            bool m_bDCA                                 : 1;
            bool m_bSSE41                               : 1;
            bool m_bSSE42                               : 1;
            bool m_bX2APIC                              : 1;
            bool m_bMOVBE                               : 1;
            bool m_bPOPCNT                              : 1;
            bool m_bReserved8                           : 1;
            bool m_bReserved9                           : 1;
            bool m_bXSAVE                               : 1;
            bool m_bOSXSAVE                             : 1;
            bool m_bReserved10                          : 1;
            bool m_bReserved11                          : 1;
            bool m_bReserved12                          : 1;
            bool m_bReserved13                          : 1;

            bool m_bFPU                                 : 1;
            bool m_bVME                                 : 1;
            bool m_bDE                                  : 1;
            bool m_bPSE                                 : 1;
            bool m_bTSC                                 : 1;
            bool m_bMSR                                 : 1;
            bool m_bPAE                                 : 1;
            bool m_bMCE                                 : 1;
            bool m_bCX8                                 : 1;
            bool m_bAPIC                                : 1;
            bool m_bReserved14                          : 1;
            bool m_bSEP                                 : 1;
            bool m_bMTRR                                : 1;
            bool m_bPGE                                 : 1;
            bool m_bMCA                                 : 1;
            bool m_bCMOV                                : 1;
            bool m_bPAT                                 : 1;
            bool m_bPSE36                               : 1;
            bool m_bPSN                                 : 1;
            bool m_bCFLSH                               : 1;
            bool m_bReserved15                          : 1;
            bool m_bDS                                  : 1;
            bool m_bACPI                                : 1;
            bool m_bMMX                                 : 1;
            bool m_bFXSR                                : 1;
            bool m_bSSE                                 : 1;
            bool m_bSSE2                                : 1;
            bool m_bSS                                  : 1;
            bool m_bHTT                                 : 1;
            bool m_bRM                                  : 1;
            bool m_bReserved16                          : 1;
            bool m_bPBE                                 : 1;
        };
    } s_xCPUFeatures;
};

#endif
