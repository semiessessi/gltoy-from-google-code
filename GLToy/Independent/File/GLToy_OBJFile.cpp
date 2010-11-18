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
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_TextFile.h>
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
    GLToy_HashTree< u_int > xMaterialReferences;
    u_int uCurrentMaterial = 0;
    GLToy_Array< GLToy_String > xLines = szData.Split( '\n' );
    GLToy_Iterate( GLToy_String, xIterator, &xLines )
    {
        GLToy_String& szLine = xIterator.Current();
        szLine.TrimTrailingWhiteSpace();
        szLine.TrimLeadingWhiteSpace();
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
                            if( auIndices[ u ] )
                            {
                                --auIndices[ u ];
                            }
                        }
                    }

                    pxModel->AppendFaceMaterial( uCurrentMaterial );
                    pxModel->AppendFaceVertex( auIndices[ 0 ], auIndices[ 1 ], auIndices[ 2 ] );
                }
                break;
            }

            case 'm':
            {
                if( szLine.Left( 6 ) == "mtllib" )
                {
                    // load mtl file
                    GLToy_String szPath = szLine.Right( szLine.GetLength() - 7 );
                    szPath.TrimLeadingWhiteSpace();
                    szPath.TrimTrailingWhiteSpace();

                    // TODO: does the path need adjusting? probably...
                    GLToy_TextFile xFile( GLToy_String( "Models/" ) + szPath );
                    GLToy_String szMtlData = xFile.GetString();

                    // now parse the mtl file - which is somewhat like the obj
                    //GLToy_Vector_3 xCurrentAmbient( GLToy_Maths::ZeroVector3 );
                    GLToy_Vector_3 xCurrentDiffuse( GLToy_Maths::ZeroVector3 );
                    GLToy_Vector_3 xCurrentSpecular( GLToy_Maths::ZeroVector3 );
                    float fCurrentExponent = 32.0f;

                    GLToy_Array< GLToy_String > xMtlLines = szMtlData.Split( '\n' );
                    GLToy_Iterate( GLToy_String, xIterator, &xMtlLines )
                    {
                        GLToy_String& szMtlLine = xIterator.Current();
                        szMtlLine.TrimTrailingWhiteSpace();
                        szMtlLine.TrimLeadingWhiteSpace();
                        if( szMtlLine.IsEmpty() )
                        {
                            continue;
                        }

                        // TODO: "Tr"/"d" for transparency as a float
                        // TODO: the various map commands for texture maps
                        switch( szMtlLine[ 0 ] )
                        {
                            case 'K':
                            {
                                switch( szMtlLine[ 1 ] )
                                {
                                    case 'a':
                                    {
                                        // TODO: ambient colour
                                        break;
                                    }
                                    
                                    case 'd':
                                    {
                                        // diffuse
                                        // Kd 1.1 2.2 3.3
                                        GLToy_Array< GLToy_String > xValues = szMtlLine.Right( szMtlLine.GetLength() - 3 ).Split( ' ' );
                
                                        if( xValues.GetCount() < 3 )
                                        {
                                            continue;
                                        }

                                        xCurrentDiffuse = GLToy_Vector_3( xValues[ 0 ].ExtractFloat(), xValues[ 1 ].ExtractFloat(), xValues[ 2 ].ExtractFloat() );
                                        break;
                                    }

                                    case 's':
                                    {
                                        // specular
                                        // Ks 1.1 2.2 3.3
                                        GLToy_Array< GLToy_String > xValues = szMtlLine.Right( szMtlLine.GetLength() - 3 ).Split( ' ' );
                
                                        if( xValues.GetCount() < 3 )
                                        {
                                            continue;
                                        }

                                        xCurrentSpecular = GLToy_Vector_3( xValues[ 0 ].ExtractFloat(), xValues[ 1 ].ExtractFloat(), xValues[ 2 ].ExtractFloat() );
                                        break;
                                    }
                                }
                                break;
                            }

                            case 'N':
                            {
                                switch( szMtlLine[ 1 ] )
                                {
                                    case 's':
                                    {
                                        // specular exponent
                                        // Ns 1.234
                                        fCurrentExponent = szMtlLine.Right( szMtlLine.GetLength() - 3 ).ExtractFloat();
                                        break;
                                    }
                                }
                                break;
                            }

                            case 'n':
                            {
                                if( szMtlLine.Left( 6 ) == "newmtl" )
                                {
                                    // use this as a signal to finish the previous material definition
                                    if( pxModel->GetMaterials().GetCount() > 0 )
                                    {
                                        pxModel->GetMaterials().End().m_xDiffuse = xCurrentDiffuse;
                                        pxModel->GetMaterials().End().m_xSpecular = xCurrentSpecular;
                                        pxModel->GetMaterials().End().m_fSpecularPower = fCurrentExponent;
                                    }

                                    GLToy_Assert( xMaterialReferences.GetCount() == pxModel->GetMaterials().GetCount(), "These counts need to line up for the materials in the .obj to match up to the ones we load from the .mtl!" );

                                    xMaterialReferences.AddNode( xMaterialReferences.GetCount(), szMtlLine.Right( szMtlLine.GetLength() - 7 ).GetHash() );
                                    pxModel->GetMaterials().Append( GLToy_ModelMaterial_FlatMaterials() );
                                }
                                break;
                            }
                        }
                    }

                    // tidy up the last material if necessary...
                    if( pxModel->GetMaterials().GetCount() )
                    {
                        pxModel->GetMaterials().End().m_xDiffuse = xCurrentDiffuse;
                        pxModel->GetMaterials().End().m_xSpecular = xCurrentSpecular;
                        pxModel->GetMaterials().End().m_fSpecularPower = fCurrentExponent;
                    }
                }
                break;
            }

            case 'u':
            {
                if( szLine.Left( 6 ) == "usemtl" )
                {
                    // select current material from mtl file
                    GLToy_String szMaterial = szLine.Right( szLine.GetLength() - 7 );
                    szMaterial.TrimLeadingWhiteSpace();
                    szMaterial.TrimTrailingWhiteSpace();
                    u_int* puMaterial = xMaterialReferences.FindData( szMaterial.GetHash() );
                    uCurrentMaterial = puMaterial ? *puMaterial : 0;
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

    m_pxModel->UpdateStripPointers();
    return m_pxModel;
}
