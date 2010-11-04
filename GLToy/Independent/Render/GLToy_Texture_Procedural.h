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

// TODO: sort out this header - its huge

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_Stack.h>
#include <Maths/GLToy_Vector.h>

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
        GRADIENT_UNUSED1                    = 0,
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
        GRADIENT_LAST                       = 13,
        GRADIENT_UNUSED2                    = 13,
        GRADIENT_UNUSED3                    = 14,
        GRADIENT_UNUSED4                    = 15,
    };

    enum PatternStyle
    {
        PATTERN_DEFAULT_BRICK               = 0,

        PATTERN_LAST
    };

    // extensions I'd like...
    // - pen - described by splines, driven by user input in tool (requires some more GLToy_Vector_2 implementation I suspect)
    // - luminance edge find for faking heightmap shading
    // - general convolutions (say, up to 7x7 with compression for 2d applied twice, symmetrical and each size up from 3x3)
    // - deformations
    // - reflections
    // - polygons
    // - AVS style colour map
    enum ExtensionFunction
    {
        // simple patterns
        EXTENSION_CHECKERBOARD                      = 0,
        EXTENSION_HORIZONTAL_STRIPE                 = 1,
        EXTENSION_VERTICAL_STRIPE                   = 2,
        EXTENSION_DIAGONAL_STRIPE_UPLEFT            = 3,
        EXTENSION_DIAGONAL_STRIPE_DOWNLEFT          = 4,

        // to save repeating layers/groups
        EXTENSION_REFERENCE                         = 5,

        // bevels/borders
        EXTENSION_BORDER                            = 6,
        EXTENSION_BEVEL                             = 7,
        EXTENSION_BEVEL_NORMALS                     = 8,
        //EXTENSION_RECTANGLE                         = 9,
        //EXTENSION_BEVEL_RECTANGLE                   = 10,
        //EXTENSION_BEVEL_NORMALS_RECTANGLE           = 11,

        // state
        EXTENSION_TEXTURE_MODE                      = 12, // quite necessary for convolution, normals from heightmaps and loads of stuff to choose between wrap and clamp
        // EXTENSION_SET_HIGHLIGHT_DIRECTION           = 13, // need this to generate highlights from normal maps

        // heightmap highlights and normal maps
        EXTENSION_HEIGHTMAP_HIGHLIGHT               = 14,
        EXTENSION_HEIGHTMAP_NORMALS                 = 15,

        // to cover things like bricks, floor tiles etc...
        EXTENSION_PATTERN                           = 16,
        // EXTENSION_PATTERN_NORMALS                   = 17,
        // EXTENSION_PATTERN_HEIGHTMAP                 = 18,
        // EXTENSION_PATTERN_BRICK                     = 19,
        // EXTENSION_PATTERN_BRICK_NORMALS             = 20,
        // EXTENSION_PATTERN_BRICK_HEIGHTMAP           = 21,

        // just filling in a gap with a transform...
        EXTENSION_ROTATE                            = 22,

        // convolution and similar filters
        EXTENSION_CONVOLUTION_SIMPLE                = 23,
        // EXTENSION_CONVOLUTION_FULL                  = 24,
        // EXTENSION_ENHANCE_EDGES                     = 25,
        // EXTENSION_FIND_EDGES                        = 26,
        // EXTENSION_SOFTEN_EDGES                      = 27,
        
        // normal mapping
        // EXTENSION_NORMAL_MAP_HALFSPHERE             = 28,
        // EXTENSION_NORMAL_MAP_HALFCYLINDER           = 29,
        // EXTENSION_NORMAL_MAP_PYRAMID                = 30,
        // EXTENSION_NORMAL_MAP_PRISM                  = 31,

        // transforms
        // EXTENSION_TRANSLATE                         = 32,
        EXTENSION_SCALE                             = 33,
        EXTENSION_ROTATE_VANILLA                    = 34,
        EXTENSION_NOISE_DEFORM                      = 35,

        // Voronoi "cellular" pattern
        // EXTENSION_VORONOI_PATTERN                   = 36,
        
        // AVS style colour map
        // EXTENSION_COLOUR_MAP                        = 37,

        // typical primitives
        // EXTENSION_LINE_STRIP                        = 38,
        // EXTENSION_POLYGON                           = 39,
        // EXTENSION_SPLINE                            = 40,
        // EXTENSION_SPLINE_POLYGON                    = 41,

        // fill areas with patterns
        // EXTENSION_PATTERN_RECTANGLE                 = 42,
        // EXTENSION_PATTERN_RECTANGLE_NORMALS         = 43,
        // EXTENSION_PATTERN_RECTANGLE_HEIGHTMAP       = 44,
        // EXTENSION_PATTERN_THICK_SPLINE              = 45,
        // EXTENSION_PATTERN_THICK_SPLINE_NORMALS      = 46,
        // EXTENSION_PATTERN_THICK_SPLINE_HEIGHTMAP    = 47,
        // EXTENSION_PATTERN_ELLIPSE                   = 48,
        // EXTENSION_PATTERN_ELLIPSE_NORMALS           = 49,
        // EXTENSION_PATTERN_ELLIPSE_HEIGHTMAP         = 50,

        EXTENSION_BAD = 63,
    };

    static const u_int uCURRENT_VERSION = 0; // version 0 - added basic functionality

private:

    class LayerNode
    : public GLToy_Serialisable
    {
        friend class GLToy_Texture_Procedural;

    public:

        LayerNode( GLToy_Texture_Procedural* const pxParentTexture = NULL )
        : m_pxChildren( NULL )
        , m_eInstruction( INSTRUCTION_FILL )
        , m_eBlendMode( BLEND_ALPHA )
        , m_eExtensionFunction( EXTENSION_CHECKERBOARD )
        , m_uParam1( 0 )
        , m_uParam2( 0 )
        , m_uParam3( 0 )
        , m_uID( s_uNextID )
        , m_pxParentTexture( pxParentTexture )
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
        , m_pxParentTexture( xLayerNode.m_pxParentTexture )
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
            m_pxParentTexture = xLayerNode.m_pxParentTexture;

            return *this;
        }

        ~LayerNode()
        {
            delete m_pxChildren;
            m_pxChildren = NULL;
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

        void SetParent( GLToy_Texture_Procedural* const pxParentTexture ) { m_pxParentTexture = pxParentTexture; }

        static LayerNode CreateFill( GLToy_Texture_Procedural* const pxParentTexture, const u_int uRGBA );
        static LayerNode CreateNoise( GLToy_Texture_Procedural* const pxParentTexture, const float fFrequency, const u_int uSeed );
        static LayerNode CreateTile( GLToy_Texture_Procedural* const pxParentTexture, const u_int uFrequency );
        static LayerNode CreateCircle( GLToy_Texture_Procedural* const pxParentTexture, const GLToy_Vector_2& xPosition, const float fRadius, const u_int uRGBA );
        static LayerNode CreateFBMNoise( GLToy_Texture_Procedural* const pxParentTexture, const float fFrequency, const u_int uSeed );
        static LayerNode CreateShaping( GLToy_Texture_Procedural* const pxParentTexture, const ShapeFunction eShapeFunction );
        static LayerNode CreateGradient( GLToy_Texture_Procedural* const pxParentTexture, const GradientStyle eGradientStyle );
        static LayerNode CreateExtension( GLToy_Texture_Procedural* const pxParentTexture, const ExtensionFunction eExtensionFunction, const u_int uParam1 = 0, const u_int uParam2 = 0, const u_int uParam3 = 0 );
        
        static LayerNode CreateExtension( GLToy_Texture_Procedural* const pxParentTexture, const ExtensionFunction eExtensionFunction, const float fParam1, const u_int uParam2 = 0, const u_int uParam3 = 0 )
        {
            CreateExtension( pxParentTexture, eExtensionFunction, *reinterpret_cast< const u_int* >( &fParam1 ), uParam2, uParam3 );
        }

        static LayerNode CreateExtension( GLToy_Texture_Procedural* const pxParentTexture, const ExtensionFunction eExtensionFunction, const float fParam1, const float fParam2, const u_int uParam3 = 0 )
        {
            CreateExtension( pxParentTexture, eExtensionFunction, *reinterpret_cast< const u_int* >( &fParam1 ), *reinterpret_cast< const u_int* >( &fParam2 ), uParam3 );
        }

        static LayerNode CreateExtension( GLToy_Texture_Procedural* const pxParentTexture, const ExtensionFunction eExtensionFunction, const u_char ucParam1, const u_char ucParam2 = 0, const u_char ucParam3 = 0, const u_char ucParam4 = 0 )
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

            CreateExtension( pxParentTexture, eExtensionFunction, u );
        }

        static LayerNode CreateGroup( GLToy_Texture_Procedural* const pxParentTexture );
        static GLToy_Vector_4 WrapAwareSample( const int u, const int v, const u_int uWidth, const u_int uHeight, const GLToy_Vector_4* const pxBuffer  );
        static GLToy_Vector_4 WrapAwareSampleFiltered( const float fX, const float fY, const u_int uWidth, const u_int uHeight, const GLToy_Vector_4* const pxBuffer  );

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
                case INSTRUCTION_FILL:                                  return "Flat Colour";
                case INSTRUCTION_NOISE:                                 return "Noise";
                case INSTRUCTION_TILE:                                  return "Tiling";
                case INSTRUCTION_CIRCLE:                                return "Circle";
                case INSTRUCTION_FBMNOISE:                              return "Fractal Noise";
                case INSTRUCTION_SHAPE:                                 return "Shaping Function";
                case INSTRUCTION_GRADIENT:                              return "Gradient";
                case INSTRUCTION_EXTENSION:
                {
                    switch( m_eExtensionFunction )
                    {
                        case EXTENSION_CHECKERBOARD:                    return "Checkerboard";
                        case EXTENSION_HORIZONTAL_STRIPE:               return "Horizontal Stripe";
                        case EXTENSION_VERTICAL_STRIPE:                 return "Vertical Stripe";
                        case EXTENSION_DIAGONAL_STRIPE_UPLEFT:          return "Diagonal Stripe (Up-Left)";
                        case EXTENSION_DIAGONAL_STRIPE_DOWNLEFT:        return "Diagonal Stripe (Down-Left)";
                        case EXTENSION_REFERENCE:                       return "Reference";
                        case EXTENSION_BORDER:                          return "Border";
                        case EXTENSION_BEVEL:                           return "Bevel";
                        case EXTENSION_BEVEL_NORMALS:                   return "Bevel (Normals)";
                        case EXTENSION_TEXTURE_MODE:                    return "Set Texture Sampling Mode";
                        case EXTENSION_HEIGHTMAP_HIGHLIGHT:             return "Convert Heightmap to Highlights";
                        case EXTENSION_HEIGHTMAP_NORMALS:               return "Convert Heightmap to Normals";
                        case EXTENSION_PATTERN:                         return "Pattern";
                        case EXTENSION_ROTATE:                          return "Rotation (Tiling)";
                        case EXTENSION_CONVOLUTION_SIMPLE:              return "Convolution (1D, symmetrical, normalised)";
                        case EXTENSION_SCALE:                           return "Scaling";
                        case EXTENSION_ROTATE_VANILLA:                  return "Rotation (Non-tiling)";
                        case EXTENSION_NOISE_DEFORM:                    return "Noisy Deformation";
                        default:                                        return "Unnamed Extension";
                    }
                }
                default:                                                return "Unknown Instruction";
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
            int m_iParam1;
            u_char m_aucParam1[ 4 ];
            char m_acParam1[ 4 ];
            u_short m_ausParam1[ 2 ];
        }
        ;
        union
        {
            u_int m_uParam2;
            float m_fParam2;
            int m_iParam2;
            u_char m_aucParam2[ 4 ];
            char m_acParam2[ 4 ];
            u_short m_ausParam2[ 2 ];
        };

        union
        {
            u_int m_uParam3;
            float m_fParam3;
            int m_iParam3;
            u_char m_aucParam3[ 4 ];
            char m_acParam3[ 4 ];
            u_short m_ausParam3[ 2 ];
        };

        u_int m_uID;
        GLToy_Texture_Procedural* m_pxParentTexture;

        static u_int s_uNextID;

        static GLToy_Stack< GLToy_Vector_4* > s_xRenderStack;
        static bool s_bWrap;
        static GLToy_Vector_3 s_xLight;

    };

public:

    GLToy_Texture_Procedural()
    : m_xLayers()
    {
    }

    virtual ~GLToy_Texture_Procedural() {}

    u_int AppendGroup()
    {
        m_xLayers.Append( LayerNode::CreateGroup( this ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendFillLayer( const u_int uRGBA = 0 )
    {
        m_xLayers.Append( LayerNode::CreateFill( this, uRGBA ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendNoiseLayer( const float fFrequency = 32.0f, const u_int uSeed = 0 )
    {
        m_xLayers.Append( LayerNode::CreateNoise( this, fFrequency, uSeed ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendTileLayer( const u_int uTileFrequency )
    {
        m_xLayers.Append( LayerNode::CreateTile( this, uTileFrequency ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendCircleLayer( const GLToy_Vector_2& xCentre, const float fRadius, const u_int uRGBA )
    {
        m_xLayers.Append( LayerNode::CreateCircle( this, xCentre, fRadius, uRGBA ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendFBMNoiseLayer( const float fFrequency = 32.0f, const u_int uSeed = 0 )
    {
        m_xLayers.Append( LayerNode::CreateFBMNoise( this, fFrequency, uSeed ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendShapingLayer( const ShapeFunction eShapeFunction )
    {
        m_xLayers.Append( LayerNode::CreateShaping( this, eShapeFunction ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendGradientLayer( const GradientStyle eGradientStyle )
    {
        m_xLayers.Append( LayerNode::CreateGradient( this, eGradientStyle ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendCheckerboardLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_CHECKERBOARD ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendHorizontalStripeLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_HORIZONTAL_STRIPE ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendVerticalStripeLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_VERTICAL_STRIPE ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendDiagonalStripeUpLeftLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_DIAGONAL_STRIPE_UPLEFT ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendDiagonalStripeDownLeftLayer()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_DIAGONAL_STRIPE_DOWNLEFT ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendBorder( const float fAmount )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_BORDER, static_cast< u_int >( fAmount * 4095.0f ) ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendBevel( const float fAmount )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_BEVEL, static_cast< u_int >( fAmount * 4095.0f ) ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendBevelNormals( const float fAmount )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_BEVEL_NORMALS, static_cast< u_int >( fAmount * 4095.0f ) ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendTextureClamp()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_TEXTURE_MODE, 0u ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendTextureWrap()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_TEXTURE_MODE, 1u ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendHeightmapToHighlights()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_HEIGHTMAP_HIGHLIGHT ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendHeightmapToNormals()
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_HEIGHTMAP_NORMALS ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendPatternLayer( const PatternStyle eStyle )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_PATTERN, static_cast< u_int >( eStyle ) ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendRotation( const float fAngle )
    {
        // 0..1 for whole circle, but forget the exact 1 because its the same as zero (hence 64, not 63)
        const u_int uAngle = static_cast< u_int >( 64.0f * fAngle / 360.0f ) & 0x3F; // bithax: mod 64
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_ROTATE, uAngle ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendSimpleConvolution( const char cCentre, const char c1 = 0, const char c2 = 0, const char c3 = 0, const char c4 = 0 )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_CONVOLUTION_SIMPLE ) );
        // normally we don't do stuff like this here... but this seems best in this case
        LayerNode& xLayerNode = m_xLayers.End();
        xLayerNode.m_uParam1 = 3;
        if( c4 == 0 )
        {
            --xLayerNode.m_uParam1;
            if( c3 == 0 ) 
            {
                --xLayerNode.m_uParam1;
                if( c2 == 0 ) 
                {
                    --xLayerNode.m_uParam1;
                }
            }
        }

        xLayerNode.m_iParam3 = cCentre;
        xLayerNode.m_acParam2[ 0 ] = c1;
        xLayerNode.m_acParam2[ 1 ] = c2;
        xLayerNode.m_acParam2[ 2 ] = c3;
        xLayerNode.m_acParam2[ 3 ] = c4;

        return xLayerNode.GetID();
    }

    u_int AppendScale( const float fScaleFactor )
    {
        const u_int uScale = static_cast< u_int >( 64.0f * fScaleFactor ) & 0xFFF; // bithax: mod 4096
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_SCALE, uScale ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendUntiledRotation( const float fAngle )
    {
        // 0..1 for whole circle, but forget the exact 1 because its the same as zero (hence 64, not 63)
        const u_int uAngle = static_cast< u_int >( 64.0f * fAngle / 360.0f ) & 0x3F; // bithax: mod 64
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_ROTATE_VANILLA, uAngle ) );
        return m_xLayers.End().GetID();
    }

    u_int AppendNoiseDeform( const float fFrequency = 32.0f, const u_int uSeed = 0 )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_NOISE_DEFORM, *reinterpret_cast< const u_int* >( &fFrequency ), uSeed ) );
        LayerNode& xLayerNode = m_xLayers.End();
        xLayerNode.m_uParam3 = 4;
        return xLayerNode.GetID();
    }

    u_int AppendReference( const u_int uReferToID = 0 )
    {
        m_xLayers.Append( LayerNode::CreateExtension( this, EXTENSION_REFERENCE, GetPositionFromID( uReferToID ) ) );
        return m_xLayers.End().GetID();
    }

    u_int MoveLayerAfter( const u_int uID, const u_int uAfterID );
    u_int MoveLayerBefore( const u_int uID, const u_int uBeforeID );
    u_int MoveLayerUnder( const u_int uID, const u_int uUnderID );
    u_int MoveLayerToOwnGroup( const u_int uID );

    u_int* CreateRGBA( const u_int uWidth, const u_int uHeight );
    u_int* CreateRGBA_4xSS( const u_int uWidth, const u_int uHeight );
    u_int* CreateRGBA_16xSS( const u_int uWidth, const u_int uHeight );
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

    // macros are evil, but significantly less evil than 1000s lines of header
#define Getter( xName, xType, xMember ) GLToy_Inline xType xName( const u_int uID ) const { const LayerNode* const pxLayerNode = GetLayerNodeFromID( uID ); if( pxLayerNode ) { return pxLayerNode->xMember; } return static_cast< xType >( 0 ); }

    Getter( GetParam1, u_int, m_uParam1 )
    Getter( GetParam2, u_int, m_uParam2 )
    Getter( GetParam3, u_int, m_uParam3 )
    Getter( GetParam1i, int, m_iParam1 )
    Getter( GetParam2i, int, m_iParam2 )
    Getter( GetParam3i, int, m_iParam3 )
    Getter( GetParam1f, float, m_fParam1 )
    Getter( GetParam2f, float, m_fParam2 )
    Getter( GetParam3f, float, m_fParam3 )
    Getter( GetParam1uc0, u_char, m_aucParam1[ 0 ] )
    Getter( GetParam1uc1, u_char, m_aucParam1[ 1 ] )
    Getter( GetParam1uc2, u_char, m_aucParam1[ 2 ] )
    Getter( GetParam1uc3, u_char, m_aucParam1[ 3 ] )
    Getter( GetParam2uc0, u_char, m_aucParam2[ 0 ] )
    Getter( GetParam2uc1, u_char, m_aucParam2[ 1 ] )
    Getter( GetParam2uc2, u_char, m_aucParam2[ 2 ] )
    Getter( GetParam2uc3, u_char, m_aucParam2[ 3 ] )
    Getter( GetParam3uc0, u_char, m_aucParam3[ 0 ] )
    Getter( GetParam3uc1, u_char, m_aucParam3[ 1 ] )
    Getter( GetParam3uc2, u_char, m_aucParam3[ 2 ] )
    Getter( GetParam3uc3, u_char, m_aucParam3[ 3 ] )
    Getter( GetParam1c0, char, m_acParam1[ 0 ] )
    Getter( GetParam1c1, char, m_acParam1[ 1 ] )
    Getter( GetParam1c2, char, m_acParam1[ 2 ] )
    Getter( GetParam1c3, char, m_acParam1[ 3 ] )
    Getter( GetParam2c0, char, m_acParam2[ 0 ] )
    Getter( GetParam2c1, char, m_acParam2[ 1 ] )
    Getter( GetParam2c2, char, m_acParam2[ 2 ] )
    Getter( GetParam2c3, char, m_acParam2[ 3 ] )
    Getter( GetParam3c0, char, m_acParam3[ 0 ] )
    Getter( GetParam3c1, char, m_acParam3[ 1 ] )
    Getter( GetParam3c2, char, m_acParam3[ 2 ] )
    Getter( GetParam3c3, char, m_acParam3[ 3 ] )

#undef Getter

#define Setter( xName, xType, xMember ) GLToy_Inline void xName( const u_int uID, const xType xValue ) { LayerNode* const pxLayerNode = GetLayerNodeFromID( uID ); if( pxLayerNode ) { pxLayerNode->xMember = xValue; } }
    
    Setter( SetParam1, u_int, m_uParam1 )
    Setter( SetParam2, u_int, m_uParam2 )
    Setter( SetParam3, u_int, m_uParam3 )
    Setter( SetParam1, int, m_iParam1 )
    Setter( SetParam2, int, m_iParam2 )
    Setter( SetParam3, int, m_iParam3 )
    Setter( SetParam1, float, m_fParam1 )
    Setter( SetParam2, float, m_fParam2 )
    Setter( SetParam3, float, m_fParam3 )
    Setter( SetParam1uc0, u_char, m_aucParam1[ 0 ] )
    Setter( SetParam1uc1, u_char, m_aucParam1[ 1 ] )
    Setter( SetParam1uc2, u_char, m_aucParam1[ 2 ] )
    Setter( SetParam1uc3, u_char, m_aucParam1[ 3 ] )
    Setter( SetParam2uc0, u_char, m_aucParam2[ 0 ] )
    Setter( SetParam2uc1, u_char, m_aucParam2[ 1 ] )
    Setter( SetParam2uc2, u_char, m_aucParam2[ 2 ] )
    Setter( SetParam2uc3, u_char, m_aucParam2[ 3 ] )
    Setter( SetParam3uc0, u_char, m_aucParam3[ 0 ] )
    Setter( SetParam3uc1, u_char, m_aucParam3[ 1 ] )
    Setter( SetParam3uc2, u_char, m_aucParam3[ 2 ] )
    Setter( SetParam3uc3, u_char, m_aucParam3[ 3 ] )
    Setter( SetParam1c0, char, m_acParam1[ 0 ] )
    Setter( SetParam1c1, char, m_acParam1[ 1 ] )
    Setter( SetParam1c2, char, m_acParam1[ 2 ] )
    Setter( SetParam1c3, char, m_acParam1[ 3 ] )
    Setter( SetParam2c0, char, m_acParam2[ 0 ] )
    Setter( SetParam2c1, char, m_acParam2[ 1 ] )
    Setter( SetParam2c2, char, m_acParam2[ 2 ] )
    Setter( SetParam2c3, char, m_acParam2[ 3 ] )
    Setter( SetParam3c0, char, m_acParam3[ 0 ] )
    Setter( SetParam3c1, char, m_acParam3[ 1 ] )
    Setter( SetParam3c2, char, m_acParam3[ 2 ] )
    Setter( SetParam3c3, char, m_acParam3[ 3 ] )

#undef Setter

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    void ReadNoHeader( const char* const pcData, const u_int uLength );
    void SaveToCPPHeader( const GLToy_String& szName, const GLToy_String* pszFilename = NULL );
    void SaveToTGAFile( const GLToy_String& szFilename, const u_int uWidth = 256, const u_int uHeight = 256, const u_int uSamples = 1 );

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

    const char* GetGradientNameFromID( const u_int uID ) const
    {
        const LayerNode* pxLayerNode = GetLayerNodeFromID( uID );
        if( !pxLayerNode )
        {
            return NULL;
        }

        return GetGradientName( static_cast< GradientStyle >( pxLayerNode->m_uParam1 ) );
    }

    ExtensionFunction GetExtension( const u_int uID ) const { const LayerNode* const pxLayerNode = GetLayerNodeFromID( uID ); return pxLayerNode ? pxLayerNode->m_eExtensionFunction : EXTENSION_BAD; }
    void DeleteFromID( const u_int uID ) { DeleteLayerNodeFromID( uID ); }
    bool IsLeaf( const u_int uID ) const { const LayerNode* const pxLayerNode = GetLayerNodeFromID( uID ); return pxLayerNode ? pxLayerNode->IsLeaf() : false; }

    static const char* GetShapingFunctionName( const ShapeFunction eFunction );
    static const char* GetGradientName( const GradientStyle eStyle );
    static const char* GetPatternName( const PatternStyle eStyle );

    u_int GetPositionFromID( const u_int uID ) const;
    u_int GetIDFromPosition( const u_int uPosition ) const;
    u_int GetLastPosition() const;
    void UpdateReferencesFromDelete( const u_int uDeletedPosition, const u_int uChildCount = 0 );
    void UpdateReferencesFromInsert( const u_int uInsertedPosition, const u_int uChildCount = 0 );
    void UpdateReferencesFromMove( const u_int uOldPosition, const u_int uNewPosition, const u_int uChildCount = 0 );
    bool CircularReferenceCheck( const u_int uReferenceID ) const;

protected:

    // TODO: tidy these nasties up
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
                if( xLayerNode.IsLeaf() )
                {
                    UpdateReferencesFromDelete( GetPositionFromID( xLayerNode.GetID() ) );
                    pxLayers->RemoveAt( xIterator.Index() );
                }
                else
                {
                    UpdateReferencesFromDelete( GetPositionFromID( xLayerNode.GetID() ), GetTotalChildCount( xLayerNode.GetID() ) );
                    pxLayers->RemoveAt( xIterator.Index() );
                }
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

    u_int GetParentIDFromID( const u_int uID, const GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL, u_int uParentID = 0 ) const
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
                return uParentID;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                u_int uReturnID = GetParentIDFromID( uID, xLayerNode.GetChildren(), xLayerNode.GetID() );
                if( uReturnID != 0 )
                {
                    return uReturnID;
                }
            }
        }

        return 0;
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

    const GLToy_SmallSerialisableArray< LayerNode >* GetParentArrayFromID( const u_int uID, const GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL ) const
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
                return pxLayers;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                const LayerNode* pxLayerNode = GetLayerNodeFromID( uID, xLayerNode.GetChildren() );
                if( pxLayerNode )
                {
                    return xLayerNode.GetChildren();
                }
            }
        }

        return NULL;
    }

    u_int GetIndexInArrayFromID( const u_int uID, const GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL ) const
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

    LayerNode* GetFirstReferenceTo( const u_int uPosition, GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL )
    {
        if( !pxLayers )
        {
            pxLayers = &m_xLayers;
        }

        GLToy_Iterate( LayerNode, xIterator, pxLayers )
        {
            LayerNode& xLayerNode = xIterator.Current();
            if( ( xLayerNode.m_eInstruction == INSTRUCTION_EXTENSION ) 
                && ( xLayerNode.m_eExtensionFunction == EXTENSION_REFERENCE )
                && ( xLayerNode.m_uParam1 == uPosition ) )
            {
                return &xLayerNode;
            }
            else if( !xLayerNode.IsLeaf() )
            {
                LayerNode* pxLayerNode = GetFirstReferenceTo( uPosition, xLayerNode.GetChildren() );
                if( pxLayerNode )
                {
                    return pxLayerNode;
                }
            }
        }

        return NULL;
    }

    u_int GetTotalChildCount( const u_int uID, const GLToy_SmallSerialisableArray< LayerNode >* pxLayers = NULL ) const
    {
        if( !pxLayers )
        {
            pxLayers = &m_xLayers;
        }

        u_int uCount = 0;
        GLToy_ConstIterate( LayerNode, xIterator, pxLayers )
        {
            const LayerNode& xLayerNode = xIterator.Current();
            if( xLayerNode.GetID() == uID )
            {
                if( xLayerNode.IsLeaf() )
                {
                    return 0;
                }
                else
                {
                    return GetTotalChildCount( uID, xLayerNode.GetChildren() );
                }
            }
            else if( xLayerNode.IsLeaf() )
            {
                ++uCount;
            }
            else
            {
                uCount += 1 + GetTotalChildCount( uID, xLayerNode.GetChildren() );
            }
        }

        return uCount;
    }

    GLToy_SmallSerialisableArray< LayerNode > m_xLayers;

};

#endif