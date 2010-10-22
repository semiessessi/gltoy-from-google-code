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
        INSTRUCTION_FILL            = 0,
        INSTRUCTION_NOISE           = 1,
        INSTRUCTION_TILE            = 2,
        INSTRUCTION_CIRCLE          = 3,
        INSTRUCTION_FBMNOISE        = 4,
        INSTRUCTION_SHAPE           = 5,
        INSTRUCTION_GRADIENT        = 6,
        INSTRUCTION_EXTENSION       = 7,
    };

    enum BlendMode
    {
        BLEND_ALPHA                 = 0,
        BLEND_MUL                   = 1,
        BLEND_ADD                   = 2,
        BLEND_SUB                   = 3,
        BLEND_MAX                   = 4,
        BLEND_MIN                   = 5,
        BLEND_LUMINANCE_INTO_ALPHA  = 6,
        BLEND_REPLACE               = 7,
    };

    enum ShapeFunction
    {
        SHAPE_COS_2PI               = 0,
        SHAPE_SIN_2PI               = 1,
        SHAPE_COS_4PI               = 2,
        SHAPE_SIN_4PI               = 3,
        SHAPE_COS_6PI               = 4,
        SHAPE_SIN_6PI               = 5,
        SHAPE_SQUARE                = 6,
        SHAPE_SQUAREROOT            = 7,
        SHAPE_ABS                   = 8,
        SHAPE_HALF                  = 9,
        SHAPE_DOUBLE                = 10,
        SHAPE_CLIPBELOWHALF         = 11,
        SHAPE_CLIPABOVEHALF         = 12,
    };

    enum GradientStyle
    {
        GRADIENT_TOP                = 1,
        GRADIENT_BOTTOM             = 2,
        GRADIENT_LEFT               = 3,
        GRADIENT_RIGHT              = 4,
        GRADIENT_TOP_LEFT           = 5,
        GRADIENT_BOTTOM_LEFT        = 6,
        GRADIENT_TOP_RIGHT          = 7,
        GRADIENT_BOTTOM_RIGHT       = 8,
        GRADIENT_RADIAL_OUT         = 9,
        GRADIENT_RADIAL_IN          = 10,
        GRADIENT_SQUARE_OUT         = 11,
        GRADIENT_SQUARE_IN          = 12,
        GRADIENT_UNUSED1            = 13,
        GRADIENT_UNUSED2            = 14,
        GRADIENT_UNUSED3            = 15,
    };

    // extensions I'd like...
    // - pen - described by splines, driven by user input in tool (requires some more GLToy_Vector_2 implementation I suspect)
    // - luminance edge find for faking heightmap shading
    // - general convolutions (say, up to 7x7 with compression for 2d applied twice, symmetrical and each size up from 3x3)
    // - normal map type functionality
    //   * half-spheres
    //   * tubing
    //   * ribbed surface
    //   * bevels
    // - borders
    // - deformations
    // - reflections
    // - rotations
    // - polygons
    // - checkerboard
    // - fancy tile patterns (and matching normal map patterns)
    // - AVS style colour map
    enum ExtensionFunction
    {
        EXTENSION_UNUSED            = 0,
    };

    static const u_int uCURRENT_VERSION = 0; // version 0 - added basic functionality

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
        , m_eExtensionFunction( EXTENSION_UNUSED )
        , m_uParam1( 0 )
        , m_uParam2( 0 )
        , m_uParam3( 0 )
        , m_uID( s_uNextID )
        {
            ++s_uNextID;
        }

        LayerNode( const LayerNode& xLayerNode )
        : m_pxChildren( NULL )
        , m_eInstruction( xLayerNode.m_eInstruction )
        , m_eBlendMode( xLayerNode.m_eBlendMode )
        , m_eExtensionFunction( xLayerNode.m_eExtensionFunction )
        , m_uParam1( xLayerNode.m_uParam1 )
        , m_uParam2( xLayerNode.m_uParam2 )
        , m_uParam3( xLayerNode.m_uParam3 )
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

        static LayerNode CreateFill( const u_int uRGBA );
        static LayerNode CreateNoise( const float fFrequency, const u_int uSeed );
        static LayerNode CreateTile( const u_int uFrequency );
        static LayerNode CreateCircle( const GLToy_Vector_2& xPosition, const float fRadius, const u_int uRGBA );
        static LayerNode CreateFBMNoise( const float fFrequency, const u_int uSeed );
        static LayerNode CreateShaping( const ShapeFunction m_eShapeFunction );
        static LayerNode CreateGradient( const GradientStyle m_eGradientStyle );

        static LayerNode CreateGroup();

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
                case INSTRUCTION_FILL:      return "Fill";
                case INSTRUCTION_NOISE:     return "Noise";
                case INSTRUCTION_TILE:      return "Tile";
                case INSTRUCTION_CIRCLE:    return "Circle";
                case INSTRUCTION_FBMNOISE:  return "Fractal Brownian Motion Noise";
                case INSTRUCTION_SHAPE:     return "Shaping";
                default:                    return "Unknown";
            }
        }

        virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
        virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

        void Render( const u_int uWidth, const u_int uHeight );

    private:

        GLToy_SmallSerialisableArray< LayerNode >* m_pxChildren;
        Instruction m_eInstruction;
        BlendMode m_eBlendMode;
        ExtensionFunction m_eExtensionFunction;
        
        union
        {
            u_int m_uParam1;
            float m_fParam1;
        }
        ;
        union
        {
            u_int m_uParam2;
            float m_fParam2;
        };

        union
        {
            u_int m_uParam3;
            float m_fParam3;
        };

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
        m_xLayers.Append( LayerNode::CreateNoise( fFrequency, uSeed ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendTileLayer( const u_int uTileFrequency )
    {
        m_xLayers.Append( LayerNode::CreateTile( uTileFrequency ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendCircleLayer( const GLToy_Vector_2& xCentre, const float fRadius, const u_int uRGBA )
    {
        m_xLayers.Append( LayerNode::CreateCircle( xCentre, fRadius, uRGBA ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendFBMNoiseLayer( const float fFrequency = 32.0f, const u_int uSeed = 0 )
    {
        m_xLayers.Append( LayerNode::CreateFBMNoise( fFrequency, uSeed ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendShapingLayer( const ShapeFunction eShapeFunction )
    {
        m_xLayers.Append( LayerNode::CreateShaping( eShapeFunction ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendGradientLayer( const GradientStyle eGradientStyle )
    {
        m_xLayers.Append( LayerNode::CreateGradient( eGradientStyle ) );
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

    void ReadNoHeader( const char* const pcData, const u_int uLength );
    void SaveToCPPHeader( const GLToy_String& szName );

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