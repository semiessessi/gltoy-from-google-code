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
    u_int uBlendMode;
    xStream.ReadBits( uBlendMode, 2 );
    m_eBlendMode = static_cast< BlendMode >( uBlendMode );

    delete m_pxChildren;

    bool bLeaf;
    xStream >> bLeaf;
    if( !bLeaf )
    {
        m_pxChildren = new GLToy_SmallSerialisableArray< LayerNode >();
        xStream >> m_pxChildren;
    }
    else
    {
        u_int uInstruction;
        xStream.ReadBits( uInstruction, 3 );
        m_pxChildren = NULL;
        m_eInstruction = static_cast< Instruction >( uInstruction );

        switch( m_eInstruction ) // note the deliberate fall-through
        {
            case INSTRUCTION_NOISE:
                xStream >> m_uParam2;
            default:
                xStream >> m_uParam1;
        };
    }
}

void GLToy_Texture_Procedural::LayerNode::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream.WriteBits( m_eBlendMode, 2 );

    xStream << ( m_pxChildren == NULL );
    if( m_pxChildren )
    {
        xStream << m_pxChildren;
    }
    else
    {
        xStream.WriteBits( m_eInstruction, 3 );

        switch( m_eInstruction ) // note the deliberate fall-through
        {
            case INSTRUCTION_NOISE:
                xStream << m_uParam2;
            default:
                xStream << m_uParam1;
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
                        const float fNoise = GLToy_Noise::Cubic2D( fX, fY, *reinterpret_cast< float* >( &m_uParam1 ), 1.0f, static_cast< u_int >( *reinterpret_cast< float* >( &m_uParam1 ) ), m_uParam2 );
                        puData[ v * uWidth + u ] = GLToy_Vector_4( fNoise, fNoise, fNoise, 1.0f ).GetRGBA();
                    }
                }
                break;
            }

            case INSTRUCTION_TILE:
            {
                // TODO: tile with some really good filtering...
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
                xCombined[ 3 ] = 1.0f;
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
                xCombined[ 3 ] *= xColour1[ 3 ];
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
                const GLToy_Vector_4 xCombined = xColour2 - xColour1;
                puData[ u ] = xCombined.GetRGBA();
            }
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
    xStream >> m_xLayers;
}

void GLToy_Texture_Procedural::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xLayers;
}
