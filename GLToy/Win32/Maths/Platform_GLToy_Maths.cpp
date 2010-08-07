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

	// __asm__
    // (
    //     "movl $1, %eax\n\t"
    //     "cpuid\n\t"
    //     "movl %eax, s_xCPUFeatures.m_uEax\n\t"
    //     "movl %ebx, s_xCPUFeatures.m_uEbx\n\t"
    //     "movl %ecx, s_xCPUFeatures.m_uEcx\n\t"
    //     "movl %edx, s_xCPUFeatures.m_uEdx\n\t"
    // );

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
    // __asm__( "fstcw [ usCW ]" );
    usCW &= ~0x300; // clear bits 8 and 9 - force low precision
    // __asm__( "fldcw [ usCW ]" );
}

float Platform_GLToy_Maths::Sqrt( const float fValue )
{
    // __asm__
    // (
    //     "fld [ fValue ]\n\t"
    //     "fsqrt\n\t"
    // );
}
