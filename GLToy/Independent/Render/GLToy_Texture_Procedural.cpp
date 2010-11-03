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

// TODO: sort out this file - its huge and repeats absolutely tons of code

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Texture_Procedural.h>

// GLToy
#include <File/GLToy_File.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Noise.h>
#include <Render/GLToy_Texture.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

// these values are pre-normalised...
static const u_int uNORMAL_UP = 0xFFDADA00;
static const u_int uNORMAL_DOWN = 0xFFDA2500;
static const u_int uNORMAL_LEFT = 0xFFDA00DA;
static const u_int uNORMAL_RIGHT = 0xFFDA0025;
static const u_int uNORMAL_FLAT = 0xFFFF7F7F;

// this looks anti-convention, but actually its just in tangent space...
static const GLToy_Vector_3 xNORMAL_UP = GLToy_Vector_3( 0.0f, 0.707f, 0.707f );
static const GLToy_Vector_3 xNORMAL_DOWN = GLToy_Vector_3( 0.0f, -0.707f, 0.707f );
static const GLToy_Vector_3 xNORMAL_LEFT = GLToy_Vector_3( 0.707f, 0.0f, 0.707f );
static const GLToy_Vector_3 xNORMAL_RIGHT = GLToy_Vector_3( -0.707f, 0.0f, 0.707f );
static const GLToy_Vector_3 xNORMAL_FLAT = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Stack< GLToy_Vector_4* > GLToy_Texture_Procedural::LayerNode::s_xRenderStack;
u_int GLToy_Texture_Procedural::LayerNode::s_uNextID = 1;
bool GLToy_Texture_Procedural::LayerNode::s_bWrap = true;
GLToy_Vector_3 GLToy_Texture_Procedural::LayerNode::s_xLight( 0.533f, 0.533f, 0.533f );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture_Procedural::LayerNode::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    // this seems sensible...
    m_uParam1 = 0;
    m_uParam2 = 0;
    m_uParam3 = 0;

    // 4-bits for blend mode and if we have children
    u_int uBlendMode;
    xStream.ReadBits( uBlendMode, 3 );
    m_eBlendMode = static_cast< BlendMode >( uBlendMode );

    delete m_pxChildren;

    bool bLeaf;
    xStream >> bLeaf;

    if( !bLeaf )
    {
        m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >();
        xStream >> m_pxChildren; // recurse...
    }
    else
    {
        // in general each instruction is 3 + n*32 bits, where n is 0-3, except extensions which use an extra 8-bits
        // some are optimised though
        u_int uInstruction;
        xStream.ReadBits( uInstruction, 3 );
        m_pxChildren = NULL;
        m_eInstruction = static_cast< Instruction >( uInstruction );

        m_uParam1 = 0;

        switch( m_eInstruction ) // note the deliberate fall-through
        {
            case INSTRUCTION_CIRCLE:
                xStream >> m_uParam3;
            
            case INSTRUCTION_NOISE:
            case INSTRUCTION_FBMNOISE:
                xStream >> m_uParam2;
            
            default:
                xStream >> m_uParam1; // TODO: split out noise and compress uParam1 as a 24-bit float - or better a 16-bit fixed point.
                break;

            case INSTRUCTION_TILE:
            {
                // only let there be only up to 31x31 tiles - 5-bits
                xStream.ReadBits( m_uParam1, 5 );
                break;
            }

            case INSTRUCTION_SHAPE:
            {
                // TODO: this can almost certainly afford to lose some bits
                // only let there be a maximum of 64 shaping functions - 6-bits
                xStream.ReadBits( m_uParam1, 6 );
                break;
            }

            case INSTRUCTION_GRADIENT:
            {
                // only let there be only up to 16 types of gradient - 4-bits
                xStream.ReadBits( m_uParam1, 4 );
                break;
            }

            case INSTRUCTION_EXTENSION:
            {
                u_char ucFunction;
                xStream >> ucFunction;
                m_eExtensionFunction = static_cast< ExtensionFunction >( ucFunction );
                
                switch( m_eExtensionFunction )
                {
                    case EXTENSION_REFERENCE:
                    {
                        xStream.ReadBits( m_uParam1, 7 ); // only allow up to 128 layers...
                        break;
                    }

                    case EXTENSION_BORDER:
                    case EXTENSION_BEVEL:
                    case EXTENSION_BEVEL_NORMALS:
                    case EXTENSION_SCALE: // reuse this size for scaling - this gives us from 1/64x up to 63x
                    {
                        xStream.ReadBits( m_uParam1, 12 ); // 4096 graduations is probably ample forever (assuming textures never get bigger than 4kx4k)
                        break;
                    }

                    case EXTENSION_TEXTURE_MODE:
                    {
                        xStream.ReadBits( m_uParam1, 1 );
                        break;
                    }

                    case EXTENSION_PATTERN: // 64 patterns should be enough
                    case EXTENSION_ROTATE: // 64 rotations should be enough
                    case EXTENSION_ROTATE_VANILLA:
                    {
                        xStream.ReadBits( m_uParam1, 6 );
                        break;
                    }

                    case EXTENSION_CONVOLUTION_SIMPLE:
                    {
                        bool bSign = false;
                        xStream.ReadBits( m_uParam1, 2 ); // 0-3 represent 3x3, 5x5, 7x7, 9x9
                        xStream.ReadBool( bSign );
                        xStream.ReadBits( m_uParam3, 4 ); // the centre value, I reckon 4 bits should be enough for each component...
                        if( bSign )
                        {
                            m_uParam3 |= 0xFFFFFFF0;
                        }
                        // ...then the other values
                        for( u_int u = 0; u < ( m_uParam1 + 1 ); ++u )
                        {
                            u_int uParam = 0;
                            xStream.ReadBool( bSign );
                            xStream.ReadBits( uParam, 4 );
                            m_aucParam2[ u ] = static_cast< u_char >( uParam ) | ( bSign ? 0xF0 : 0x00 );
                        }
                    }

                    default:
                    {
                        break;
                    }
                }
                
                break;
            }
        };
    }
}

void GLToy_Texture_Procedural::LayerNode::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    // 4-bits for blend mode and if we have children
    xStream.WriteBits( m_eBlendMode, 3 );

    xStream << ( m_pxChildren == NULL );
    if( m_pxChildren )
    {
        xStream << m_pxChildren;
    }
    else
    {
        // in general each instruction is 3 + n*32 bits, where n is 0-3, except extensions which use an extra 8-bits
        // some are optimised though
        xStream.WriteBits( m_eInstruction, 3 );

        switch( m_eInstruction ) // note the deliberate fall-through
        {
            case INSTRUCTION_CIRCLE:
                xStream << m_uParam3;

            case INSTRUCTION_NOISE:
            case INSTRUCTION_FBMNOISE:
                xStream << m_uParam2;

            default:
                xStream << m_uParam1; // TODO: split out noise and compress uParam1 as a 24-bit float - or better a 16-bit fixed point.
                break;

            case INSTRUCTION_TILE:
            {
                // only let there be only up to 31x31 tiles - 5-bits
                xStream.WriteBits( m_uParam1, 5 );
                break;
            }
            
            case INSTRUCTION_SHAPE:
            {
                // only let there be a maximum of 64 shaping functions - 6-bits
                xStream.WriteBits( m_uParam1, 6 );
                break;
            }

            case INSTRUCTION_GRADIENT:
            {
                // only let there be only up to 16 types of gradient - 4-bits
                xStream.WriteBits( m_uParam1, 4 );
                break;
            }

            case INSTRUCTION_EXTENSION:
            {
                xStream << static_cast< u_char >( m_eExtensionFunction );

                switch( m_eExtensionFunction )
                {
                    case EXTENSION_REFERENCE:
                    {
                        xStream.WriteBits( m_uParam1, 7 ); // only allow up to 128 layers...
                        break;
                    }

                    case EXTENSION_BORDER:
                    case EXTENSION_BEVEL:
                    case EXTENSION_BEVEL_NORMALS:
                    case EXTENSION_SCALE: // reuse this size for scaling - this gives us from 1/64x up to 64x
                    {
                        xStream.WriteBits( m_uParam1, 12 ); // 4096 graduations is probably ample forever (assuming textures never get bigger than 4kx4k)
                        break;
                    }

                    case EXTENSION_TEXTURE_MODE:
                    {
                        xStream.WriteBits( m_uParam1, 1 );
                        break;
                    }
                                        
                    case EXTENSION_PATTERN: // 64 patterns should be enough
                    case EXTENSION_ROTATE: // 64 rotations should be enough
                    case EXTENSION_ROTATE_VANILLA:
                    {
                        xStream.WriteBits( m_uParam1, 6 );
                        break;
                    }

                    case EXTENSION_CONVOLUTION_SIMPLE:
                    {
                        // make sure the values are correct.
                        u_int uParam1 = 0;
                        for( u_int u = 1; u < 4; ++u )
                        {
                            if( m_aucParam2[ u ] != 0 )
                            {
                                ++uParam1;
                            }
                        }
                        xStream.WriteBits( uParam1, 2 ); // 0-3 represent 3x3, 5x5, 7x7, 9x9
                        xStream.WriteBool( m_iParam3 < 0 );
                        xStream.WriteBits( m_uParam3, 4 ); // the centre value, I reckon 4 bits should be enough for each component...
                        // ...then the other values
                        for( u_int u = 0; u < ( m_uParam1 + 1 ); ++u )
                        {
                            const u_int uParam = m_aucParam2[ u ];
                            xStream.WriteBool( m_acParam2[ u ] < 0 );
                            xStream.WriteBits( uParam, 4 );
                        }
                    }

                    default:
                        break;
                }
                
                break;
            }
        };
    }
}

void GLToy_Texture_Procedural::LayerNode::Render( const u_int uWidth, const u_int uHeight )
{
    GLToy_Vector_4* pxData = new GLToy_Vector_4[ uWidth * uHeight ];

    GLToy_Memory::SetDWords( pxData, 4 * uWidth * uHeight, 0 );

    s_xRenderStack.Push( pxData );

    if( m_pxChildren )
    {
        // traverse the tree
        GLToy_Iterate( LayerNode, xIterator, m_pxChildren )
        {
            xIterator.Current().Render( uWidth, uHeight );
        }
    }
    else
    {
        switch( m_eInstruction )
        {
            default:
            case INSTRUCTION_FILL:
            {                
                for( u_int u = 0; u < uWidth * uHeight; ++u )
                {
                    pxData[ u ] = GLToy_Vector_4( m_uParam1 );
                }

                break;
            }

            case INSTRUCTION_NOISE:
            {
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        const float fX = static_cast< float >( u ) / static_cast< float >( uWidth );
                        const float fY = static_cast< float >( v ) / static_cast< float >( uHeight );
                        const float fNoise = 0.5f + 0.5f * GLToy_Noise::Cubic2D( fX, fY, m_fParam1, 1.0f, static_cast< u_int >( m_fParam1 ), m_uParam2 );
                        pxData[ v * uWidth + u ] = GLToy_Vector_4( fNoise, fNoise, fNoise, 1.0f );
                    }
                }
                break;
            }

            case INSTRUCTION_TILE:
            {
                // TODO: some filtering
                // maybe constuct some mipmaps?
                const bool bOldWrap = s_bWrap;
                s_bWrap = true; // force it for this...
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        const float fX = static_cast< float >( u * m_uParam1 ) / static_cast< float >( uWidth );
                        const float fY = static_cast< float >( v * m_uParam1 ) / static_cast< float >( uHeight );
                        pxData[ v * uWidth + u ] = WrapAwareSampleFiltered( fX, fY, uWidth, uHeight, s_xRenderStack.Peek( 1 ) );
                    }
                }
                s_bWrap = bOldWrap;
                break;
            }

            case INSTRUCTION_CIRCLE:
            {
                // TODO: some filtering
                const float fCentreX = static_cast< float >( m_ausParam1[ 1 ] ) / 65535.0f;
                const float fCentreY = static_cast< float >( m_ausParam1[ 0 ] ) / 65535.0f;
                const float fRadius = m_fParam2;
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        const float fX = fCentreX - static_cast< float >( u ) / static_cast< float >( uWidth );
                        const float fY = fCentreY - static_cast< float >( v ) / static_cast< float >( uHeight );
                        if( fX * fX + fY * fY < fRadius * fRadius )
                        {
                            pxData[ v * uWidth + u ] = GLToy_Vector_4( m_uParam3 );
                        }
                    }
                }
                break;
            }

            case INSTRUCTION_FBMNOISE:
            {
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        const float fX = static_cast< float >( u ) / static_cast< float >( uWidth );
                        const float fY = static_cast< float >( v ) / static_cast< float >( uHeight );
                        const float fNoise = 0.5f + 0.5f * GLToy_Noise::FractalCubic2D( fX, fY, m_fParam1, 1.0f, 0.5f, 10, true, m_uParam2 );
                        pxData[ v * uWidth + u ] = GLToy_Vector_4( fNoise, fNoise, fNoise, 1.0f );
                    }
                }
                break;
            }

            case INSTRUCTION_SHAPE:
            {
                for( u_int u = 0; u < uWidth * uHeight; ++u )
                {
                    GLToy_Vector_4& xDest = pxData[ u ];
                    GLToy_Vector_4& xSource = s_xRenderStack.Peek( 1 )[ u ];
                    
                    switch( static_cast< ShapeFunction >( m_uParam1 ) )
                    {
                        case SHAPE_COS_2PI:
                        {
                            xDest[ 0 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 0 ] ) + 0.5f;
                            xDest[ 1 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 1 ] ) + 0.5f;
                            xDest[ 2 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 2 ] ) + 0.5f;
                            xDest[ 3 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 3 ] ) + 0.5f;
                            break;
                        }

                        case SHAPE_SIN_2PI:
                        {
                            xDest[ 0 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 0 ] ) + 0.5f;
                            xDest[ 1 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 1 ] ) + 0.5f;
                            xDest[ 2 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 2 ] ) + 0.5f;
                            xDest[ 3 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 3 ] ) + 0.5f;
                            break;
                        }

                        case SHAPE_COS_4PI:
                        {
                            xDest[ 0 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 0 ] ) + 0.5f;
                            xDest[ 1 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 1 ] ) + 0.5f;
                            xDest[ 2 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 2 ] ) + 0.5f;
                            xDest[ 3 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 3 ] ) + 0.5f;
                            break;
                        }

                        case SHAPE_SIN_4PI:
                        {
                            xDest[ 0 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 0 ] ) + 0.5f;
                            xDest[ 1 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 1 ] ) + 0.5f;
                            xDest[ 2 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 2 ] ) + 0.5f;
                            xDest[ 3 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 3 ] ) + 0.5f;
                            break;
                        }

                        case SHAPE_COS_6PI:
                        {
                            xDest[ 0 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 0 ] ) + 0.5f;
                            xDest[ 1 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 1 ] ) + 0.5f;
                            xDest[ 2 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 2 ] ) + 0.5f;
                            xDest[ 3 ] = 0.5f * GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 3 ] ) + 0.5f;
                            break;
                        }

                        case SHAPE_SIN_6PI:
                        {
                            xDest[ 0 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 0 ] ) + 0.5f;
                            xDest[ 1 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 1 ] ) + 0.5f;
                            xDest[ 2 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 2 ] ) + 0.5f;
                            xDest[ 3 ] = 0.5f * GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 3 ] ) + 0.5f;
                            break;
                        }

                        case SHAPE_SQUARE:
                        {
                            xDest[ 0 ] = xSource[ 0 ] * xSource[ 0 ];
                            xDest[ 1 ] = xSource[ 1 ] * xSource[ 1 ];
                            xDest[ 2 ] = xSource[ 2 ] * xSource[ 2 ];
                            xDest[ 3 ] = xSource[ 3 ] * xSource[ 3 ];
                            break;
                        }

                        case SHAPE_SQUAREROOT:
                        {
                            xDest[ 0 ] = GLToy_Maths::Sqrt( xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Sqrt( xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Sqrt( xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Sqrt( xSource[ 3 ] );
                            break;
                        }

                        case SHAPE_ABS:
                        {
                            xDest[ 0 ] = GLToy_Maths::Abs( 2.0f * xSource[ 0 ] - 1.0f );
                            xDest[ 1 ] = GLToy_Maths::Abs( 2.0f * xSource[ 1 ] - 1.0f );
                            xDest[ 2 ] = GLToy_Maths::Abs( 2.0f * xSource[ 2 ] - 1.0f );
                            xDest[ 3 ] = GLToy_Maths::Abs( 2.0f * xSource[ 3 ] - 1.0f );
                            break;
                        }

                        case SHAPE_HALF:
                        {
                            xDest = xSource * 0.5f;
                            break;
                        }

                        case SHAPE_DOUBLE:
                        {
                            xDest = xSource * 2.0f;
                            break;
                        }

                        case SHAPE_CLIPBELOWHALF:
                        {
                            xDest[ 0 ] = xSource[ 0 ] < 0.5f ? 0.0f : xSource[ 0 ];
                            xDest[ 1 ] = xSource[ 1 ] < 0.5f ? 0.0f : xSource[ 1 ];
                            xDest[ 2 ] = xSource[ 2 ] < 0.5f ? 0.0f : xSource[ 2 ];
                            xDest[ 3 ] = xSource[ 3 ] < 0.5f ? 0.0f : xSource[ 3 ];
                            break;
                        }

                        case SHAPE_CLIPABOVEHALF:
                        {
                            xDest[ 0 ] = xSource[ 0 ] > 0.5f ? 0.0f : xSource[ 0 ];
                            xDest[ 1 ] = xSource[ 1 ] > 0.5f ? 0.0f : xSource[ 1 ];
                            xDest[ 2 ] = xSource[ 2 ] > 0.5f ? 0.0f : xSource[ 2 ];
                            xDest[ 3 ] = xSource[ 3 ] > 0.5f ? 0.0f : xSource[ 3 ];
                            break;
                        }

                        case SHAPE_INVERT:
                        {
                            xDest[ 0 ] = 1.0f - xSource[ 0 ];
                            xDest[ 1 ] = 1.0f - xSource[ 1 ];
                            xDest[ 2 ] = 1.0f - xSource[ 2 ];
                            xDest[ 3 ] = 1.0f - xSource[ 3 ];
                            break;
                        }
                        
                        case SHAPE_MIN_50:
                        {
                            xDest[ 0 ] = GLToy_Maths::Min( xSource[ 0 ], 0.5f );
                            xDest[ 1 ] = GLToy_Maths::Min( xSource[ 1 ], 0.5f );
                            xDest[ 2 ] = GLToy_Maths::Min( xSource[ 2 ], 0.5f );
                            xDest[ 3 ] = GLToy_Maths::Min( xSource[ 3 ], 0.5f );
                            break;
                        }

                        case SHAPE_MAX_50:
                        {
                            xDest[ 0 ] = GLToy_Maths::Max( xSource[ 0 ], 0.5f );
                            xDest[ 1 ] = GLToy_Maths::Max( xSource[ 1 ], 0.5f );
                            xDest[ 2 ] = GLToy_Maths::Max( xSource[ 2 ], 0.5f );
                            xDest[ 3 ] = GLToy_Maths::Max( xSource[ 3 ], 0.5f );
                            break;
                        }

                        case SHAPE_MIN_25:
                        {
                            xDest[ 0 ] = GLToy_Maths::Min( xSource[ 0 ], 0.25f );
                            xDest[ 1 ] = GLToy_Maths::Min( xSource[ 1 ], 0.25f );
                            xDest[ 2 ] = GLToy_Maths::Min( xSource[ 2 ], 0.25f );
                            xDest[ 3 ] = GLToy_Maths::Min( xSource[ 3 ], 0.25f );
                            break;
                        }

                        case SHAPE_MAX_25:
                        {
                            xDest[ 0 ] = GLToy_Maths::Max( xSource[ 0 ], 0.25f );
                            xDest[ 1 ] = GLToy_Maths::Max( xSource[ 1 ], 0.25f );
                            xDest[ 2 ] = GLToy_Maths::Max( xSource[ 2 ], 0.25f );
                            xDest[ 3 ] = GLToy_Maths::Max( xSource[ 3 ], 0.25f );
                            break;
                        }

                        case SHAPE_MIN_75:
                        {
                            xDest[ 0 ] = GLToy_Maths::Min( xSource[ 0 ], 0.75f );
                            xDest[ 1 ] = GLToy_Maths::Min( xSource[ 1 ], 0.75f );
                            xDest[ 2 ] = GLToy_Maths::Min( xSource[ 2 ], 0.75f );
                            xDest[ 3 ] = GLToy_Maths::Min( xSource[ 3 ], 0.75f );
                            break;
                        }

                        case SHAPE_MAX_75:
                        {
                            xDest[ 0 ] = GLToy_Maths::Max( xSource[ 0 ], 0.75f );
                            xDest[ 1 ] = GLToy_Maths::Max( xSource[ 1 ], 0.75f );
                            xDest[ 2 ] = GLToy_Maths::Max( xSource[ 2 ], 0.75f );
                            xDest[ 3 ] = GLToy_Maths::Max( xSource[ 3 ], 0.75f );
                            break;
                        }

                        case SHAPE_MIN_10:
                        {
                            xDest[ 0 ] = GLToy_Maths::Min( xSource[ 0 ], 0.10f );
                            xDest[ 1 ] = GLToy_Maths::Min( xSource[ 1 ], 0.10f );
                            xDest[ 2 ] = GLToy_Maths::Min( xSource[ 2 ], 0.10f );
                            xDest[ 3 ] = GLToy_Maths::Min( xSource[ 3 ], 0.10f );
                            break;
                        }

                        case SHAPE_MAX_10:
                        {
                            xDest[ 0 ] = GLToy_Maths::Max( xSource[ 0 ], 0.10f );
                            xDest[ 1 ] = GLToy_Maths::Max( xSource[ 1 ], 0.10f );
                            xDest[ 2 ] = GLToy_Maths::Max( xSource[ 2 ], 0.10f );
                            xDest[ 3 ] = GLToy_Maths::Max( xSource[ 3 ], 0.10f );
                            break;
                        }

                        case SHAPE_MIN_90:
                        {
                            xDest[ 0 ] = GLToy_Maths::Min( xSource[ 0 ], 0.90f );
                            xDest[ 1 ] = GLToy_Maths::Min( xSource[ 1 ], 0.90f );
                            xDest[ 2 ] = GLToy_Maths::Min( xSource[ 2 ], 0.90f );
                            xDest[ 3 ] = GLToy_Maths::Min( xSource[ 3 ], 0.90f );
                            break;
                        }

                        case SHAPE_MAX_90:
                        {
                            xDest[ 0 ] = GLToy_Maths::Max( xSource[ 0 ], 0.90f );
                            xDest[ 1 ] = GLToy_Maths::Max( xSource[ 1 ], 0.90f );
                            xDest[ 2 ] = GLToy_Maths::Max( xSource[ 2 ], 0.90f );
                            xDest[ 3 ] = GLToy_Maths::Max( xSource[ 3 ], 0.90f );
                            break;
                        }

                        case SHAPE_INFINITE_MULTIPLIER:
                        {
                            xDest[ 0 ] = xSource[ 0 ] > 0.0f ? 1.0f : 0.0f;
                            xDest[ 1 ] = xSource[ 1 ] > 0.0f ? 1.0f : 0.0f;
                            xDest[ 2 ] = xSource[ 2 ] > 0.0f ? 1.0f : 0.0f;
                            xDest[ 3 ] = xSource[ 3 ] > 0.0f ? 1.0f : 0.0f;
                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    //pxData[ u ] = xDest.GetRGBA();
                }
                break;
            }

            case INSTRUCTION_GRADIENT:
            {
                GradientStyle eStyle = static_cast< GradientStyle >( m_uParam1 );
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        float fLuminance = 0.0f;
                        switch( eStyle )
                        {
                            case GRADIENT_TOP:
                            {
                                fLuminance = 1.0f - static_cast< float >( v ) / static_cast< float >( uHeight );
                                break;
                            }

                            case GRADIENT_BOTTOM:
                            {
                                fLuminance = static_cast< float >( v ) / static_cast< float >( uHeight );
                                break;
                            }

                            case GRADIENT_RIGHT:
                            {
                                fLuminance = static_cast< float >( u ) / static_cast< float >( uWidth );
                                break;
                            }

                            case GRADIENT_LEFT:
                            {
                                fLuminance = 1.0f - static_cast< float >( u ) / static_cast< float >( uWidth );
                                break;
                            }

                            case GRADIENT_TOP_LEFT:
                            {
                                fLuminance = 1.0f - static_cast< float >( v ) / static_cast< float >( uHeight );
                                fLuminance += 1.0f - static_cast< float >( u ) / static_cast< float >( uWidth );
                                fLuminance *= 0.5f;
                                break;
                            }

                            case GRADIENT_BOTTOM_LEFT:
                            {
                                fLuminance = static_cast< float >( v ) / static_cast< float >( uHeight );
                                fLuminance += 1.0f - static_cast< float >( u ) / static_cast< float >( uWidth );
                                fLuminance *= 0.5f;
                                break;
                            }

                            case GRADIENT_TOP_RIGHT:
                            {
                                fLuminance = 1.0f - static_cast< float >( v ) / static_cast< float >( uHeight );
                                fLuminance += static_cast< float >( u ) / static_cast< float >( uWidth );
                                fLuminance *= 0.5f;
                                break;
                            }

                            case GRADIENT_BOTTOM_RIGHT:
                            {
                                fLuminance = static_cast< float >( v ) / static_cast< float >( uHeight );
                                fLuminance += static_cast< float >( u ) / static_cast< float >( uWidth );
                                fLuminance *= 0.5f;
                                break;
                            }

                            case GRADIENT_RADIAL_OUT:
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                fLuminance = GLToy_Maths::Sqrt( fX * fX + fY * fY );
                                break;
                            }

                            case GRADIENT_RADIAL_IN:
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                fLuminance = 1.0f - GLToy_Maths::Sqrt( fX * fX + fY * fY );
                                break;
                            }

                            case GRADIENT_SQUARE_OUT:
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                fLuminance = GLToy_Maths::Max( GLToy_Maths::Abs( fX ), GLToy_Maths::Abs( fY ) );
                                break;
                            }

                            case GRADIENT_SQUARE_IN:
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                fLuminance = 1.0f - GLToy_Maths::Max( GLToy_Maths::Abs( fX ), GLToy_Maths::Abs( fY ) );
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }

                        pxData[ v * uWidth + u ] = GLToy_Vector_4( fLuminance, fLuminance, fLuminance, 1.0f );
                    }
                }
                break;
            }

            case INSTRUCTION_EXTENSION:
            {
                switch( m_eExtensionFunction )
                {
                    case EXTENSION_CHECKERBOARD:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                pxData[ v * uWidth + u ] = ( fX * fY >= 0.0f ) ? GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) : GLToy_Maths::ZeroVector4;
                            }
                        }
                        break;
                    }

                    case EXTENSION_HORIZONTAL_STRIPE:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                pxData[ v * uWidth + u ] = ( fY >= 0.0f ) ? GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) : GLToy_Maths::ZeroVector4;
                            }
                        }
                        break;
                    }

                    case EXTENSION_VERTICAL_STRIPE:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                pxData[ v * uWidth + u ] = ( fX >= 0.0f ) ? GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) : GLToy_Maths::ZeroVector4;
                            }
                        }
                        break;
                    }

                    case EXTENSION_DIAGONAL_STRIPE_UPLEFT:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                pxData[ v * uWidth + u ] =
                                    ( fX + fY >= 0.0f )
                                        ? ( fX + fY >= 1.0f ) ? GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) : GLToy_Maths::ZeroVector4
                                        : ( fX + fY < -1.0f ) ? GLToy_Maths::ZeroVector4 : GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f );
                            }
                        }
                        break;
                    }

                    case EXTENSION_DIAGONAL_STRIPE_DOWNLEFT:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u << 1 ) / static_cast< float >( uWidth ) - 1.0f;
                                const float fY = static_cast< float >( v << 1 ) / static_cast< float >( uHeight ) - 1.0f;
                                pxData[ v * uWidth + u ] =
                                    ( fX + fY < 0.0f )
                                        ? ( fX + fY >= -1.0f ) ? GLToy_Maths::ZeroVector4 : GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f )
                                        : ( fX + fY < 1.0f ) ? GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) : GLToy_Maths::ZeroVector4;
                            }
                        }
                        break;
                    }

                    case EXTENSION_REFERENCE:
                    {
                        if( m_pxParentTexture )
                        {
                            LayerNode* pxLayerNode = m_pxParentTexture->GetLayerNodeFromID( m_pxParentTexture->GetIDFromPosition( m_uParam1 ) );
                            if( pxLayerNode )
                            {
                                pxLayerNode->Render( uWidth, uHeight );
                            }
                        }
                        break;
                    }

                    case EXTENSION_BORDER:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u ) / static_cast< float >( uWidth );
                                const float fY = static_cast< float >( v ) / static_cast< float >( uHeight );
                                const float fMX = fX > 0.5f ? 1.0f - fX : fX;
                                const float fMY = fY > 0.5f ? 1.0f - fY : fY;
                                const float fAmount = static_cast< float >( m_uParam1 ) / 4095.0f;
                                pxData[ v * uWidth + u ] =
                                    ( ( GLToy_Maths::Abs( fMX ) < fAmount ) || ( GLToy_Maths::Abs( fMY ) < fAmount ) )
                                        ? GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f )
                                        : GLToy_Maths::ZeroVector4;
                            }
                        }
                        break;
                    }

                    case EXTENSION_BEVEL:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u ) / static_cast< float >( uWidth );
                                const float fY = static_cast< float >( v ) / static_cast< float >( uHeight );
                                const float fMX = fX > 0.5f ? 1.0f - fX : fX;
                                const float fMY = fY > 0.5f ? 1.0f - fY : fY;
                                const float fAmount = static_cast< float >( m_uParam1 ) / 4095.0f;
                                
                                GLToy_Vector_3 xSlope = xNORMAL_FLAT;
                                if( ( GLToy_Maths::Abs( fMX ) < fAmount ) || ( GLToy_Maths::Abs( fMY ) < fAmount ) )
                                {
                                    if( fX + fY > 1.0f )
                                    {
                                        if( fX > fY )
                                        {
                                            xSlope = xNORMAL_RIGHT;
                                        }
                                        else
                                        {
                                            xSlope = xNORMAL_DOWN;
                                        }
                                    }
                                    else
                                    {
                                        if( fX > fY )
                                        {
                                            xSlope = xNORMAL_UP;
                                        }
                                        else
                                        {
                                            xSlope = xNORMAL_LEFT;
                                        }
                                    }
                                }

                                const float fBrightness = xSlope * s_xLight;
                                pxData[ v * uWidth + u ] = GLToy_Vector_4( fBrightness, fBrightness, fBrightness, 1.0f );
                            }
                        }
                        break;
                    }
                    
                    case EXTENSION_BEVEL_NORMALS:
                    {
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                const float fX = static_cast< float >( u ) / static_cast< float >( uWidth );
                                const float fY = static_cast< float >( v ) / static_cast< float >( uHeight );
                                const float fMX = fX > 0.5f ? 1.0f - fX : fX;
                                const float fMY = fY > 0.5f ? 1.0f - fY : fY;
                                const float fAmount = static_cast< float >( m_uParam1 ) / 4095.0f;
                                
                                GLToy_Vector_3 xSlope = xNORMAL_FLAT;
                                if( ( GLToy_Maths::Abs( fMX ) < fAmount ) || ( GLToy_Maths::Abs( fMY ) < fAmount ) )
                                {
                                    if( fX + fY > 1.0f )
                                    {
                                        if( fX > fY )
                                        {
                                            xSlope = xNORMAL_RIGHT;
                                        }
                                        else
                                        {
                                            xSlope = xNORMAL_DOWN;
                                        }
                                    }
                                    else
                                    {
                                        if( fX > fY )
                                        {
                                            xSlope = xNORMAL_UP;
                                        }
                                        else
                                        {
                                            xSlope = xNORMAL_LEFT;
                                        }
                                    }
                                }

                                pxData[ v * uWidth + u ] = GLToy_Vector_4( xSlope * 0.5f + GLToy_Vector_3( 0.5f, 0.5f, 0.5f ), 1.0f );
                            }
                        }
                        break;
                    }

                    case EXTENSION_TEXTURE_MODE:
                    {
                        s_bWrap = ( m_uParam1 != 0 );
                        break;
                    }

                    case EXTENSION_HEIGHTMAP_HIGHLIGHT:
                    {
                        // take brightness as height - pretend alpha doesn't exist
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                // use central difference method - it prevents bias in theory
                                const float fBrightnessX1 = WrapAwareSample( u - 1, v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );
                                const float fBrightnessX2 = WrapAwareSample( u + 1, v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );
                                const float fBrightnessY1 = WrapAwareSample( u, v - 1, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );
                                const float fBrightnessY2 = WrapAwareSample( u, v + 1, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );

                                // this is important, otherwise the normals are shallower for larger textures (!)
                                const float fScaleX = 0.001953125f * static_cast< float >( uWidth );
                                const float fScaleY = 0.001953125f * static_cast< float >( uHeight );
                                GLToy_Vector_3 xDifferences( ( fBrightnessX2 - fBrightnessX1 ) * fScaleX, ( fBrightnessY2 - fBrightnessY1 ) * fScaleY, 1.0f );
                                xDifferences.Normalise();
                                const float fBrightness = xDifferences * s_xLight;
                                pxData[ v * uWidth + u ] = GLToy_Vector_4( fBrightness, fBrightness, fBrightness, 1.0f );
                            }
                        }
                        break;
                    }

                    case EXTENSION_HEIGHTMAP_NORMALS:
                    {
                        // take brightness as height - pretend alpha doesn't exist
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                // use central difference method - it prevents bias in theory
                                const float fBrightnessX1 = WrapAwareSample( u - 1, v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );
                                const float fBrightnessX2 = WrapAwareSample( u + 1, v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );
                                const float fBrightnessY1 = WrapAwareSample( u, v - 1, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );
                                const float fBrightnessY2 = WrapAwareSample( u, v + 1, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * GLToy_Vector_4( 0.333f, 0.333f, 0.333f, 0.0f );

                                // this is important, otherwise the normals are shallower for larger textures (!)
                                const float fScaleX = 0.001953125f * static_cast< float >( uWidth );
                                const float fScaleY = 0.001953125f * static_cast< float >( uHeight );
                                GLToy_Vector_3 xDifferences( ( fBrightnessX2 - fBrightnessX1 ) * fScaleX, ( fBrightnessY2 - fBrightnessY1 ) * fScaleY, 1.0f );
                                xDifferences.Normalise();
                                pxData[ v * uWidth + u ] = GLToy_Vector_4( xDifferences * 0.5f + GLToy_Vector_3( 0.5f, 0.5f, 0.5f ), 1.0f );
                            }
                        }
                        break;
                    }

                    case EXTENSION_ROTATE:
                    {
                        // note - this rotation should preserve tiling, so we need to sneak a scaling in there as well...
                        const float fAngle = static_cast< float >( m_uParam1 / 64.0f ) * 2.0f * GLToy_Maths::Pi;
                        // fsincos might be nice here... but a tiny optimisation at best
                        const float fCos = GLToy_Maths::Cos( fAngle );
                        const float fSin = GLToy_Maths::Sin( fAngle );
                        const float fScale = fCos + fSin;
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {           
                                const GLToy_Vector_2 xScaledPosition(
                                    fScale * static_cast< float >( u ) / static_cast< float >( uWidth ),
                                    fScale * static_cast< float >( v ) / static_cast< float >( uHeight )
                                );

                                const GLToy_Vector_2 xRotated( GLToy_Maths::Rotate_2D_FromCosSin( xScaledPosition, fCos, fSin ) );
                                pxData[ v * uWidth + u ] = WrapAwareSampleFiltered( xRotated[ 0 ], xRotated[ 1 ], uWidth, uHeight, s_xRenderStack.Peek( 1 ) );
                            }
                        }
                        break;
                    }

                    case EXTENSION_CONVOLUTION_SIMPLE:
                    {
                        // TODO: some validation/edit correction function to do this instead...
                        // make sure the values are correct.
                        u_int uParam1 = 1;
                        for( u_int u = 1; u < 4; ++u )
                        {
                            if( m_acParam2[ u ] != 0 )
                            {
                                ++uParam1;
                            }
                        }
                        // unpack data
                        const u_int& uNumCoefficients = uParam1;
                        const float afCoefficients[ 5 ] =
                        {
                            static_cast< float >( m_iParam3 ),
                            static_cast< float >( m_acParam2[ 0 ] ),
                            static_cast< float >( m_acParam2[ 1 ] ),
                            static_cast< float >( m_acParam2[ 2 ] ),
                            static_cast< float >( m_acParam2[ 3 ] )
                        };
                        const float fNormalisationFactor = 1.0f /
                            ( afCoefficients[ 0 ]
                            + afCoefficients[ 1 ] * 2.0f
                            + afCoefficients[ 2 ] * 2.0f
                            + afCoefficients[ 3 ] * 2.0f
                            + afCoefficients[ 4 ] * 2.0f );
                        GLToy_Vector_4* pxBufferData = new GLToy_Vector_4[ uWidth * uHeight ];
                        // first do x...
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                GLToy_Vector_4& xRunningTotal = pxBufferData[ v * uWidth + u ];
                                xRunningTotal = WrapAwareSample( u, v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * afCoefficients[ 0 ];
                                for( u_int w = 0; w < uNumCoefficients; ++w )
                                {
                                    xRunningTotal += WrapAwareSample( static_cast< int >( u ) + static_cast< int >( w + 1 ), v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * afCoefficients[ w + 1 ];
                                    xRunningTotal += WrapAwareSample( static_cast< int >( u ) - static_cast< int >( w + 1 ), v, uWidth, uHeight, s_xRenderStack.Peek( 1 ) ) * afCoefficients[ w + 1 ];
                                }
                                xRunningTotal *= fNormalisationFactor;
                            }
                        }

                        // ...then do y
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {
                                GLToy_Vector_4& xRunningTotal = pxData[ v * uWidth + u ];
                                xRunningTotal = WrapAwareSample( u, v, uWidth, uHeight, pxBufferData ) * afCoefficients[ 0 ];
                                for( u_int w = 0; w < uNumCoefficients; ++w )
                                {
                                    xRunningTotal += WrapAwareSample( u, static_cast< int >( v ) + static_cast< int >( w + 1 ), uWidth, uHeight, pxBufferData ) * afCoefficients[ w + 1 ];
                                    xRunningTotal += WrapAwareSample( u, static_cast< int >( v ) - static_cast< int >( w + 1 ), uWidth, uHeight, pxBufferData ) * afCoefficients[ w + 1 ];
                                }
                                xRunningTotal *= fNormalisationFactor;
                            }
                        }

                        delete[] pxBufferData;

                        break;
                    }

                    case EXTENSION_SCALE:
                    {
                        const float fScale = 64.0f / static_cast< float >( m_uParam1 ); // 1 / the value we store as the scale factor...
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {           
                                const GLToy_Vector_2 xScaledPosition(
                                    fScale * static_cast< float >( u ) / static_cast< float >( uWidth ),
                                    fScale * static_cast< float >( v ) / static_cast< float >( uHeight )
                                );

                                pxData[ v * uWidth + u ] = WrapAwareSampleFiltered( xScaledPosition[ 0 ], xScaledPosition[ 1 ], uWidth, uHeight, s_xRenderStack.Peek( 1 ) );
                            }
                        }
                        break;
                    }

                    case EXTENSION_ROTATE_VANILLA:
                    {
                        const float fAngle = static_cast< float >( m_uParam1 / 64.0f ) * 2.0f * GLToy_Maths::Pi;
                        // fsincos might be nice here... but a tiny optimisation at best
                        const float fCos = GLToy_Maths::Cos( fAngle );
                        const float fSin = GLToy_Maths::Sin( fAngle );
                        for( u_int v = 0; v < uHeight; ++v )
                        {
                            for( u_int u = 0; u < uWidth; ++u )
                            {           
                                const GLToy_Vector_2 xScaledPosition(
                                    static_cast< float >( u ) / static_cast< float >( uWidth ),
                                    static_cast< float >( v ) / static_cast< float >( uHeight )
                                );

                                const GLToy_Vector_2 xRotated( GLToy_Maths::Rotate_2D_FromCosSin( xScaledPosition, fCos, fSin ) );
                                pxData[ v * uWidth + u ] = WrapAwareSampleFiltered( xRotated[ 0 ], xRotated[ 1 ], uWidth, uHeight, s_xRenderStack.Peek( 1 ) );
                            }
                        }
                        break;
                    }

                    // because this is a horribly complicated case, do it last
                    case EXTENSION_PATTERN:
                    {
                        const PatternStyle eStyle = static_cast< PatternStyle >( m_uParam1 );
                        switch( eStyle )
                        {
                            case PATTERN_DEFAULT_BRICK:
                            {
                                // this should help generalise this elsewhere when copy-pasted...
                                const float fBrickProportion = 0.95f;
                                const float fRowOffset = 0.5f;
                                for( u_int v = 0; v < uHeight; ++v )
                                {
                                    for( u_int u = 0; u < uWidth; ++u )
                                    {
                                        // start with blank, then fill in the brick areas
                                        pxData[ v * uWidth + u ] = GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 0.0f );
                                        
                                        const float fX = static_cast< float >( u ) / static_cast< float >( uWidth );
                                        const float fY = static_cast< float >( v ) / static_cast< float >( uHeight );

                                        // the complete brick is quite easy...
                                        if( ( fX < fBrickProportion ) && ( fY < ( fBrickProportion - 0.5f ) ) && ( fY <= 0.5f ) )
                                        {
                                            pxData[ v * uWidth + u ] = GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f );
                                        }
                                        // ...actually, so are the two halves on the bottom
                                        if( ( GLToy_Maths::Wrap( fX + fRowOffset ) < fBrickProportion ) && ( fY < fBrickProportion ) && ( fY > 0.5f ) )
                                        {
                                            pxData[ v * uWidth + u ] = GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f );
                                        }
                                    }
                                }
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    pxData = s_xRenderStack.Pop();

    switch( m_eBlendMode )
    {
        default:
        case BLEND_ALPHA:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const float& fAlpha = pxData[ u ][ 3 ];
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                pxData[ u ] = xColour1 * fAlpha + xColour2 * ( 1.0f - fAlpha );
            }
            break;
        }

        case BLEND_MUL:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                GLToy_Vector_4 xCombined = xColour2;
                xCombined[ 0 ] *= xColour1[ 0 ];
                xCombined[ 1 ] *= xColour1[ 1 ];
                xCombined[ 2 ] *= xColour1[ 2 ];
                pxData[ u ] = xCombined;
            }
            break;
        }

        case BLEND_ADD:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                const GLToy_Vector_4 xCombined = xColour1 + xColour2;
                pxData[ u ] = xCombined;
            }
            break;
        }

        case BLEND_SUB:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                GLToy_Vector_4 xCombined = xColour2 - xColour1;
                xCombined[ 3 ] = xColour2[ 3 ];
                pxData[ u ] = xCombined;
            }
            break;
        }

        case BLEND_MAX:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                GLToy_Vector_4 xCombined = xColour2;
                xCombined[ 0 ] = GLToy_Maths::Max( xCombined[ 0 ], xColour1[ 0 ] );
                xCombined[ 1 ] = GLToy_Maths::Max( xCombined[ 1 ], xColour1[ 1 ] );
                xCombined[ 2 ] = GLToy_Maths::Max( xCombined[ 2 ], xColour1[ 2 ] );
                xCombined[ 3 ] = 1.0f;
                pxData[ u ] = xCombined;
            }
            break;
        }

        case BLEND_MIN:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                GLToy_Vector_4 xCombined = xColour2;
                xCombined[ 0 ] = GLToy_Maths::Min( xCombined[ 0 ], xColour1[ 0 ] );
                xCombined[ 1 ] = GLToy_Maths::Min( xCombined[ 1 ], xColour1[ 1 ] );
                xCombined[ 2 ] = GLToy_Maths::Min( xCombined[ 2 ], xColour1[ 2 ] );
                xCombined[ 3 ] = 1.0f;
                pxData[ u ] = xCombined;
            }
            break;
        }

        case BLEND_LUMINANCE_INTO_ALPHA:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4& xColour1 = pxData[ u ];
                const float fAlpha = ( xColour1[ 0 ] + xColour1[ 1 ] + xColour1[ 2 ] ) / 3.0f;
                const GLToy_Vector_4& xColour2 = s_xRenderStack.Peek()[ u ];
                pxData[ u ] = GLToy_Vector_4( xColour2[ 0 ], xColour2[ 1 ], xColour2[ 2 ], fAlpha );
            }
            break;
        }

        case BLEND_REPLACE:
        {
            break;
        }
    }
    
    // swap the data
    GLToy_Vector_4* pxOld = s_xRenderStack.Peek();
    s_xRenderStack.Peek() = pxData;
    delete[] pxOld;
}

u_int* GLToy_Texture_Procedural::CreateRGBA( const u_int uWidth, const u_int uHeight )
{
    GLToy_Vector_4* pxData = new GLToy_Vector_4[ uWidth * uHeight ];

    GLToy_Memory::SetDWords( pxData, uWidth * uHeight * 4, 0 );

    // initialise render state
    LayerNode::s_xRenderStack.Push( pxData );
    LayerNode::s_bWrap = true;
    LayerNode::s_xLight = GLToy_Vector_3( 0.533f, 0.533f, 0.533f );

    // traverse the tree
    GLToy_Iterate( LayerNode, xIterator, &m_xLayers )
    {
        xIterator.Current().Render( uWidth, uHeight );
    }

    pxData = LayerNode::s_xRenderStack.Pop();

    u_int* puData = new u_int[ uWidth * uHeight ];
    for( u_int u = 0; u < uWidth * uHeight; ++u )
    {
        puData[ u ] = pxData[ u ].GetRGBA();
    }

    delete[] pxData;

    return puData;
}

u_int* GLToy_Texture_Procedural::CreateRGBA_4xSS( const u_int uWidth, const u_int uHeight )
{
    const u_int uWidth2 = uWidth << 1;
    const u_int uHeight2 = uHeight << 1;
    u_int* const puData4 = CreateRGBA( uWidth2, uHeight2 );
    u_int* const puData = new u_int[ uWidth * uHeight ];

    // 4x supersample the output
    for( u_int v = 0; v < uHeight; ++v )
    {
        for( u_int u = 0; u < uWidth; ++u )
        {
            const u_int s = u << 1;
            const u_int t = v << 1;
            GLToy_Vector_4 xOutput;
            xOutput = GLToy_Vector_4( puData4[ t * uWidth2 + s ] )
                + GLToy_Vector_4( puData4[ t * uWidth2 + s + 1 ] )
                + GLToy_Vector_4( puData4[ ( t + 1 ) * uWidth2 + s ] )
                + GLToy_Vector_4( puData4[ ( t + 1 ) * uWidth2 + s + 1 ] );

            xOutput *= 0.25f;

            puData[ v * uWidth + u ] = xOutput.GetRGBA();
        }
    }

    delete[] puData4;
    return puData;
}

u_int* GLToy_Texture_Procedural::CreateRGBA_16xSS( const u_int uWidth, const u_int uHeight )
{
    const u_int uWidth4 = uWidth << 2;
    const u_int uHeight4 = uHeight << 2;
    u_int* const puData16 = CreateRGBA( uWidth4, uHeight4 );
    u_int* const puData = new u_int[ uWidth * uHeight ];

    // 4x supersample the output
    for( u_int v = 0; v < uHeight; ++v )
    {
        for( u_int u = 0; u < uWidth; ++u )
        {
            const u_int s = u << 2;
            const u_int t = v << 2;
            GLToy_Vector_4 xOutput;
            xOutput = GLToy_Vector_4( puData16[ t * uWidth4 + s ] )
                + GLToy_Vector_4( puData16[ t * uWidth4 + s + 1 ] )
                + GLToy_Vector_4( puData16[ t * uWidth4 + s + 2 ] )
                + GLToy_Vector_4( puData16[ t * uWidth4 + s + 3 ] )
                + GLToy_Vector_4( puData16[ ( t + 1 ) * uWidth4 + s ] )
                + GLToy_Vector_4( puData16[ ( t + 1 ) * uWidth4 + s + 1 ] )
                + GLToy_Vector_4( puData16[ ( t + 1 ) * uWidth4 + s + 2 ] )
                + GLToy_Vector_4( puData16[ ( t + 1 ) * uWidth4 + s + 3 ] )
                + GLToy_Vector_4( puData16[ ( t + 2 ) * uWidth4 + s ] )
                + GLToy_Vector_4( puData16[ ( t + 2 ) * uWidth4 + s + 1 ] )
                + GLToy_Vector_4( puData16[ ( t + 2 ) * uWidth4 + s + 2 ] )
                + GLToy_Vector_4( puData16[ ( t + 2 ) * uWidth4 + s + 3 ] )
                + GLToy_Vector_4( puData16[ ( t + 3 ) * uWidth4 + s ] )
                + GLToy_Vector_4( puData16[ ( t + 3 ) * uWidth4 + s + 1 ] )
                + GLToy_Vector_4( puData16[ ( t + 3 ) * uWidth4 + s + 2 ] )
                + GLToy_Vector_4( puData16[ ( t + 3 ) * uWidth4 + s + 3 ] );

            xOutput *= 0.0625f;

            puData[ v * uWidth + u ] = xOutput.GetRGBA();
        }
    }

    delete[] puData16;
    return puData;
}

void GLToy_Texture_Procedural::CreateTexture( const GLToy_String& szName, const u_int uWidth, const u_int uHeight )
{
    // TODO: once there are some actual textures see if 16x is acceptably fast
    // SE - 03/11/2010 - nope definately nowhere near fast enough to always use 16x
    // all the allocation/deallocation is a serious concern and the layer operations
    // themselves are pretty slow - aligned vectors might help that but part of the
    // problem is that these are intrinsically slow operations
    u_int* const puData = CreateRGBA_4xSS( uWidth, uHeight );

    GLToy_Texture_System::CreateTextureFromRGBAData( szName.GetHash(), puData, uWidth, uHeight );

    delete[] puData;
}

void GLToy_Texture_Procedural::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    // 21-bit header - every bit counts - 16 for "twocc", 5 for version

    char cHeader1, cHeader2;
    xStream >> cHeader1;
    xStream >> cHeader2;

    if( cHeader1 != 'P' || cHeader2 != 'T' )
    {
        GLToy_Assert( cHeader1 == 'P' && cHeader2 == 'T', "Procedural texture header is invalid!" );
        return;
    }

    u_int uVersion;
    xStream.ReadBits( uVersion, 5 ); // 32 versions should be plenty - we can always hax if it goes over...

    if( uVersion > uCURRENT_VERSION )
    {
        GLToy_Assert( uVersion <= uCURRENT_VERSION, "Trying to load unsupported version of procedural texture!" );
        return;
    }

    xStream >> m_xLayers;
}

void GLToy_Texture_Procedural::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    // 21-bit header - every bit counts - 16 for "twocc", 5 for version
    xStream.WriteChar( 'P' );
    xStream.WriteChar( 'T' );
    xStream.WriteBits( uCURRENT_VERSION, 5 );

    // actual data
    xStream << m_xLayers;
}

void GLToy_Texture_Procedural::ReadNoHeader( const char* const pcData, const u_int uLength )
{
    // to create from constants - save 21-bits on header and assume its the latest version
    GLToy_BitStream xStream;
    xStream.CopyFromByteArray( pcData, uLength );
    xStream.ResetReadPosition();
    xStream >> m_xLayers;
}

void GLToy_Texture_Procedural::SaveToCPPHeader( const GLToy_String& szName, const GLToy_String* pszFilename )
{
    GLToy_String szCleanName( "" );
    // TODO: GLToy_String::Replace
    for( u_int u = 0; u < szName.GetLength(); ++u )
    {
        if( szName[ u ] != '.' )
        {
            szCleanName += szName[ u ];
        }
    }

    GLToy_File xFile( pszFilename ? *pszFilename : ( szCleanName + ".h" ) );

    GLToy_BitStream xStream;
    xStream << m_xLayers;

    GLToy_String szData = GLToy_String() +
        "#ifndef __PTX_" + szCleanName + "_H_\r\n"
        "#define __PTX_" + szCleanName + "_H_\r\n"
        "\r\n"
        "static const char acPTX_" + szCleanName + "[] =\r\n"
        "{\r\n";
    for( u_int u = 0; u < xStream.GetBytesWritten(); ++u )
    {
        GLToy_String szByte;
        szByte.SetToFormatString( "    0x%X,\r\n", reinterpret_cast< u_char* >( xStream.GetData() )[ u ] );
        szData += szByte;
    }
    szData +=
        "};\r\n"
        "\r\n"
        "#endif\r\n";

    char* szRaw = szData.CreateANSIString();

    GLToy_BitStream xWriteStream;
    xWriteStream.SetFromByteArray( szRaw, szData.GetCount() - 1 );
    xFile.WriteFromBitStream( xWriteStream );
}

void GLToy_Texture_Procedural::SaveToTGAFile( const GLToy_String& szFilename, const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    u_int* puData = NULL;
    switch( uSamples )
    {
        case 16:
        {
            puData = CreateRGBA_16xSS( uWidth, uHeight );
            break;
        }

        case 4:
        {
            puData = CreateRGBA_4xSS( uWidth, uHeight );
            break;
        }

        default:
        {
            puData = CreateRGBA( uWidth, uHeight );
            break;
        }
    }

    GLToy_Texture_System::Platform_SaveTextureTGA( szFilename, puData, uWidth, uHeight );
    delete[] puData;
}
u_int GLToy_Texture_Procedural::MoveLayerAfter( const u_int uID, const u_int uAfterID )
{
    if( uAfterID == 0 )
    {
        LayerNode* pxChild = GetLayerNodeFromID( uID );
        if( !pxChild )
        {
            return 0;
        }

        LayerNode xCopy( *pxChild );
        xCopy.AssignNewID();
        DeleteLayerNodeFromID( uID );

        m_xLayers.Append( xCopy );
        return xCopy.GetID();
    }

    GLToy_Array< LayerNode >* pxArray = GetParentArrayFromID( uAfterID );

    if( !pxArray )
    {
        return 0;
    }

    LayerNode* pxChild = GetLayerNodeFromID( uID );
    if( !pxChild )
    {
        return 0;
    }
    
    LayerNode xCopy( *pxChild );
    xCopy.AssignNewID();
    DeleteLayerNodeFromID( uID );

    const u_int uIndex = GetIndexInArrayFromID( uAfterID );
    if( uIndex == ( pxArray->GetCount() - 1 ) )
    {
        pxArray->Append( xCopy );
    }
    else
    {
        pxArray->InsertAt( uIndex + 1, xCopy );
    }

    const u_int uNewPosition = GetPositionFromID( xCopy.GetID() );
    UpdateReferencesFromInsert( uNewPosition, GetTotalChildCount( xCopy.GetID() ) );

    return xCopy.GetID();
}

u_int GLToy_Texture_Procedural::MoveLayerBefore( const u_int uID, const u_int uBeforeID )
{
    GLToy_Array< LayerNode >* pxArray = GetParentArrayFromID( uBeforeID );

    if( !pxArray )
    {
        return 0;
    }

    LayerNode* pxChild = GetLayerNodeFromID( uID );
    if( !pxChild )
    {
        return 0;
    }
    
    LayerNode xCopy( *pxChild );
    xCopy.AssignNewID();
    DeleteLayerNodeFromID( uID );

    const u_int uIndex = GetIndexInArrayFromID( uBeforeID );

    pxArray->InsertAt( uIndex, xCopy );
    const u_int uNewPosition = GetPositionFromID( xCopy.GetID() );
    UpdateReferencesFromInsert( uNewPosition, GetTotalChildCount( xCopy.GetID() ) );

    return xCopy.GetID();
}

u_int GLToy_Texture_Procedural::MoveLayerUnder( const u_int uID, const u_int uUnderID )
{
    LayerNode* pxChild = GetLayerNodeFromID( uID );
    if( !pxChild )
    {
        return 0;
    }

    LayerNode xCopy( *pxChild );
    xCopy.AssignNewID();
    DeleteLayerNodeFromID( uID );

    LayerNode* pxParent = GetLayerNodeFromID( uUnderID );
    if( !pxParent || pxParent->IsLeaf() )
    {
        return 0;
    }

    GLToy_Array< LayerNode >* pxChildren = pxParent->GetChildren();
    if( !pxChildren )
    {
        return 0;
    }

    pxChildren->Append( xCopy );

    const u_int uNewPosition = GetPositionFromID( xCopy.GetID() );
    UpdateReferencesFromInsert( uNewPosition, GetTotalChildCount( xCopy.GetID() ) );
    
    return xCopy.GetID();
}

u_int GLToy_Texture_Procedural::MoveLayerToOwnGroup( const u_int uID )
{
    GLToy_Array< LayerNode >* pxArray = GetParentArrayFromID( uID );

    if( !pxArray )
    {
        return 0;
    }

    LayerNode xLayerNode = LayerNode::CreateGroup( this );
    pxArray->Append( xLayerNode );

    return MoveLayerUnder( uID, xLayerNode.GetID() );
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateFill( GLToy_Texture_Procedural* const pxParentTexture, const u_int uRGBA )
{
    LayerNode xReturnValue;
    xReturnValue.m_uParam1 = uRGBA;
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateNoise( GLToy_Texture_Procedural* const pxParentTexture, const float fFrequency, const u_int uSeed )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_NOISE;
    xReturnValue.m_fParam1 = fFrequency;
    xReturnValue.m_uParam2 = uSeed;
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateTile( GLToy_Texture_Procedural* const pxParentTexture, const u_int uFrequency )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_TILE;
    xReturnValue.m_uParam1 = uFrequency;
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateCircle( GLToy_Texture_Procedural* const pxParentTexture, const GLToy_Vector_2& xPosition, const float fRadius, const u_int uRGBA )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_CIRCLE;
    xReturnValue.m_ausParam1[ 0 ] = static_cast< u_short >( GLToy_Maths::Clamp( xPosition[ 0 ] ) * 65535.0f );
    xReturnValue.m_ausParam1[ 1 ] = static_cast< u_short >( GLToy_Maths::Clamp( xPosition[ 1 ] ) * 65535.0f );
    xReturnValue.m_fParam2 = fRadius;
    xReturnValue.m_uParam3 = uRGBA;
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateFBMNoise( GLToy_Texture_Procedural* const pxParentTexture, const float fFrequency, const u_int uSeed )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_FBMNOISE;
    xReturnValue.m_fParam1 = fFrequency;
    xReturnValue.m_uParam2 = uSeed;
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateShaping( GLToy_Texture_Procedural* const pxParentTexture, const ShapeFunction eShapeFunction )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_SHAPE;
    xReturnValue.m_uParam1 = static_cast< u_int >( eShapeFunction );
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateGradient( GLToy_Texture_Procedural* const pxParentTexture, const GradientStyle eGradientStyle )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_GRADIENT;
    xReturnValue.m_uParam1 = static_cast< u_int >( eGradientStyle );
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateExtension( GLToy_Texture_Procedural* const pxParentTexture, const ExtensionFunction eExtensionFunction, const u_int uParam1, const u_int uParam2, const u_int uParam3 )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_EXTENSION;
    xReturnValue.m_eExtensionFunction = eExtensionFunction;
    xReturnValue.m_uParam1 = uParam1;
    xReturnValue.m_uParam2 = uParam2;
    xReturnValue.m_uParam3 = uParam3;
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateGroup( GLToy_Texture_Procedural* const pxParentTexture )
{
    LayerNode xReturnValue;
    xReturnValue.m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >();
    xReturnValue.SetParent( pxParentTexture );
    return xReturnValue;
}

const char* GLToy_Texture_Procedural::GetShapingFunctionName( const ShapeFunction eFunction )
{
    switch( eFunction )
    {
        case SHAPE_COS_2PI:                 return "Cosine 2pi x";
        case SHAPE_SIN_2PI:                 return "Sine 2pi x";
        case SHAPE_COS_4PI:                 return "Cosine 4pi x";
        case SHAPE_SIN_4PI:                 return "Sine 4pi x";
        case SHAPE_COS_6PI:                 return "Cosine 6pi x";
        case SHAPE_SIN_6PI:                 return "Sine 6pi x";
        case SHAPE_SQUARE:                  return "Square";
        case SHAPE_SQUAREROOT:              return "Square Root";
        case SHAPE_ABS:                     return "Absolute Value";
        case SHAPE_HALF:                    return "Half";
        case SHAPE_DOUBLE:                  return "Double";
        case SHAPE_CLIPBELOWHALF:           return "Clip Below Half";
        case SHAPE_CLIPABOVEHALF:           return "Clip Above Half";
        case SHAPE_INVERT:                  return "Invert";
        case SHAPE_MIN_50:                  return "Minimum 50%";
        case SHAPE_MAX_50:                  return "Maximum 50%";
        case SHAPE_MIN_25:                  return "Minimum 25%";
        case SHAPE_MAX_25:                  return "Maximum 25%";
        case SHAPE_MIN_75:                  return "Minimum 75%";
        case SHAPE_MAX_75:                  return "Maximum 75%";
        case SHAPE_MIN_10:                  return "Minimum 10%";
        case SHAPE_MAX_10:                  return "Maximum 10%";
        case SHAPE_MIN_90:                  return "Minimum 90%";
        case SHAPE_MAX_90:                  return "Maximum 90%";
        case SHAPE_INFINITE_MULTIPLIER:     return "Infinite Multiplier";
        default:
        {
            break;
        }
    }
    return "Unknown Shaping Function";
}

const char* GLToy_Texture_Procedural::GetGradientName( const GradientStyle eStyle )
{
    switch( eStyle )
    {
        case GRADIENT_TOP:              return "Top";
        case GRADIENT_BOTTOM:           return "Bottom";
        case GRADIENT_LEFT:             return "Left";
        case GRADIENT_RIGHT:            return "Right";
        case GRADIENT_TOP_LEFT:         return "Top Left";
        case GRADIENT_BOTTOM_LEFT:      return "Bottom Left";
        case GRADIENT_TOP_RIGHT:        return "Top Right";
        case GRADIENT_BOTTOM_RIGHT:     return "Bottom Right";
        case GRADIENT_RADIAL_OUT:       return "Radial Outwards";
        case GRADIENT_RADIAL_IN:        return "Radial Inwards";
        case GRADIENT_SQUARE_OUT:       return "Square Outwards";
        case GRADIENT_SQUARE_IN:        return "Square Inwards";
        default:
        {
            break;
        }
    }

    return "Unknown Gradient";
}

const char* GLToy_Texture_Procedural::GetPatternName( const PatternStyle eStyle )
{
    switch( eStyle )
    {
        case PATTERN_DEFAULT_BRICK:     return "Brick";
        default:
        {
            break;
        }
    }

    return "Unknown Pattern";
}

u_int GLToy_Texture_Procedural::GetPositionFromID( const u_int uID ) const
{
    if( uID == 0 )
    {
        return 0;
    }

    // iterate over by going down eagerly, then across
    u_int uPosition = 1;
    u_int uCursor = 0;
    const GLToy_Array< LayerNode >* pxArray = &m_xLayers;
    while( true )
    {
        const LayerNode& xCurrent = ( *pxArray )[ uCursor ];
        if( xCurrent.GetID() == uID )
        {
            return uPosition;
        }

        ++uPosition;
        // if we have children see if we can go down
        if( !xCurrent.IsLeaf() )
        {
            if( xCurrent.GetChildren()->GetCount() > 0 )
            {
                pxArray = xCurrent.GetChildren();
                uCursor = 0;
                continue;
            }
        }
        
        // otherwise go right
        if( pxArray->GetCount() && ( uCursor < ( pxArray->GetCount() - 1 ) ) )
        {
            ++uCursor;
            continue;
        }
        
        // otherwise, go up and right
        bool bGoUp = true;
        u_int uClimbID = xCurrent.GetID();
        while( bGoUp )
        {
            const u_int uParentID = GetParentIDFromID( uClimbID );
            pxArray = GetParentArrayFromID( uParentID );
            if( !pxArray )
            {
                return 0; // must have reached the end
            }

            uCursor = GetIndexInArrayFromID( uParentID );
            if( uCursor >= ( pxArray->GetCount() - 1 ) )
            {
                bGoUp = true;
                uClimbID = uParentID;
            }
            else
            {
                ++uCursor;
                break;
            }

            if( ( pxArray == &m_xLayers ) && bGoUp )
            {
                return 0; // done - can't find
            }
        }
    }

    return 0;
}

u_int GLToy_Texture_Procedural::GetIDFromPosition( const u_int uPosition ) const
{
    u_int uIteratorPosition = 1;
    u_int uCursor = 0;
    const GLToy_Array< LayerNode >* pxArray = &m_xLayers;
    while( true )
    {
        const LayerNode& xCurrent = ( *pxArray )[ uCursor ];
        if( uIteratorPosition == uPosition )
        {
            return xCurrent.GetID();
        }

        ++uIteratorPosition;
        // if we have children see if we can go down
        if( !xCurrent.IsLeaf() )
        {
            if( xCurrent.GetChildren()->GetCount() > 0 )
            {
                pxArray = xCurrent.GetChildren();
                uCursor = 0;
                continue;
            }
        }

        // otherwise go right
        if( pxArray->GetCount() && ( uCursor < ( pxArray->GetCount() - 1 ) ) )
        {
            ++uCursor;
            continue;
        }

        // otherwise, go up and right
        bool bGoUp = true;
        u_int uClimbID = xCurrent.GetID();
        while( bGoUp )
        {
            const u_int uParentID = GetParentIDFromID( uClimbID );
            pxArray = GetParentArrayFromID( uParentID );
            uCursor = GetIndexInArrayFromID( uParentID );
            if( !pxArray )
            {
                return 0; // must have reached the end
            }

            if( uCursor >= ( pxArray->GetCount() - 1 ) )
            {
                bGoUp = true;
                uClimbID = uParentID;
            }
            else
            {
                ++uCursor;
                break;
            }

            if( ( pxArray == &m_xLayers ) && bGoUp )
            {
                return 0; // done - can't find
            }
        }
    }

    return 0;
}

u_int GLToy_Texture_Procedural::GetLastPosition() const
{
    u_int uIteratorPosition = 1;
    u_int uCursor = 0;
    const GLToy_Array< LayerNode >* pxArray = &m_xLayers;
    while( true )
    {
        const LayerNode& xCurrent = ( *pxArray )[ uCursor ];

        if( uCursor >= ( pxArray->GetCount() - 1 ) )
        {
            // pointing beyond the end of the array...
        }

        ++uIteratorPosition;

        // if we have children see if we can go down
        if( !xCurrent.IsLeaf() )
        {
            if( xCurrent.GetChildren()->GetCount() > 0 )
            {
                pxArray = xCurrent.GetChildren();
                uCursor = 0;
                continue;
            }
        }
        
        // otherwise go right
        if( pxArray->GetCount() && ( uCursor < ( pxArray->GetCount() - 1 ) ) )
        {
            ++uCursor;
            continue;
        }
        
        // otherwise, go up and right
        bool bGoUp = true;
        u_int uClimbID = xCurrent.GetID();
        while( bGoUp )
        {
            const u_int uParentID = GetParentIDFromID( uClimbID );
            pxArray = GetParentArrayFromID( uParentID );
            if( !pxArray ) // we must have reached the end
            {
                return --uIteratorPosition;
            }

            uCursor = GetIndexInArrayFromID( uParentID );
            if( uCursor >= ( pxArray->GetCount() - 1 ) )
            {
                bGoUp = true;
                uClimbID = uParentID;
            }
            else
            {
                ++uCursor;
                break;
            }

            if( ( pxArray == &m_xLayers ) && bGoUp )
            {
                return --uIteratorPosition;
            }
        }
    }

    return 0;
}

void GLToy_Texture_Procedural::UpdateReferencesFromDelete( const u_int uDeletedPosition, const u_int uChildCount )
{
    // this is expected to be done /before/ delete actually happens
    const u_int uLastPosition = GetLastPosition();
    if( uDeletedPosition > uLastPosition )
    {
        return;
    }

    // if the deleted object was a group then we have to be careful to adjust accordingly
    const u_int uAdjustAmount = uChildCount + 1;

    // any references to our position or children should be set to refer to null...
    for( u_int u = uDeletedPosition; u < ( uDeletedPosition + uAdjustAmount ); ++u )
    {
        LayerNode* pxReference = GetFirstReferenceTo( u );
        while( pxReference )
        {
            pxReference->m_uParam1 = 0;
            pxReference = GetFirstReferenceTo( u );
        }
    }

    // everything after the deleted position needs to be corrected
    const u_int uFirstModified = uDeletedPosition + uAdjustAmount;

    for( u_int u = uFirstModified; u < ( uLastPosition + 1 ); ++u )
    {
        LayerNode* pxReference = GetFirstReferenceTo( u );
        while( pxReference )
        {
            pxReference->m_uParam1 = u - uAdjustAmount;
            pxReference = GetFirstReferenceTo( u );
        }
    }
}

void GLToy_Texture_Procedural::UpdateReferencesFromInsert( const u_int uInsertedPosition, const u_int uChildCount )
{
    // this is expected to be done /after/ insert actually happens
    const u_int uLastPosition = GetLastPosition();
    if( uInsertedPosition > uLastPosition )
    {
        return;
    }
    // everything after us needs to be incremented...

    // if the inserted object is a group then we have to be careful to adjust accordingly
    const u_int uAdjustAmount = uChildCount + 1;

    // everything after the inserted position needs to be corrected
    const u_int uFirstModified = uInsertedPosition + uAdjustAmount;

    for( u_int u = uFirstModified; u < ( uLastPosition + 1 ); ++u )
    {
        LayerNode* pxReference = GetFirstReferenceTo( u );
        while( pxReference )
        {
            pxReference->m_uParam1 = u + uAdjustAmount;
            pxReference = GetFirstReferenceTo( u );
        }
    }
}

void GLToy_Texture_Procedural::UpdateReferencesFromMove( const u_int uOldPosition, const u_int uNewPosition, const u_int uChildCount )
{
    UpdateReferencesFromDelete( uOldPosition, uChildCount );
    UpdateReferencesFromInsert( uNewPosition, uChildCount );
}

bool GLToy_Texture_Procedural::CircularReferenceCheck( const u_int uReferenceID ) const
{
    // is it even a valid ID?
    const LayerNode* const pxLayerNode = GetLayerNodeFromID( uReferenceID );
    if( !pxLayerNode )
    {
        return false; // no node, no circular reference...
    }

    // is it even an extension layer?
    if( pxLayerNode->m_eInstruction != INSTRUCTION_EXTENSION )
    {
        return false; // only extensions can be references
    }

    // is it even a reference layer?
    if( pxLayerNode->m_eExtensionFunction != EXTENSION_REFERENCE )
    {
        return false; // only references can cause circular references
    }

    const u_int uReferredToPosition = pxLayerNode->m_uParam1;

    // does it even reference a valid position
    if( uReferredToPosition == 0 )
    {
        // these might be left over from deletion... at any rate they reference nothing
        return false;
    }

    const u_int uReferredToID = GetIDFromPosition( uReferredToPosition );
    // does it even reference a valid ID
    if( uReferredToID == 0 )
    {
        // this is a bad reference - shouldn't happen, and not likely to cause circular references...
        return false;
    }

    // ... and not itself
    if( uReferenceID == uReferredToID )
    {
        return true; // self-referencing is a circular reference
    }

    // does it reference any of its parents?
    u_int uID = uReferenceID;
    while( uID != 0 )
    {
        uID = GetParentIDFromID( uID );
        if( uID == uReferredToID )
        {
            return true; // can't reference own parent - that makes a circular reference
        }
    }

    return false;
}

GLToy_Vector_4 GLToy_Texture_Procedural::LayerNode::WrapAwareSample( const int u, const int v, const u_int uWidth, const u_int uHeight, const GLToy_Vector_4* const pxBuffer )
{
    const u_int s = s_bWrap ? ( u % uWidth ) : GLToy_Maths::Clamp( static_cast< u_int >( u ), 0u, uWidth );
    const u_int t = s_bWrap ? ( v % uHeight ) : GLToy_Maths::Clamp( static_cast< u_int >( v ), 0u, uHeight );
    return pxBuffer[ t * uWidth + s ];
}

GLToy_Vector_4 GLToy_Texture_Procedural::LayerNode::WrapAwareSampleFiltered( const float fX, const float fY, const u_int uWidth, const u_int uHeight, const GLToy_Vector_4* const pxBuffer )
{
    // TODO: something better... cubic is good, but I suspect its not good enough for the general case
    // (what happens if the sampling frequency is 0.125x the texture width or height?)

    const float fT = fX * static_cast< float >( uWidth );
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) ) - 1;
    u_int u2 = u1 + 1;
	u_int u3 = u2 + 1;
    u_int u4 = u3 + 1;
    
    const float fS = fY * static_cast< float >( uHeight );
    u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) ) - 1;
    u_int v2 = v1 + 1;
	u_int v3 = v2 + 1;
    u_int v4 = v3 + 1;

	const float fU2 = static_cast< float >( u2 );
	const float fV2 = static_cast< float >( v2 );

    if( s_bWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWidth );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWidth );
        u3 = GLToy_Maths::Wrap( u3, 0u, uWidth );
        u4 = GLToy_Maths::Wrap( u4, 0u, uWidth );
        v1 = GLToy_Maths::Wrap( v1, 0u, uHeight );
        v2 = GLToy_Maths::Wrap( v2, 0u, uHeight );
        v3 = GLToy_Maths::Wrap( v3, 0u, uHeight );
        v4 = GLToy_Maths::Wrap( v4, 0u, uHeight );
    }

    const GLToy_Vector_4 xX1 = GLToy_Maths::CatmullRomInterpolate( pxBuffer[ v1 * uWidth + u1 ], pxBuffer[ v1 * uWidth + u2 ], pxBuffer[ v1 * uWidth + u3 ], pxBuffer[ v1 * uWidth + u4 ], fT - fU2 );
    const GLToy_Vector_4 xX2 = GLToy_Maths::CatmullRomInterpolate( pxBuffer[ v2 * uWidth + u1 ], pxBuffer[ v2 * uWidth + u2 ], pxBuffer[ v2 * uWidth + u3 ], pxBuffer[ v2 * uWidth + u4 ], fT - fU2 );
	const GLToy_Vector_4 xX3 = GLToy_Maths::CatmullRomInterpolate( pxBuffer[ v3 * uWidth + u1 ], pxBuffer[ v3 * uWidth + u2 ], pxBuffer[ v3 * uWidth + u3 ], pxBuffer[ v3 * uWidth + u4 ], fT - fU2 );
	const GLToy_Vector_4 xX4 = GLToy_Maths::CatmullRomInterpolate( pxBuffer[ v4 * uWidth + u1 ], pxBuffer[ v4 * uWidth + u2 ], pxBuffer[ v4 * uWidth + u3 ], pxBuffer[ v4 * uWidth + u4 ], fT - fU2 );

    return GLToy_Maths::CatmullRomInterpolate( xX1, xX2, xX3, xX4, fS - fV2 );
}
