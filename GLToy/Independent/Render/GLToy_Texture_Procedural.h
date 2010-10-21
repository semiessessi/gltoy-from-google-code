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

#ifndef __GLTOY_TEXTURE_PROCEDURAL_H_
#define __GLTOY_TEXTURE_PROCEDURAL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture_Procedural
{
    enum Instruction
    {
        INSTRUCTION_FILL = 0,
        INSTRUCTION_NOISE = 1,
    };

    class LayerNode
    {

    public:

        LayerNode()
        : m_pxChildren( NULL )
        , m_eInstruction( INSTRUCTION_FILL )
        , m_uParam1( 0 )
        {
        }

        ~LayerNode()
        {
            delete m_pxChildren;
        }

        bool IsLeaf() const
        {
            return m_pxChildren == NULL;
        }

        static LayerNode CreateFill( const u_int uRGBA )
        {
            LayerNode xReturnValue;
            xReturnValue.m_uParam1 = uRGBA;
        }

        static LayerNode CreateNoise( const float fFrequency )
        {
            LayerNode xReturnValue;
            xReturnValue.m_uParam1 = *reinterpret_cast< const u_int* >( &fFrequency );
        }

        static LayerNode CreateGroup()
        {
            LayerNode xReturnValue;
            xReturnValue.m_pxChildren = new GLToy_Array< LayerNode >();
        }

        const char* GetInstructionName() const
        {
            switch( m_eInstruction )
            {
                case INSTRUCTION_FILL:
                {
                    return "Fill";
                }
                
                case INSTRUCTION_NOISE:
                {
                    return "Noise";
                }
                default:
                {
                    return "Unknown";
                }
            }
        }

    private:

        GLToy_Array< LayerNode >* m_pxChildren;
        Instruction m_eInstruction;
        u_int m_uParam1;

    };

public:

    GLToy_Texture_Procedural()
    {
    }

    virtual ~GLToy_Texture_Procedural() {}

protected:

    GLToy_Array< LayerNode > m_xLayers;

};

#endif