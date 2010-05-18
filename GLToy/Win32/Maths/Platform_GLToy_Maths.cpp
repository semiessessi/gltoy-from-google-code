/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
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

bool Platform_GLToy_Maths::Initialise()
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

    // demand that we at least have an FPU
    return s_xCPUFeatures.m_bFPU;
}

void Platform_GLToy_Maths::Update()
{
    // TODO - intrinsics or something
    unsigned short usCW;
    __asm fstcw [ usCW ];
    usCW &= ~0x300; // clear bits 8 and 9 - force low precision
    __asm fldcw [ usCW ];
}

float Platform_GLToy_Maths::Sqrt( const float fValue )
{
    __asm
    {
        fld [ fValue ]
        fsqrt
    }
}
