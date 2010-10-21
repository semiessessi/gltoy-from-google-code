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
#include <Core/Data Structures/GLToy_Stack.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture_Procedural
: public GLToy_Serialisable
{

public:
    
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
        BLEND_SUB = 3,
    };

private:

    class LayerNode
    : public GLToy_Serialisable
    {
        friend class GLToy_Texture_Procedural;

    public:

        LayerNode()
        : m_pxChildren( NULL )
        , m_eInstruction( INSTRUCTION_FILL )
        , m_eBlendMode( BLEND_ALPHA )
        , m_uParam1( 0 )
        , m_uParam2( 0 )
        , m_uID( s_uNextID )
        {
            ++s_uNextID;
        }

        LayerNode( const LayerNode& xLayerNode )
        : m_pxChildren( NULL )
        , m_eInstruction( xLayerNode.m_eInstruction )
        , m_eBlendMode( xLayerNode.m_eBlendMode )
        , m_uParam1( xLayerNode.m_uParam1 )
        , m_uParam2( xLayerNode.m_uParam2 )
        , m_uID( xLayerNode.m_uID )
        {
            if( xLayerNode.m_pxChildren )
            {
                m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >( *( xLayerNode.m_pxChildren ) );
            }
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
            return xReturnValue;
        }

        static LayerNode CreateNoise( const float fFrequency, const u_int uSeed )
        {
            LayerNode xReturnValue;
            xReturnValue.m_eInstruction = INSTRUCTION_NOISE;
            xReturnValue.m_uParam1 = *reinterpret_cast< const u_int* >( &fFrequency );
            xReturnValue.m_uParam2 = uSeed;
            return xReturnValue;
        }

        static LayerNode CreateTile( const u_int uFrequency )
        {
            LayerNode xReturnValue;
            xReturnValue.m_eInstruction = INSTRUCTION_TILE;
            xReturnValue.m_uParam1 = uFrequency;
            return xReturnValue;
        }

        static LayerNode CreateGroup()
        {
            LayerNode xReturnValue;
            xReturnValue.m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >();
            return xReturnValue;
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

        virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
        virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

        void Render( const u_int uWidth, const u_int uHeight );

    private:

        GLToy_SmallSerialisableArray< LayerNode >* m_pxChildren;
        Instruction m_eInstruction;
        BlendMode m_eBlendMode;
        u_int m_uParam1;
        u_int m_uParam2;
        u_int m_uID;

        static u_int s_uNextID;

        static GLToy_Stack< u_int* > s_xRenderStack;

    };

public:

    GLToy_Texture_Procedural()
    : m_xLayers()
    {
    }

    virtual ~GLToy_Texture_Procedural() {}

    u_int AppendFillLayer( const u_int uRGBA = 0 )
    {
        m_xLayers.Append( LayerNode::CreateFill( uRGBA ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendNoiseLayer( const float fFrequency = 32.0f, const u_int uSeed = 0 )
    {
        m_xLayers.Append( LayerNode::CreateNoise( fFrequency , uSeed ) );
        return m_xLayers.End().GetID();
    }

    void CreateTexture( const GLToy_String& szName, const u_int uWidth, const u_int uHeight );
    void SetBlendMode( const u_int uID, const BlendMode eBlendMode )
    {
        LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            pxLayerNode->m_eBlendMode = eBlendMode;
        }
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

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

    GLToy_SmallSerialisableArray< LayerNode > m_xLayers;

};

#endif