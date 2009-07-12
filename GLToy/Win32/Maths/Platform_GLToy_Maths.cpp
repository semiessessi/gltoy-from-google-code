/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Maths/GLToy_Maths.h>
#include <Maths/Platform_GLToy_Maths.h>

// C/C++ headers
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

Platform_GLToy_Maths::CPUFeatures Platform_GLToy_Maths::s_xCPUFeatures;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void Platform_GLToy_Maths::Initialise()
{
    GLToy_DebugOutput( "\r\nPlatform_GLToy_Maths::Initialise() - Initialising CPU\r\n" );

    __asm
    {
        mov eax, 1
        cpuid
        mov s_xCPUFeatures.m_uEax, eax
        mov s_xCPUFeatures.m_uEbx, ebx
        mov s_xCPUFeatures.m_uEcx, ecx
        mov s_xCPUFeatures.m_uEdx, edx
    }

    GLToy_DebugOutput( "Family: %X, Model: %X, Stepping: %X\r\n",
        s_xCPUFeatures.m_uFamilyID, s_xCPUFeatures.m_uModelID, s_xCPUFeatures.m_uSteppingId );
    GLToy_DebugOutput( "Extended Family: %X, Extended Model: %X\r\n",
        s_xCPUFeatures.m_uExtendedFamilyID, s_xCPUFeatures.m_uExtendedModelID );
    GLToy_DebugOutput( "Available Instructions:\r\n" );
    GLToy_DebugOutput( "  FPU: %s\r\n", s_xCPUFeatures.m_bFPU ? "Yes" : "No" );
    GLToy_DebugOutput( "  MMX: %s\r\n", s_xCPUFeatures.m_bMMX ? "Yes" : "No" );
    GLToy_DebugOutput( "  SSE: %s\r\n", s_xCPUFeatures.m_bSSE ? "Yes" : "No" );
    GLToy_DebugOutput( "  SSE2: %s\r\n", s_xCPUFeatures.m_bSSE2 ? "Yes" : "No" );
    GLToy_DebugOutput( "  SSE3: %s\r\n", s_xCPUFeatures.m_bSSE3 ? "Yes" : "No" );
    GLToy_DebugOutput( "  SSSE3: %s\r\n", s_xCPUFeatures.m_bSSSE3 ? "Yes" : "No" );
    GLToy_DebugOutput( "  SSE4.1: %s\r\n", s_xCPUFeatures.m_bSSE41 ? "Yes" : "No" );
    GLToy_DebugOutput( "  SSE4.2: %s\r\n", s_xCPUFeatures.m_bSSE42 ? "Yes" : "No" );

    GLToy_Assert( s_xCPUFeatures.m_bFPU, "CPU has no built-in FPU!" );
}