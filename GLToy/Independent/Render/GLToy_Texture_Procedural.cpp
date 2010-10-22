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

// This file's header
#include <Render/GLToy_Texture_Procedural.h>

// GLToy
#include <File/GLToy_File.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Noise.h>
#include <Render/GLToy_Texture.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Stack< u_int* > GLToy_Texture_Procedural::LayerNode::s_xRenderStack;
u_int GLToy_Texture_Procedural::LayerNode::s_uNextID = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture_Procedural::LayerNode::ReadFromBitStream( const GLToy_BitStream& xStream )
{
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
                
                //switch( m_eExtensionFunction )
                //{
                //    default:
                //        break;
                //}
                
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

                //switch( m_eExtensionFunction )
                //{
                //    default:
                //        break;
                //}
                
                break;
            }
        };
    }
}

void GLToy_Texture_Procedural::LayerNode::Render( const u_int uWidth, const u_int uHeight )
{
    u_int* puData = new u_int[ uWidth * uHeight ];

    GLToy_Memory::SetDWords( puData, uWidth * uHeight, 0 );

    s_xRenderStack.Push( puData );

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
                GLToy_Memory::SetDWords( puData, uWidth * uHeight, m_uParam1 );
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
                        const float fNoise = GLToy_Noise::Cubic2D( fX, fY, m_fParam1, 1.0f, static_cast< u_int >( m_fParam1 ), m_uParam2 );
                        puData[ v * uWidth + u ] = GLToy_Vector_4( fNoise, fNoise, fNoise, 1.0f ).GetRGBA();
                    }
                }
                break;
            }

            case INSTRUCTION_TILE:
            {
                // TODO: some filtering
                // cubic isn't good enough for high frequencies, but would be a good start
                // maybe constuct some mipmaps?
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        const u_int s = ( u * m_uParam1 ) % uWidth;
                        const u_int t = ( v * m_uParam1 ) % uHeight;
                        const float fX = static_cast< float >( s ) / static_cast< float >( uWidth );
                        const float fY = static_cast< float >( t ) / static_cast< float >( uHeight );
                        puData[ v * uWidth + u ] = s_xRenderStack.Peek( 1 )[ t * uWidth + s ];
                    }
                }
                break;
                break;
            }

            case INSTRUCTION_CIRCLE:
            {
                // TODO: some filtering
                // cubic isn't good enough for high frequencies, but would be a good start
                // maybe constuct some mipmaps?
                const float fCentreX = static_cast< float >( m_uParam1 & 0xFFFF ) / 65535.0f;
                const float fCentreY = static_cast< float >( m_uParam1 >> 16 ) / 65535.0f;
                const float fRadius = m_fParam2;
                for( u_int v = 0; v < uHeight; ++v )
                {
                    for( u_int u = 0; u < uWidth; ++u )
                    {
                        const float fX = fCentreX - static_cast< float >( u ) / static_cast< float >( uWidth );
                        const float fY = fCentreY - static_cast< float >( v ) / static_cast< float >( uHeight );
                        if( fX * fX + fY * fY < fRadius * fRadius )
                        {
                            puData[ v * uWidth + u ] = m_uParam3;
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
                        const float fNoise = GLToy_Noise::FractalCubic2D( fX, fY, m_fParam1, 1.0f, 0.5f, 10, true, m_uParam2 );
                        puData[ v * uWidth + u ] = GLToy_Vector_4( fNoise, fNoise, fNoise, 1.0f ).GetRGBA();
                    }
                }
                break;
            }

            case INSTRUCTION_SHAPE:
            {
                for( u_int u = 0; u < uWidth * uHeight; ++u )
                {
                    GLToy_Vector_4 xDest( puData[ u ] );
                    GLToy_Vector_4 xSource( s_xRenderStack.Peek( 1 )[ u ] );
                    
                    switch( static_cast< ShapeFunction >( m_uParam1 ) )
                    {
                        case SHAPE_COS_2PI:
                        {
                            xDest[ 0 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 2.0f * xSource[ 3 ] );
                            break;
                        }

                        case SHAPE_SIN_2PI:
                        {
                            xDest[ 0 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 2.0f * xSource[ 3 ] );
                            break;
                        }

                        case SHAPE_COS_4PI:
                        {
                            xDest[ 0 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 4.0f * xSource[ 3 ] );
                            break;
                        }

                        case SHAPE_SIN_4PI:
                        {
                            xDest[ 0 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 4.0f * xSource[ 3 ] );
                            break;
                        }

                        case SHAPE_COS_6PI:
                        {
                            xDest[ 0 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Cos( GLToy_Maths::Pi * 6.0f * xSource[ 3 ] );
                            break;
                        }

                        case SHAPE_SIN_6PI:
                        {
                            xDest[ 0 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Sin( GLToy_Maths::Pi * 6.0f * xSource[ 3 ] );
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
                            xDest[ 0 ] = GLToy_Maths::Abs( xSource[ 0 ] );
                            xDest[ 1 ] = GLToy_Maths::Abs( xSource[ 1 ] );
                            xDest[ 2 ] = GLToy_Maths::Abs( xSource[ 2 ] );
                            xDest[ 3 ] = GLToy_Maths::Abs( xSource[ 3 ] );
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

                    puData[ u ] = xDest.GetRGBA();
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

                        GLToy_Vector_4 xOutput = GLToy_Vector_4( fLuminance, fLuminance, fLuminance, 1.0f );
                        puData[ v * uWidth + u ] = xOutput.GetRGBA();
                    }
                }
                break;
            }

            case INSTRUCTION_EXTENSION:
            {
                // TODO: implement extensions
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
                                puData[ v * uWidth + u ] = ( fX * fY >= 0.0f ) ? 0xFFFFFFFF : 0x00000000;
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
                                puData[ v * uWidth + u ] = ( fY >= 0.0f ) ? 0xFFFFFFFF : 0x00000000;
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
                                puData[ v * uWidth + u ] = ( fX >= 0.0f ) ? 0xFFFFFFFF : 0x00000000;
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
                                puData[ v * uWidth + u ] =
                                    ( fX + fY >= 0.0f )
                                        ? ( fX + fY >= 1.0f ) ? 0xFFFFFFFF : 0x00000000
                                        : ( fX + fY < -1.0f ) ? 0x00000000 : 0xFFFFFFFF;
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
                                puData[ v * uWidth + u ] =
                                    ( fX + fY < 0.0f )
                                        ? ( fX + fY < 1.0f ) ? 0xFFFFFFFF : 0x00000000
                                        : ( fX + fY >= -1.0f ) ? 0x00000000 : 0xFFFFFFFF;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    puData = s_xRenderStack.Pop();

    switch( m_eBlendMode )
    {
        default:
        case BLEND_ALPHA:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const float fAlpha = static_cast< float >( puData[ u ] >> 24 ) / 255.0f;
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                GLToy_Vector_4 xCombined = xColour1 * fAlpha + xColour2 * ( 1.0f - fAlpha );
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_MUL:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const float fAlpha = static_cast< float >( puData[ u ] >> 24 ) / 255.0f;
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                GLToy_Vector_4 xCombined = xColour2;
                xCombined[ 0 ] *= xColour1[ 0 ];
                xCombined[ 1 ] *= xColour1[ 1 ];
                xCombined[ 2 ] *= xColour1[ 2 ];
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_ADD:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                const GLToy_Vector_4 xCombined = xColour1 + xColour2;
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_SUB:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const float fAlpha = static_cast< float >( puData[ u ] >> 24 ) / 255.0f;
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                GLToy_Vector_4 xCombined = xColour2 - xColour1;
                xCombined[ 3 ] = 1.0f;
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_MAX:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const float fAlpha = static_cast< float >( puData[ u ] >> 24 ) / 255.0f;
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                GLToy_Vector_4 xCombined = xColour2;
                xCombined[ 0 ] = GLToy_Maths::Max( xCombined[ 0 ], xColour1[ 0 ] );
                xCombined[ 1 ] = GLToy_Maths::Max( xCombined[ 1 ], xColour1[ 1 ] );
                xCombined[ 2 ] = GLToy_Maths::Max( xCombined[ 2 ], xColour1[ 2 ] );
                xCombined[ 3 ] = 1.0f;
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_MIN:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const float fAlpha = static_cast< float >( puData[ u ] >> 24 ) / 255.0f;
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                GLToy_Vector_4 xCombined = xColour2;
                xCombined[ 0 ] = GLToy_Maths::Min( xCombined[ 0 ], xColour1[ 0 ] );
                xCombined[ 1 ] = GLToy_Maths::Min( xCombined[ 1 ], xColour1[ 1 ] );
                xCombined[ 2 ] = GLToy_Maths::Min( xCombined[ 2 ], xColour1[ 2 ] );
                xCombined[ 3 ] = 1.0f;
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_LUMINANCE_INTO_ALPHA:
        {
            for( u_int u = 0; u < uWidth * uHeight; ++u )
            {
                const GLToy_Vector_4 xColour1( puData[ u ] );
                const float fAlpha = ( xColour1[ 0 ] + xColour1[ 1 ] + xColour1[ 2 ] ) / 3.0f;
                const GLToy_Vector_4 xColour2( s_xRenderStack.Peek()[ u ] );
                const GLToy_Vector_4 xCombined = GLToy_Vector_4( xColour2[ 0 ], xColour2[ 1 ], xColour2[ 2 ], fAlpha );
                puData[ u ] = xCombined.GetRGBA();
            }
            break;
        }

        case BLEND_REPLACE:
        {
            break;
        }
    }
    
    // swap the data
    u_int* puOld = s_xRenderStack.Peek();
    s_xRenderStack.Peek() = puData;
    delete[] puOld;
}

void GLToy_Texture_Procedural::CreateTexture( const GLToy_String& szName, const u_int uWidth, const u_int uHeight )
{
    u_int* puData = new u_int[ uWidth * uHeight ];

    GLToy_Memory::Set( puData, uWidth * uHeight * 4, 0 );

    LayerNode::s_xRenderStack.Push( puData );

    // traverse the tree
    GLToy_Iterate( LayerNode, xIterator, &m_xLayers )
    {
        xIterator.Current().Render( uWidth, uHeight );
    }

    puData = LayerNode::s_xRenderStack.Pop();

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

void GLToy_Texture_Procedural::SaveToCPPHeader( const GLToy_String& szName )
{
    GLToy_File xFile( szName + ".h" );

    GLToy_BitStream xStream;
    xStream << m_xLayers;

    GLToy_String szData = GLToy_String() +
        "#ifndef __PTX_" + szName + "_H_\r\n"
        "#define __PTX_" + szName + "_H_\r\n"
        "static const char acPTX_" + szName + "[] =\r\n"
        "{\r\n";
    for( u_int u = 0; u < xStream.GetBytesWritten(); ++u )
    {
        GLToy_String szByte;
        szByte.SetToFormatString( "    0x%X,\r\n", reinterpret_cast< u_char* >( xStream.GetData() )[ u ] );
        szData += szByte;
    }
    szData +=
        "}\r\n"
        "#endif\r\n";

    char* szRaw = szData.CreateANSIString();

    GLToy_BitStream xWriteStream;
    xWriteStream.SetFromByteArray( szRaw, szData.GetCount() - 1 );
    xFile.WriteFromBitStream( xWriteStream );
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateFill( const u_int uRGBA )
{
    LayerNode xReturnValue;
    xReturnValue.m_uParam1 = uRGBA;
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateNoise( const float fFrequency, const u_int uSeed )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_NOISE;
    xReturnValue.m_fParam1 = fFrequency;
    xReturnValue.m_uParam2 = uSeed;
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateTile( const u_int uFrequency )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_TILE;
    xReturnValue.m_uParam1 = uFrequency;
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateCircle( const GLToy_Vector_2& xPosition, const float fRadius, const u_int uRGBA )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_CIRCLE;
    xReturnValue.m_uParam1 = static_cast< u_int >( GLToy_Maths::Clamp( xPosition[ 0 ] ) * 65535.0f ) | ( static_cast< u_int >( GLToy_Maths::Clamp( xPosition[ 1 ] ) * 65535.0f ) << 16 );
    xReturnValue.m_fParam2 = fRadius;
    xReturnValue.m_uParam3 = uRGBA;
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateFBMNoise( const float fFrequency, const u_int uSeed )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_FBMNOISE;
    xReturnValue.m_fParam1 = fFrequency;
    xReturnValue.m_uParam2 = uSeed;
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateShaping( const ShapeFunction eShapeFunction )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_SHAPE;
    xReturnValue.m_uParam1 = static_cast< u_int >( eShapeFunction );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateGradient( const GradientStyle eGradientStyle )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_GRADIENT;
    xReturnValue.m_uParam1 = static_cast< u_int >( eGradientStyle );
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateExtension( const ExtensionFunction eExtensionFunction, const u_int uParam1, const u_int uParam2, const u_int uParam3 )
{
    LayerNode xReturnValue;
    xReturnValue.m_eInstruction = INSTRUCTION_EXTENSION;
    xReturnValue.m_eExtensionFunction = eExtensionFunction;
    xReturnValue.m_uParam1 = uParam1;
    xReturnValue.m_uParam2 = uParam2;
    xReturnValue.m_uParam3 = uParam3;
    return xReturnValue;
}

GLToy_Texture_Procedural::LayerNode GLToy_Texture_Procedural::LayerNode::CreateGroup()
{
    LayerNode xReturnValue;
    xReturnValue.m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >();
    return xReturnValue;
}
