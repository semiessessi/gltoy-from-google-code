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

// This file's header
#include <Core/O2M.h>

// GLToy
#include <File/GLToy_TextFile.h>
#include <File/GLToy_OBJFile.h>
#include <Model/GLToy_Model_FlatMaterials.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool O2M::Initialise()
{
    GLToy_Array< GLToy_String > aszArguments = GLToy::GetCommandLine().Split( ' ' );

	if( aszArguments.GetCount() < 1 )
	{
		GLToy_DebugOutput_Release( "Not enough command line arguments!\n" );
		return false;
	}

	GLToy_String szOBJPath = "";
	if( aszArguments[ 0 ][ 0 ] == '"' )
	{
		aszArguments[ 0 ].RemoveAt( 0 );
		for( u_int i = 0; i < aszArguments.GetCount(); ++i )
		{
			szOBJPath += aszArguments[ i ];
			if( aszArguments[ i ].End() == '"' )
			{
				szOBJPath.RemoveFromEnd();
				break;
			}
		}
	}

	GLToy_String szMapOutput = "\r\n// entity 0\r\n{\r\n\"classname\" \"worldspawn\"\r\n";
	GLToy_OBJFile xOBJFile( szOBJPath );

	const GLToy_Model_FlatMaterials* const pxModel = static_cast< GLToy_Model_FlatMaterials* >( xOBJFile.LoadModel() );

	for( u_int i = 0; i < pxModel->GetCount(); ++i )
	{
		const GLToy_ModelStrip_FlatMaterials& xStrip = static_cast< const GLToy_ModelStrip_FlatMaterials& >( ( *pxModel )[ i ] );
		
		GLToy_Vector_3 xNormal = GLToy_Maths::ZeroVector3;
		for( u_int j = 0; j < xStrip.GetVertexCount(); ++j )
		{
			xNormal += xStrip.GetNormal( j );
		}

		xNormal /= static_cast< float >( xStrip.GetVertexCount() );
		xNormal.Normalise();

		// work out verts for each plane
		GLToy_Vector_3 xPositions[ 3 ] =
		{
			64.0f * xStrip.GetVertex( 0 ),
			64.0f * xStrip.GetVertex( 1 ),
			64.0f * xStrip.GetVertex( 2 )
		};

		GLToy_Vector_3 xPositionsExtruded[ 3 ] =
		{
			xPositions[ 0 ] - 2.0f * xNormal,
			xPositions[ 1 ] - 2.0f * xNormal,
			xPositions[ 2 ] - 2.0f * xNormal,
		};

		// write out triangular prism brush
		GLToy_String szBrushFace = "";

		// "top"
		szBrushFace.SetToFormatString(	"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"c_met5_1 0 0 0 1.0 1.0\r\n", // ( texture offsets, rotation and scales )
										xPositions[ 0 ].x, xPositions[ 0 ].y, xPositions[ 0 ].z ,
										xPositions[ 1 ].x, xPositions[ 1 ].y, xPositions[ 1 ].z,
										xPositions[ 2 ].x, xPositions[ 2 ].y, xPositions[ 2 ].z );
		szMapOutput += szBrushFace;

		// "bottom"
		szBrushFace.SetToFormatString(	"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"c_met5_1 0 0 0 1.0 1.0\r\n", // ( texture offsets, rotation and scales )
										xPositionsExtruded[ 2 ].x, xPositionsExtruded[ 2 ].y, xPositionsExtruded[ 2 ].z ,
										xPositionsExtruded[ 1 ].x, xPositionsExtruded[ 1 ].y, xPositionsExtruded[ 1 ].z,
										xPositionsExtruded[ 0 ].x, xPositionsExtruded[ 0 ].y, xPositionsExtruded[ 0 ].z );
		szMapOutput += szBrushFace;

		// side 0 - 1
		szBrushFace.SetToFormatString(	"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"c_met5_1 0 0 0 1.0 1.0\r\n", // ( texture offsets, rotation and scales )
										xPositions[ 0 ].x, xPositions[ 0 ].y, xPositions[ 0 ].z ,
										xPositionsExtruded[ 0 ].x, xPositionsExtruded[ 0 ].y, xPositionsExtruded[ 0 ].z,
										xPositionsExtruded[ 1 ].x, xPositionsExtruded[ 1 ].y, xPositionsExtruded[ 1 ].z );
		szMapOutput += szBrushFace;

		// side 1 - 2
		szBrushFace.SetToFormatString(	"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"c_met5_1 0 0 0 1.0 1.0\r\n", // ( texture offsets, rotation and scales )
										xPositions[ 1 ].x, xPositions[ 1 ].y, xPositions[ 1 ].z ,
										xPositionsExtruded[ 1 ].x, xPositionsExtruded[ 1 ].y, xPositionsExtruded[ 1 ].z,
										xPositionsExtruded[ 2 ].x, xPositionsExtruded[ 2 ].y, xPositionsExtruded[ 2 ].z );
		szMapOutput += szBrushFace;

		for( u_int i = 3; i < ( xStrip.GetVertexCount() - 1 ); ++i )
		{
			const int xN = xStrip.GetVertexCount() - 1;
			GLToy_Vector_3 xPosition = xStrip.GetVertex( xN );
			GLToy_Vector_3 xExtrudedPosition = xStrip.GetVertex( xN ) - 2.0f * xNormal;
			GLToy_Vector_3 xExtrudedPosition2 = xStrip.GetVertex( xN + 1 ) - 2.0f * xNormal;
			szBrushFace.SetToFormatString(	"( %.0f, %.0f, %.0f ) "
											"( %.0f, %.0f, %.0f ) "
											"( %.0f, %.0f, %.0f ) "
											"c_met5_1 0 0 0 1.0 1.0\r\n", // ( texture offsets, rotation and scales )
											xPosition.x, xPosition.y, xPosition.z ,
											xExtrudedPosition.x, xExtrudedPosition.y, xExtrudedPosition.z,
											xExtrudedPosition2.x, xExtrudedPosition2.y, xExtrudedPosition2.z );
			szMapOutput += szBrushFace;
		}

		const int xN = xStrip.GetVertexCount() - 1;
		GLToy_Vector_3 xPosition = xStrip.GetVertex( xN );
		GLToy_Vector_3 xExtrudedPosition = xStrip.GetVertex( xN ) - 2.0f * xNormal;
		// side n - 0
		szBrushFace.SetToFormatString(	"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"( %.0f, %.0f, %.0f ) "
										"c_met5_1 0 0 0 1.0 1.0\r\n", // ( texture offsets, rotation and scales )
										xPosition.x, xPosition.y, xPosition.z ,
										xExtrudedPosition.x, xExtrudedPosition.y, xExtrudedPosition.z,
										xPositionsExtruded[ 0 ].x, xPositionsExtruded[ 0 ].y, xPositionsExtruded[ 0 ].z );
		szMapOutput += szBrushFace;
	}

	szMapOutput += "\r\n}\r\n";

	GLToy_String szMapPath = szOBJPath;
	szMapPath.RemoveFromEnd( 4 ); // ".obj"
	szMapPath += ".map";
	GLToy_TextFile xMapFile( szMapPath );

	GLToy_BitStream xBitStream;
	char* pcData = szMapOutput.CreateANSIString();
	for( u_int u = 0; u < szMapOutput.GetLength(); ++u )
	{
		xBitStream << pcData[ u ];
	}

	xMapFile.WriteFromBitStream( xBitStream );

    return true;
}

void O2M::Shutdown()
{
}

void O2M::Update()
{
}
