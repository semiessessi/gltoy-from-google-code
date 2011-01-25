varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;
varying vec3 xPosition;
varying vec3 xViewPosition;
varying float fFog;

void main()
{
	xTexCoord = gl_MultiTexCoord0.xy;
	
	// decode normal basis using stereographic projection
	xNormal = ReconstructNormal( gl_MultiTexCoord1.xy );
	xTangent = ReconstructNormal( gl_MultiTexCoord1.zw );
	xBinormal = cross( xNormal, xTangent );
	
	xPosition = gl_Vertex.xyz;
	xViewPosition = ( gl_Vertex * gl_ModelViewMatrix ).xyz;
	fFog = ( -xViewPosition.z - 400.0f ) * 0.001f ;
	
	gl_Position = ftransform();
}