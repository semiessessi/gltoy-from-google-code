varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;
varying vec3 xPosition;
varying vec3 xViewPosition;
varying float fFog;

uniform mat4x4 xViewMatrix;
uniform mat4x4 xInverseViewMatrix;

uniform vec4 xFogParams;

void main()
{
	xTexCoord = gl_MultiTexCoord0.xy;
	
	// decode normal basis and move into view space...
	xNormal = ( vec4( ReconstructNormal( gl_MultiTexCoord1.xy ), 0.0 ) * xViewMatrix ).xyz;
	xTangent = ( vec4( ReconstructNormal( gl_MultiTexCoord1.zw ), 0.0 ) * xViewMatrix ).xyz;
	xBinormal = cross( xNormal, xTangent );
	
	xPosition = gl_Vertex.xyz;
	xViewPosition = ( gl_Vertex * xViewMatrix ).xyz;
	fFog = clamp( ( length( xViewPosition.xyz ) - xFogParams.x ) * xFogParams.y, 0.0, 1.0 );
	
	gl_Position = ftransform();
}