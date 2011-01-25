// the plan for the buffers is...
//
// D = diffuse
// N = normals
// Sm = specular mask
// Sp = specular power
// Sf = specular fresnel term
// H = heightmap
// SD = surface depth (for subsurface effects)
// AO = ambient occlusion
//
// gl_FragData[ 0 ] = Dr Dg Db Fog
// gl_FragData[ 1 ] = Nx Ny H  AO
// gl_FragData[ 2 ] = Sm Sp Sf SD

varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;
varying vec3 xPosition;
varying vec3 xViewPosition;
varying float fFog;

uniform sampler2D DiffuseMap;

void main()
{
	gl_FragData[ 0 ] = vec4( texture2D( DiffuseMap, xTexCoord ).xyz, fFog );
	gl_FragData[ 1 ] = vec4( CompressNormal( xNormal ), 0.0, 0.0 );
	gl_FragData[ 2 ] = vec4( 0.0, 0.0, 0.0, 0.0 );
}