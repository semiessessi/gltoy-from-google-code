varying vec2 xTexCoord;

void main()
{
	xTexCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}