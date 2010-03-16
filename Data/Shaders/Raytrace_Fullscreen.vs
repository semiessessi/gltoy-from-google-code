#version 120

varying vec3 xDirection;
varying vec3 xPosition;

void main()
{
	xPosition = vec3( gl_Normal );
	xDirection = transpose( mat3( gl_ModelViewMatrix ) ) * gl_MultiTexCoord0.xyz;
	gl_Position = gl_Vertex;
}