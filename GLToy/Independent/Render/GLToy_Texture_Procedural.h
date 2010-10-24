/////////////////////////////////////////////////////////////////////////////////////////////
//
// �Copyright 2010 Semi Essessi
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
        INSTRUCTION_FILL                    = 0,
        INSTRUCTION_NOISE                   = 1,
        INSTRUCTION_TILE                    = 2,
        INSTRUCTION_CIRCLE                  = 3,
        INSTRUCTION_FBMNOISE                = 4,
        INSTRUCTION_SHAPE                   = 5,
        INSTRUCTION_GRADIENT                = 6,
        INSTRUCTION_EXTENSION               = 7,
    };

    enum BlendMode
    {
        BLEND_ALPHA                         = 0,
        BLEND_MUL                           = 1,
        BLEND_ADD                           = 2,
        BLEND_SUB                           = 3,
        BLEND_MAX                           = 4,
        BLEND_MIN                           = 5,
        BLEND_LUMINANCE_INTO_ALPHA          = 6,
        BLEND_REPLACE                       = 7,
    };

    enum ShapeFunction
    {
        SHAPE_COS_2PI                       = 0,
        SHAPE_SIN_2PI                       = 1,
        SHAPE_COS_4PI                       = 2,
        SHAPE_SIN_4PI                       = 3,
        SHAPE_COS_6PI                       = 4,
        SHAPE_SIN_6PI                       = 5,
        SHAPE_SQUARE                        = 6,
        SHAPE_SQUAREROOT                    = 7,
        SHAPE_ABS                           = 8,
        SHAPE_HALF                          = 9,
        SHAPE_DOUBLE                        = 10,
        SHAPE_CLIPBELOWHALF                 = 11,
        SHAPE_CLIPABOVEHALF                 = 12,
        SHAPE_INVERT                        = 13,
        SHAPE_MIN_50                        = 14,
        SHAPE_MAX_50                        = 15,
        SHAPE_MIN_25                        = 16,
        SHAPE_MAX_25                        = 17,
        SHAPE_MIN_75                        = 18,
        SHAPE_MAX_75                        = 19,
        SHAPE_MIN_10                        = 20,
        SHAPE_MAX_10                        = 21,
        SHAPE_MIN_90                        = 22,
        SHAPE_MAX_90                        = 23,
        SHAPE_INFINITE_MULTIPLIER           = 24, // 'just' another 40 to use up :)

        SHAPE_FUNCTION_LAST
    };

    enum GradientStyle
    {
        GRADIENT_TOP                        = 1,
        GRADIENT_BOTTOM                     = 2,
        GRADIENT_LEFT                       = 3,
        GRADIENT_RIGHT                      = 4,
        GRADIENT_TOP_LEFT                   = 5,
        GRADIENT_BOTTOM_LEFT                = 6,
        GRADIENT_TOP_RIGHT                  = 7,
        GRADIENT_BOTTOM_RIGHT               = 8,
        GRADIENT_RADIAL_OUT                 = 9,
        GRADIENT_RADIAL_IN                  = 10,
        GRADIENT_SQUARE_OUT                 = 11,
        GRADIENT_SQUARE_IN                  = 12,
        GRADIENT_UNUSED1                    = 13,
        GRADIENT_UNUSED2                    = 14,
        GRADIENT_UNUSED3                    = 15,
    };

    // extensions I'd like...
    // - pen - described by splines, driven by user input in tool (requires some more GLToy_Vector_2 implementation I suspect)
    // - luminance edge find for faking heightmap shading
    // - general convolutions (say, up to 7x7 with compression for 2d applied twice, symmetrical and each size up from 3x3)
    // - deformations
    // - reflections
    // - rotations
    // - polygons
    // - AVS style colour map
    enum ExtensionFunction
    {
        EXTENSION_CHECKERBOARD              = 0,
        EXTENSION_HORIZONTAL_STRIPE         = 1,
        EXTENSION_VERTICAL_STRIPE           = 2,
        EXTENSION_DIAGONAL_STRIPE_UPLEFT    = 3,
        EXTENSION_DIAGONAL_STRIPE_DOWNLEFT  = 4,

        // some stuff for baked in highlights and normal maps
        // EXTENSION_SET_HIGHLIGHT_DIRECTION
        // EXTENSION_SET_HIGHLIGHT_COLOUR
        // EXTENSION_HEIGHTMAP_HIGHLIGHT
        // EXTENSION_HEIGHTMAP_NORMALS

        // to cover things like bricks, floor tiles etc...
        // EXTENSION_PATTERN
        // EXTENSION_PATTERN_NORMALS

        // normal mapping
        // EXTENSION_NORMAL_MAP_HALFSPHERE
        // EXTENSION_NORMAL_MAP_METALLUMPS
        // EXTENSION_NORMAL_MAP_RIBBED

        // bevels/borders
        // EXTENSION_BORDER
        // EXTENSION_BEVEL
        // EXTENSION_BEVEL_NORMALS
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
        , m_eExtensionFunction( EXTENSION_CHECKERBOARD )
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

        LayerNode& operator =( const LayerNode& xLayerNode )
        {
            delete m_pxChildren;
            m_pxChildren = NULL;

            if( xLayerNode.m_pxChildren )
            {
                m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >( *( xLayerNode.m_pxChildren ) );
            }

            m_eInstruction = xLayerNode.m_eInstruction;
            m_eBlendMode = xLayerNode.m_eBlendMode;
            m_eExtensionFunction = xLayerNode.m_eExtensionFunction;
            m_uParam1 = xLayerNode.m_uParam1;
            m_uParam2 = xLayerNode.m_uParam2;
            m_uParam3 = xLayerNode.m_uParam3;
            m_uID = xLayerNode.m_uID;

            return *this;
        }

        ~LayerNode()
        {
            delete m_pxChildren;
        }

        bool IsLeaf() const
        {
            return m_pxChildren == NULL;
        }

        void AssignNewID()
        {
            m_uID = s_uNextID;
            ++s_uNextID;
        }

        static LayerNode CreateFill( const u_int uRGBA );
        static LayerNode CreateNoise( const float fFrequency, const u_int uSeed );
        static LayerNode CreateTile( const u_int uFrequency );
        static LayerNode CreateCircle( const GLToy_Vector_2& xPosition, const float fRadius, const u_int uRGBA );
        static LayerNode CreateFBMNoise( const float fFrequency, const u_int uSeed );
        static LayerNode CreateShaping( const ShapeFunction eShapeFunction );
        static LayerNode CreateGradient( const GradientStyle eGradientStyle );
        static LayerNode CreateExtension( const ExtensionFunction eExtensionFunction, const u_int uParam1 = 0, const u_int uParam2 = 0, const u_int uParam3 = 0 );
        
        static LayerNode CreateExtension( const ExtensionFunction eExtensionFunction, const float fParam1, const u_int uParam2 = 0, const u_int uParam3 = 0 )
        {
            CreateExtension( eExtensionFunction, *reinterpret_cast< const u_int* >( &fParam1 ), uParam2, uParam3 );
        }

        static LayerNode CreateExtension( const ExtensionFunction eExtensionFunction, const float fParam1, const float fParam2, const u_int uParam3 = 0 )
        {
            CreateExtension( eExtensionFunction, *reinterpret_cast< const u_int* >( &fParam1 ), *reinterpret_cast< const u_int* >( &fParam2 ), uParam3 );
        }

        static LayerNode CreateExtension( const ExtensionFunction eExtensionFunction, const u_char ucParam1, const u_char ucParam2 = 0, const u_char ucParam3 = 0, const u_char ucParam4 = 0 )
        {
            union
            {
                u_int u;
                u_char auc[ 4 ];
            };

            auc[ 0 ] = ucParam1;
            auc[ 1 ] = ucParam2;
            auc[ 2 ] = ucParam3;
            auc[ 3 ] = ucParam4;

            CreateExtension( eExtensionFunction, u );
        }

        static LayerNode CreateGroup();

        GLToy_SmallSerialisableArray< LayerNode >* GetChildren()
        {
            return m_pxChildren;
        }

        const GLToy_SmallSerialisableArray< LayerNode >* GetChildren() const
        {
            return m_pxChildren;
        }

        u_int GetID() const
        {
            return m_uID;
        }

        const char* GetInstructionName() const
        {
            if( GetChildren() )
            {
                return "Group";
            }

            switch( m_eInstruction )
            {
                case INSTRUCTION_FILL:                  return "Fill";
                case INSTRUCTION_NOISE:                 return "Noise";
                case INSTRUCTION_TILE:                  return "Tiling";
                case INSTRUCTION_CIRCLE:                return "Circle";
                case INSTRUCTION_FBMNOISE:              return "Fractal Noise";
                case INSTRUCTION_SHAPE:                 return "Shaping Function";
                case INSTRUCTION_GRADIENT:              return "Gradient";
                case INSTRUCTION_EXTENSION:
                {
                    switch( m_eExtensionFunction )
                    {
                        case EXTENSION_CHECKERBOARD:                return "Checkerboard";
                        case EXTENSION_HORIZONTAL_STRIPE:           return "Horizontal Stripe";
                        case EXTENSION_VERTICAL_STRIPE:             return "Vertical Stripe";
                        case EXTENSION_DIAGONAL_STRIPE_UPLEFT:      return "Diagonal Stripe (Up-Left)";
                        case EXTENSION_DIAGONAL_STRIPE_DOWNLEFT:    return "Diagonal Stripe (Down-Left)";
                        default:                                    return "Unnamed Extension";
                    }
                }
                default:                    return "Unknown Instruction";
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
            u_char m_aucParam1[ 4 ];
            u_short m_ausParam1[ 2 ];
        }
        ;
        union
        {
            u_int m_uParam2;
            float m_fParam2;
            u_char m_aucParam2[ 4 ];
            u_short m_ausParam2[ 2 ];
        };

        union
        {
            u_int m_uParam3;
            float m_fParam3;
            u_char m_aucParam3[ 4 ];
            u_short m_ausParam3[ 2 ];
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

    u_int AppendGroup()
    {
        m_xLayers.Append( LayerNode::CreateGroup() );
        return m_xLayers.End().GetID();
    }

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

    u_int AppendCheckerboardLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( EXTENSION_CHECKERBOARD ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendHorizontalStripeLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( EXTENSION_HORIZONTAL_STRIPE ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendVerticalStripeLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( EXTENSION_VERTICAL_STRIPE ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendDiagonalStripeUpLeftLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( EXTENSION_DIAGONAL_STRIPE_UPLEFT ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendDiagonalStripeDownLeftLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( EXTENSION_DIAGONAL_STRIPE_DOWNLEFT ) );
        return m_xLayers.End().GetID();
    }

    u_int MoveLayerAfter( const u_int uID, const u_int uAfterID );
    u_int MoveLayerBefore( const u_int uID, const u_int uBeforeID );
    u_int MoveLayerUnder( const u_int uID, const u_int uUnderID );
    u_int MoveLayerToOwnGroup( const u_int uID );

    u_int* CreateRGBA( const u_int uWidth, const u_int uHeight );
    void CreateTexture( const GLToy_String& szName, const u_int uWidth, const u_int uHeight );
    
    BlendMode GetBlendMode( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            return pxLayerNode->m_eBlendMode;
        }

        return BLEND_ALPHA;
    }

    Instruction GetInstruction( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            return pxLayerNode->m_eInstruction;
        }

        return INSTRUCTION_FILL;
    }

    void SetBlendMode( const u_int uID, const BlendMode eBlendMode )
    {
        LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            pxLayerNode->m_eBlendMode = eBlendMode;
        }
    }

    u_int GetParam1( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            return pxLayerNode->m_uParam1;
        }

        return 0;
    }

    u_int GetParam2( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            return pxLayerNode->m_uParam2;
        }

        return 0;
    }

    void SetParam1( const u_int uID, const u_int uValue )
    {
        LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            pxLayerNode->m_uParam1 = uValue;
        }
    }

    void SetParam1( const u_int uID, const float fValue )
    {
        LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            pxLayerNode->m_fParam1 = fValue;
        }
    }

    void SetParam2( const u_int uID, const u_int uValue )
    {
        LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            pxLayerNode->m_uParam2 = uValue;
        }
    }

    void SetParam3( const u_int uID, const u_int uValue )
    {
        LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( pxLayerNode )
        {
            pxLayerNode->m_uParam3 = uValue;
        }
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    void ReadNoHeader( const char* const pcData, const u_int uLength );
    void SaveToCPPHeader( const GLToy_String& szName, const GLToy_String* pszFilename = NULL );
    void SaveToTGAFile( const GLToy_String& szFilename, const u_int uSize = 256 );

    u_int GetLayerCount( const u_int uParentID = 0 ) const
    {
        if( uParentID == 0 )
        {
            return m_xLayers.GetCount();
        }

        const LayerNode* pxLayerNode = GetLayerNodeFromID( uParentID );
        if( pxLayerNode && !pxLayerNode->IsLeaf() )
        {
            const GLToy_Array< LayerNode >* pxArray = pxLayerNode->GetChildren();
            if( pxArray )
            {
                return pxArray->GetCount();
            }
        }

        return 0;
    }

    u_int GetLayerIDFromIndex( const u_int uIndex, const u_int uParentID = 0 ) const
    {
        if( uParentID == 0 )
        {
            return m_xLayers[ uIndex ].GetID();
        }

        const LayerNode* pxLayerNode = GetLayerNodeFromID( uParentID );
        if( pxLayerNode && !pxLayerNode->IsLeaf() )
        {
            const GLToy_Array< LayerNode >* pxArray = pxLayerNode->GetChildren();
            if( pxArray )
            {
                return ( *pxArray )[ uIndex ].GetID();
            }
        }

        return 0;
    }

    const char* GetLayerName( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( !pxLayerNode )
        {
            return NULL;
        }

        return pxLayerNode->GetInstructionName();
    }

    const char* GetShapingFunctionNameFromID( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( !pxLayerNode )
        {
            return NULL;
        }

        return GetShapingFunctionName( static_cast< ShapeFunction >( pxLayerNode->m_uParam1 ) );
    }

    void DeleteFromID( const u_int uID ) { DeleteLayerNodeFromID( uID ); }
    bool IsLeaf( const u_int uID ) { LayerNode* pxLayerNode = GetLayerNodeFromID( uID ); return pxLayerNode ? pxLayerNode->IsLeaf() : false; }

    static const char* GetShapingFunctionName( const ShapeFunction eFunction );
protected:

    LayerNode* GetLayerNodeFromID( const u_int uID, GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL )
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

    const LayerNode* GetLayerNodeFromID( const u_int uID, const GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL ) const
    {
        if( !pxLayers )
        {
            pxLayers = &m_xLayers;
        }

        GLToy_ConstIterate( LayerNode, xIterator, pxLayers )
        {
            const LayerNode& xLayerNode = xIterator.Current();
            if( xLayerNode.GetID() == uID )
            {
                return &xLayerNode;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                const LayerNode* pxLayerNode = GetLayerNodeFromID( uID, xLayerNode.GetChildren() );
                if( pxLayerNode )
                {
                    return pxLayerNode;
                }
            }
        }

        return NULL;
    }

    bool DeleteLayerNodeFromID( const u_int uID, GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL )
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
                pxLayers->RemoveAt( xIterator.Index() );
                return true;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                bool bDeleted = DeleteLayerNodeFromID( uID, xLayerNode.GetChildren() );
                if( bDeleted )
                {
                    return true;
                }
            }
        }

        return false;
    }

    GLToy_SmallSerialisableArray< LayerNode >* GetParentArrayFromID( const u_int uID, GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL )
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
                return pxLayers;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                LayerNode* pxLayerNode = GetLayerNodeFromID( uID, xLayerNode.GetChildren() );
                if( pxLayerNode )
                {
                    return xLayerNode.GetChildren();
                }
            }
        }

        return NULL;
    }

    u_int GetIndexInArrayFromID( const u_int uID, GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL )
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
                return xIterator.Index();
            }
            else if( !xLayerNode.IsLeaf() )
            {
                const u_int uChildID = GetIndexInArrayFromID( uID, xLayerNode.GetChildren() );
                if( uChildID != 0xFFFFFFFF )
                {
                    return uChildID;
                }
            }
        }

        return 0xFFFFFFFF;
    }

    GLToy_SmallSerialisableArray< LayerNode > m_xLayers;

};

#endif