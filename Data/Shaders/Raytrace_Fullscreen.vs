varying vec3 xDirection;
varying vec3 xPosition;

void main()
{
	xPosition = vec3( gl_Normal );
	xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix );
	gl_Position = gl_Vertex;
}