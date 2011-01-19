varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;
varying vec3 xPosition;
varying vec3 xViewPosition;

void main()
{
	xTexCoord = gl_MultiTexCoord0.xy;
	
	// decode normal basis using stereographic projection
	xNormal = ReconstructNormal( gl_MultiTexCoord1.xy );
	xTangent = ReconstructNormal( gl_MultiTexCoord1.zw );
	xBinormal = cross( xNormal, xTangent );
	
	xPosition = gl_Vertex.xyz;
	xViewPosition = ( gl_Vertex * gl_ModelViewMatrix ).xyz;
	
	gl_Position = ftransform();
}