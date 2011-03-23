/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

// GLToy
#include <Core/Data Structures/GLToy_BSPTree.h>
#include <Core/Data Structures/GLToy_Pair.h>
#include <File/BSP/GLToy_BSP38_Types.h>
#include <File/GLToy_3DSFile.h>
#include <File/GLToy_OBJFile.h>
#include <Maths/GLToy_Maths.h>
#include <Model/GLToy_Model_3DS.h>
#include <Model/GLToy_Model_FlatMaterials.h>
#include <Render/GLToy_Render.h>
#include <String/GLToy_String.h>

// C
#include <stdio.h>

// External
#include "../../../stb_image/stb_image.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_PLATFORM_WIN32
    #pragma comment( lib, "GLToy" )
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

enum TriangleType
{
    TT_POSITIVE,
    TT_NEGATIVE,
    TT_COINCIDENT,
    TT_SPANNING
};

struct Triangle
{
    GLToy_Vector_3 m_axVertices[ 3 ];
    GLToy_Vector_2 m_axUVs[ 3 ];
    GLToy_Plane m_xPlane;
    
    void CalculatePlane()
    {
        const GLToy_Vector_3 xNormal = m_axVertices[ 0 ].Cross( m_axVertices[ 1 ] );
        m_xPlane = GLToy_Plane( xNormal, xNormal * m_axVertices[ 0 ] );
    }

    void CalculatePlane( const GLToy_Vector_3& xNormal )
    {
        m_xPlane = GLToy_Plane( xNormal, xNormal * m_axVertices[ 0 ] );
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO: some small tolerance for error...
TriangleType ClassifyTriangle( const Triangle& xTriangle, const GLToy_Plane& xPlane )
{
    const float fV1D = xPlane.SignedDistance( xTriangle.m_axVertices[ 0 ] );
    const float fV2D = xPlane.SignedDistance( xTriangle.m_axVertices[ 1 ] );
    const float fV3D = xPlane.SignedDistance( xTriangle.m_axVertices[ 2 ] );

    if( ( fV1D > 0.0f ) && ( fV2D > 0.0f ) && ( fV3D > 0.0f ) )
    {
        return TT_POSITIVE;
    }

    if( ( fV1D > 0.0f ) && ( fV2D > 0.0f ) && ( fV3D > 0.0f ) )
    {
        return TT_NEGATIVE;
    }

    if( ( fV1D == 0.0f ) && ( fV2D == 0.0f ) && ( fV3D == 0.0f ) )
    {
        return TT_COINCIDENT;
    }

    return TT_SPANNING;
}

GLToy_Pair< GLToy_Array< Triangle > > TriangleSplit( const Triangle& xTriangle, const GLToy_Plane& xPlane )
{
    GLToy_Pair< GLToy_Array< Triangle > > xSplit;
    u_int uIndexSplit = 0;
    u_int uIndexPos = 1;
    u_int uIndexNeg = 2;

    // there are two cases - either the plane splits the triangle in half into two...
    bool bCaseOne = false;
    if( xPlane.SignedDistance( xTriangle.m_axVertices[ 0 ] ) == 0.0f )
    {
        bool bCondition = xPlane.SignedDistance( xTriangle.m_axVertices[ 1 ] ) >= 0.0f;
        uIndexPos = bCondition ? 1 : 2;
        uIndexNeg = bCondition ? 2 : 1;
        bCaseOne = true;
    }

    if( xPlane.SignedDistance( xTriangle.m_axVertices[ 1 ] ) == 0.0f )
    {
        bool bCondition = xPlane.SignedDistance( xTriangle.m_axVertices[ 0 ] ) >= 0.0f;
        uIndexPos = bCondition ? 0 : 2;
        uIndexNeg = bCondition ? 2 : 0;
        uIndexSplit = 1;
        bCaseOne = true;
    }

    if( xPlane.SignedDistance( xTriangle.m_axVertices[ 2 ] ) == 0.0f )
    {
        bool bCondition = xPlane.SignedDistance( xTriangle.m_axVertices[ 0 ] ) >= 0.0f;
        uIndexPos = bCondition ? 0 : 1;
        uIndexNeg = bCondition ? 1 : 0;
        uIndexSplit = 2;
        bCaseOne = true;
    }

    if( bCaseOne )
    {
        float fParameter = 0.0f;
        GLToy_Ray::FromPositions( xTriangle.m_axVertices[ uIndexPos ], xTriangle.m_axVertices[ uIndexNeg ] ).IntersectsWithPlane( xPlane, &fParameter );
        fParameter /= ( xTriangle.m_axVertices[ uIndexPos ] - xTriangle.m_axVertices[ uIndexNeg ] ).Magnitude();

        GLToy_Vector_3 xNewVert = GLToy_Maths::Lerp( xTriangle.m_axVertices[ uIndexPos ], xTriangle.m_axVertices[ uIndexNeg ], fParameter );
        GLToy_Vector_2 xNewUV = GLToy_Maths::Lerp( xTriangle.m_axUVs[ uIndexPos ], xTriangle.m_axUVs[ uIndexNeg ], fParameter );

        Triangle xPositive;
        Triangle xNegative;

        // TODO: sort winding out
        xPositive.m_axVertices[ 0 ] = xTriangle.m_axVertices[ uIndexSplit ];
        xPositive.m_axVertices[ 1 ] = xNewVert;
        xPositive.m_axVertices[ 2 ] = xTriangle.m_axVertices[ uIndexPos ];
        xPositive.m_axUVs[ 0 ] = xTriangle.m_axUVs[ uIndexSplit ];
        xPositive.m_axUVs[ 1 ] = xNewUV;
        xPositive.m_axUVs[ 2 ] = xTriangle.m_axUVs[ uIndexPos ];
        xPositive.CalculatePlane();

        xNegative.m_axVertices[ 0 ] = xTriangle.m_axVertices[ uIndexSplit ];
        xNegative.m_axVertices[ 1 ] = xNewVert;
        xNegative.m_axVertices[ 2 ] = xTriangle.m_axVertices[ uIndexNeg ];
        xNegative.m_axUVs[ 0 ] = xTriangle.m_axUVs[ uIndexSplit ];
        xNegative.m_axUVs[ 1 ] = xNewUV;
        xNegative.m_axUVs[ 2 ] = xTriangle.m_axUVs[ uIndexNeg ];
        xNegative.CalculatePlane();

        xSplit.First().Append( xPositive );
        xSplit.Second().Append( xNegative );

        return xSplit;
    }

    // ... or it splits it into a quad and a triangle

    return xSplit;
}

void BuildTree( GLToy_BSPNode< GLToy_Array< Triangle > >* const pxTree, const GLToy_Array< Triangle >& xSourceTriangles )
{
    pxTree->SetPlane( xSourceTriangles[ 0 ].m_xPlane );
    pxTree->SetData( new GLToy_Array< Triangle >() );
    pxTree->GetData()->Append( xSourceTriangles[ 0 ] );

    GLToy_Array< Triangle > xPositives;
    GLToy_Array< Triangle > xNegatives;

    for( u_int u = 1; u < xSourceTriangles.GetCount(); ++u )
    {
        const Triangle& xTriangle = xSourceTriangles[ u ];
        const TriangleType eType = ClassifyTriangle( xTriangle, pxTree->GetPlane() );
        switch( eType )
        {
            case TT_COINCIDENT:
            {
                pxTree->GetData()->Append( xTriangle );
                break;
            }

            case TT_POSITIVE:
            {
                if( !pxTree->GetPositiveNode() )
                {
                    pxTree->AddPositiveLeaf( new GLToy_Array< Triangle >() );
                }
                xPositives.Append( xTriangle );
                break;
            }

            case TT_NEGATIVE:
            {
                if( !pxTree->GetNegativeNode() )
                {
                    pxTree->AddNegativeLeaf( new GLToy_Array< Triangle >() );
                }
                xNegatives.Append( xTriangle );
                break;
            }

            case TT_SPANNING:
            {
                GLToy_Pair< GLToy_Array< Triangle > > xSplit = TriangleSplit( xTriangle, pxTree->GetPlane() );
                xPositives.Append( xSplit.First() );
                xNegatives.Append( xSplit.Second() );
                break;
            }
        }
    }

    if( pxTree->GetPositiveNode() )
    {
        BuildTree( pxTree->GetPositiveNode(), xPositives );
    }
    
    if( pxTree->GetNegativeNode() )
    {
        BuildTree( pxTree->GetNegativeNode(), xNegatives );
    }
}

void GenerateBSPData( const GLToy_Array< Triangle >& xSourceTriangles, GLToy_BitStream& xBitStream, const u_int uVersion = 38, const u_int uHeaderBytes = GLToy_HeaderBytes( "IBSP" ) )
{
    xBitStream << uHeaderBytes;
    xBitStream << uVersion ;

    // generate BSP
    GLToy_BSPNode< GLToy_Array< Triangle > >* pxHead =  new GLToy_BSPNode< GLToy_Array< Triangle > >( xSourceTriangles[ 0 ].m_xPlane.GetNormal(), xSourceTriangles[ 0 ].m_xPlane.GetDistance() );

    BuildTree( pxHead, xSourceTriangles );

    GLToy_BSPTree< GLToy_Array< Triangle > > xBSPTree;
    xBSPTree.SetToNodePointer( pxHead );

    // write out data
    printf( "Converting data to BSP v%d format...\r\n", uVersion );
    
    GLToy_BSP38_LumpDirectory xLumpDirectory;

    xBitStream << xLumpDirectory;

    //for( u_int u = 0; u < xPlanes.GetCount(); ++u )
    //{
    //    GLToy_BSP38_Plane xPlane;
    //    xPlane.m_xPlane = xPlanes[ u ];
    //    xBitStream << xPlane;
    //}

    // TODO...

    // clean up

    xBSPTree.DeleteAllIndirect();
}

GLToy_Array< Triangle > TriangleListFrom3DS( const char* const szPath )
{
    GLToy_Array< Triangle > xTriangleList;

    GLToy_3DSFile xFile( szPath );

    GLToy_Model_3DS* const pxModel = static_cast< GLToy_Model_3DS* >( xFile.LoadModel() );

    if( pxModel )
    {
        GLToy_ConstIterate( GLToy_3DS_Object, xObject, pxModel->Get3DSObjects() )
            Triangle xCurrentTriangle;
            u_int uVertex = 0;
            GLToy_ConstIterate( u_int, uIndex, xObject.m_xIndices )
                xCurrentTriangle.m_axVertices[ uVertex ] = xObject.m_xVertices[ uIndex ];
                xCurrentTriangle.m_axUVs[ uVertex ] = GLToy_Maths::ZeroVector2;
                ++uVertex;
                uVertex %= 3;
                if( uVertex == 0 )
                {
                    xCurrentTriangle.CalculatePlane();
                    xTriangleList.Append( xCurrentTriangle );
                }
            GLToy_Iterate_End;
        GLToy_Iterate_End;
    }

    return xTriangleList;
}

GLToy_Array< Triangle > TriangleListFromOBJ( const char* const szPath )
{
    GLToy_Array< Triangle > xTriangleList;

    GLToy_OBJFile xFile( szPath );

    GLToy_Model_FlatMaterials* const pxModel = static_cast< GLToy_Model_FlatMaterials* >( xFile.LoadModel() );

    if( pxModel )
    {
        for( u_int u = 0; u < pxModel->GetCount(); ++u )
        {
            GLToy_ModelStrip_FlatMaterials* const pxStrip = static_cast< GLToy_ModelStrip_FlatMaterials* >( &( ( *pxModel )[ u ] ) );
            // each strip is a polygon
            Triangle xCurrentTriangle;
            xCurrentTriangle.m_axVertices[ 0 ] = pxStrip->GetVertex( 0 );
            xCurrentTriangle.m_axUVs[ 0 ] = pxStrip->GetUV( 0 );
            for( u_int v = 1; v < ( pxStrip->GetVertexCount() - 1 ); ++v )
            {
                xCurrentTriangle.m_axVertices[ 1 ] = pxStrip->GetVertex( v );
                xCurrentTriangle.m_axVertices[ 2 ] = pxStrip->GetVertex( v + 1 );
                xCurrentTriangle.m_axUVs[ 1 ] = pxStrip->GetUV( v );
                xCurrentTriangle.m_axUVs[ 2 ] = pxStrip->GetUV( v + 1 );

                xCurrentTriangle.CalculatePlane( /*pxStrip->GetNormal( v )*/ );

                xTriangleList.Append( xCurrentTriangle );
            }
        }
    }

    return xTriangleList;
}

int GLToy_ForceCDecl main( const int iArgumentCount, const char* const* const pszArguments )
{
    //GLToy::SilentEntryPoint();

    printf( "GLToy BSP Tool\r\n" );

    // parse parameters
    bool bHelp = ( iArgumentCount <= 2 );

    if( bHelp )
    {
        printf( "Usage:\r\n" );
        printf( "  BSP <input-file> <output-file>\r\n" );
        printf( "\r\n" );
        printf( "By default this will try to convert a mesh to a v38 BSP file, more options to come later\r\n" );

        return 0;
    }

    const char* const szInPath = pszArguments[ iArgumentCount - 2 ];
    const char* const szOutPath = pszArguments[ iArgumentCount - 1 ];

    printf( "Loading input file %s...\r\n", szInPath );

    GLToy_Array< Triangle > xTriangleList;
    const GLToy_String szIn( szInPath );
    if( szIn.EndsWith( ".3ds" ) )
    {
        xTriangleList = TriangleListFrom3DS( szInPath );
    }
    else if( szIn.EndsWith( ".bsp" ) )
    {
        printf( "BSP not yet supported\r\n" );
    }
    else if( szIn.EndsWith( ".lwo" ) )
    {
        printf( "LWO not yet supported\r\n" );
    }
    else
    {
        if( !szIn.EndsWith( ".obj" ) )
        {
            printf( "Assuming OBJ format...\r\n" );
        }

        xTriangleList = TriangleListFromOBJ( szInPath );
    }

    if( xTriangleList.GetCount() == 0 )
    {
        printf( "No triangles loaded.\r\n" );
        return 0;
    }
    else
    {
        printf( "%d triangles loaded.\r\n", xTriangleList.GetCount() );
    }

    GLToy_BitStream xBitStream;

    GenerateBSPData( xTriangleList, xBitStream );

    printf( "Writing output file %s...\r\n", szOutPath );

    GLToy_String szOut( szOutPath );
    if( !szOut.EndsWith( ".bsp" ) )
    {
        szOut += ".bsp";
    }

    GLToy_File( szOut ).WriteFromBitStream( xBitStream );

    //GLToy::SilentShutdown();

    return 0;
}

bool GLToy::Project_Initialise()
{
	return true;
}

void GLToy::Project_Shutdown()
{
}

void GLToy::Project_Update()
{
}

bool GLToy_Render::Project_Initialise()
{
	return true;
}

void GLToy_Render::Project_Shutdown()
{
}

void GLToy_Render::Project_Render()
{
}
