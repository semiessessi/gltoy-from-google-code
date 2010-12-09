varying vec3 xDirection;
varying vec3 xPosition;

void main()
{
	xPosition = vec3( gl_Normal );
	// 1.10 and intel graphics cards hate this matrix-matrix constructor
    // xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix );
    xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix[ 0 ][ 0 ], gl_ModelViewMatrix[ 0 ][ 1 ], gl_ModelViewMatrix[ 0 ][ 2 ], gl_ModelViewMatrix[ 1 ][ 0 ], gl_ModelViewMatrix[ 1 ][ 1 ], gl_ModelViewMatrix[ 1 ][ 2 ], gl_ModelViewMatrix[ 2 ][ 0 ], gl_ModelViewMatrix[ 2 ][ 1 ], gl_ModelViewMatrix[ 2 ][ 2 ] );
	gl_Position = gl_Vertex;
}