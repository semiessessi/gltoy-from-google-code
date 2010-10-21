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
        INSTRUCTION_TILE = 2,
    };

    enum BlendMode
    {
        BLEND_ALPHA = 0,
        BLEND_MUL = 1,
        BLEND_ADD = 2,
    };

    class LayerNode
    {

    public:

        LayerNode()
        : m_pxChildren( NULL )
        , m_eInstruction( INSTRUCTION_FILL )
        , m_eBlendMode( BLEND_ALPHA )
        , m_uParam1( 0 )
        , m_uID( s_uNextID )
        {
            ++s_uNextID;
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

        static LayerNode CreateTile( const u_int uFrequency )
        {
            LayerNode xReturnValue;
            xReturnValue.m_uParam1 = uFrequency;
        }

        static LayerNode CreateGroup()
        {
            LayerNode xReturnValue;
            xReturnValue.m_pxChildren = new GLToy_Array< LayerNode >();
        }

        GLToy_Array< LayerNode >* GetChildren()
        {
            return m_pxChildren;
        }

        const GLToy_Array< LayerNode >* GetChildren() const
        {
            return m_pxChildren;
        }

        u_int GetID() const
        {
            return m_uID;
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

                case INSTRUCTION_TILE:
                {
                    return "Tile";
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
        BlendMode m_eBlendMode;
        u_int m_uParam1;
        u_int m_uID;

        static u_int s_uNextID;

    };

public:

    GLToy_Texture_Procedural()
    : m_xLayers()
    {
    }

    virtual ~GLToy_Texture_Procedural() {}

    void AppendFillLayer( const u_int uRGBA = 0 )
    {
        m_xLayers.Append( LayerNode::CreateFill( uRGBA ) );
    }

    void AppendNoiseLayer( const float fFrequency = 32.0f )
    {
        m_xLayers.Append( LayerNode::CreateNoise( fFrequency ) );
    }

protected:

    LayerNode* GetLayerNodeFromID( const u_int uID, GLToy_Array< LayerNode>* pxLayers = NULL )
    {
        if( !pxLayers )
        {
            pxLayers = &m_xLayers;
        }

        GLToy_Iterate( LayerNode, xIterator, pxLayers )
        {
            LayerNode& xLayerNode = xIterator.Current();
            if( xLayerNode.GetID() == uID )
            {
                return &xLayerNode;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                LayerNode* pxLayerNode = GetLayerNodeFromID( uID, xLayerNode.GetChildren() );
                if( pxLayerNode )
                {
                    return pxLayerNode;
                }
            }
        }

        return NULL;
    }

    GLToy_Array< LayerNode > m_xLayers;

};

#endif