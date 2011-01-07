varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;

vec3 InverseStereographicProjection( vec2 xProjected )
{
	float fSumOfSquares = dot( xProjected, xProjected );
	return vec3( 2.0 * xProjected.x, 2.0 * xProjected.y, fSumOfSquares - 1.0 ) / ( 1.0 + fSumOfSquares );
}

vec2 StereographicProjection( vec3 xNormalised )
{
	return xNormalised.xy / ( 1.0 - xNormalised.z );
}

void main()
{
	xTexCoord = gl_MultiTexCoord0.xy;
	
	// decode normal basis using stereographic projection
	xNormal = InverseStereographicProjection( gl_MultiTexCoord1.xy );
	xTangent = InverseStereographicProjection( gl_MultiTexCoord1.zw );
	xBinormal = cross( xNormal, xTangent );
	
	gl_Position = ftransform();
}