varying vec2 xTexCoord;

void main()
{
	xTexCoord = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}