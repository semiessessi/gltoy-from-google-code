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
#include <File/GLToy_OBJFile.h>

// GLToy
#include <Model/GLToy_Model_FlatMaterials.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model* GLToy_OBJFile::LoadModel() const
{
    if( m_pxModel )
    {
        return m_pxModel;
    }

    GLToy_Model_FlatMaterials* const pxModel = new GLToy_Model_FlatMaterials();
    m_pxModel = pxModel;

    char* pcData = new char[ GetSize() ];
    GetAllData( pcData );
    GLToy_String szData( pcData );
    delete[] pcData;

    // go through the data and remove junk, build index arrays etc.
    struct FaceVertex
    {
        u_int m_uIndices[ 3 ];

        FaceVertex() { m_uIndices[ 0 ] = m_uIndices[ 1 ] = m_uIndices[ 2 ] = 0; }
    };

    GLToy_Array< GLToy_String > xLines = szData.Split( '\n' );
    GLToy_Iterate( GLToy_String, xIterator, &xLines )
    {
        GLToy_String& szLine = xIterator.Current();
        szLine.TrimTrailingWhiteSpace();
        if( szLine.IsEmpty() )
        {
            continue;
        }

        switch( szLine[ 0 ] )
        {
            case '#':
            {
                // comment
                continue;
            }

            case 'f':
            {
                // face
                // f 0/1/2 1/2/3 etc.
                pxModel->AppendFace();
                
                GLToy_Array< GLToy_String > xFaceVertices = szLine.Right( szLine.GetLength() - 2 ).Split( ' ' );
                GLToy_Iterate( GLToy_String, xIterator, &xFaceVertices )
                {
                    GLToy_String& szVertex = xIterator.Current();
                    GLToy_Array< GLToy_String > xIndices = szVertex.Split( '/' );
                    
                    u_int auIndices[ 3 ];
                    for( u_int u = 0; u < 3; ++u )
                    {
                        if( u < xIndices.GetCount() )
                        {
                            auIndices[ u ] = xIndices[ u ].ExtractUnsignedInt();
                        }
                    }

                    pxModel->AppendFaceVertex( auIndices[ 0 ], auIndices[ 1 ], auIndices[ 2 ] );
                }
                break;
            }

            case 'v':
            {
                if( szLine[ 1 ] == 't' )
                {
                    // texture coordinate
                    // vt 0.12 1.23 2.34                
                    GLToy_Array< GLToy_String > xValues = szLine.Right( szLine.GetLength() - 3 ).Split( ' ' );
                
                    if( xValues.GetCount() < 2 )
                    {
                        continue;
                    }

                    pxModel->AppendUV( GLToy_Vector_2( xValues[ 0 ].ExtractFloat(), xValues[ 1 ].ExtractFloat() ) );
                }
                else if( szLine[ 1 ] == 'n' )
                {
                    // normal
                    // vn 0.12 1.23 2.34
                    GLToy_Array< GLToy_String > xValues = szLine.Right( szLine.GetLength() - 3 ).Split( ' ' );
                
                    if( xValues.GetCount() < 3 )
                    {
                        continue;
                    }

                    GLToy_Vector_3 xNormal( xValues[ 0 ].ExtractFloat(), xValues[ 1 ].ExtractFloat(), xValues[ 2 ].ExtractFloat() );
                    xNormal.Normalise();
                    pxModel->AppendNormal( xNormal );
                }
                else
                {
                    // vertex
                    // v 0.12 1.23 2.34                
                    GLToy_Array< GLToy_String > xValues = szLine.Right( szLine.GetLength() - 2 ).Split( ' ' );
                
                    if( xValues.GetCount() < 3 )
                    {
                        continue;
                    }

                    pxModel->AppendVertex( GLToy_Vector_3( xValues[ 0 ].ExtractFloat(), xValues[ 1 ].ExtractFloat(), xValues[ 2 ].ExtractFloat() ) );
                }
                break;
            }
        }
    }

    // TODO - materials
    m_pxModel->UpdateStripPointers();
    return m_pxModel;
}
