
// osg predefined unifoms:
//   int osg_FrameNumber; 
//   float osg_FrameTime; 
//   float osg_DeltaFrameTime;
//   mat4 osg_ViewMatrix;
//   mat4 osg_InverseViewMatrix; 



/** approximate normal recalculation with 
 * n' = normalize( (J(v)*t) cross (J(v)*b))
 * where J(v) is the jacobian matrix for position v of our surface deformer:
 *     f(x, y, z) = (x, y, z + h)
 * t is the vertex tangent vector, b the binormal vertex vector.
 * @see http://http.developer.nvidia.com/GPUGems/gpugems_ch42.html
 * @see http://www.ozone3d.net/tutorials/mesh_deformer_p2.php
 * @pos vertex position
 * @tangent tangent to the 
 */
vec3 computeNormal(vec3 pos, vec3 tangent, 
                    vec3 binormal, 
                    float phase, 
                    float freq )
{
	mat3 J;
	
	float dist = sqrt(pos.x*pos.x + pos.z*pos.z);
	float jac_coef = cos(freq*dist + phase) / (dist+0.00001);
	
	// A matrix is an array of column vectors so J[2] is 
	// the third column of J.
	
	J[0][0] = 1.0;
	J[0][1] = jac_coef * pos.x;
	J[0][2] = 0.0;
	
	J[1][0] = 0.0;
	J[1][1] = 1.0;
	J[1][2] = 0.0;

	J[2][0] = 0.0;
	J[2][1] = jac_coef * pos.z;
	J[2][2] = 1.0;
	
	vec3 u = J * tangent;
	vec3 v = J * binormal;
	
	vec3 n = cross(v, u);
	return normalize(n);
}

varying vec3 normal, lightDir, eyeVec;
attribute float height;
void main(void) {
	vec3 origNormal = gl_NormalMatrix * gl_Normal;
	vec4 origVertex = gl_ModelViewMatrix * gl_Vertex;

	lightDir = gl_LightSource[0].position.xyz - origVertex.xyz;
     eyeVec = -origVertex.xyz;
     vec4 newVertex = gl_Vertex;
     newVertex.z += height;

	normal = normalize(origNormal + normalize(newVertex - origVertex).xyz);
     gl_Position = gl_ModelViewProjectionMatrix * newVertex;
}


//uniform float4x4 world : WORLD;
//uniform float4x4 worldViewProjection : WORLDVIEWPROJECTION;
//uniform float4x4 worldInverseTranspose : WORLDINVERSETRANSPOSE;
//uniform float clock;
//uniform float3 lightWorldPos;
//uniform float3 cameraWorldPos;
//uniform float4 diffuseColor;
//
//// Input parameters for the vertex shader.
//struct VertexShaderInput {
//  float4 position : POSITION;
//  float3 normal : NORMAL;
//  float4 color : COLOR;
//};
//
//
//// Input parameters for the pixel shader (also the output parameters for the
//// vertex shader.)
//struct PixelShaderInput {
//  float4 position : POSITION;          // the position in clip space
//  float3 objectPosition : TEXCOORD0;   // the position in world space
//  float3 normal : TEXCOORD1;           // the normal in world space
//};
//
///**
// * vertexShaderFunction - The vertex shader perturbs the vertices of the plane
// * to achieve the ripples.  Then it applies the worldViewProjection matrix.
// *
// * @param input.position Position vector of vertex in object coordinates.
// * @param input.normal Normal of vertex in object coordinates.
// * @param input.color Color of vertex.
// */
//PixelShaderInput vertexShaderFunction(VertexShaderInput input) {
//  PixelShaderInput output;
//
//  // Transform position into clip space.
//  output.position = mul(input.position, worldViewProjection);
//
//  // Transform normal into world space, where we can do lighting
//  // calculations even if the world transform contains scaling.
//  output.normal = mul(input.normal, worldInverseTranspose).xyz;
//
//  // Calculate surface position in world space.
//  output.objectPosition = mul(input.position, world).xyz;
//
//  return output;
//}

