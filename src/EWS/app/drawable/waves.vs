
// osg predefined unifoms:
//   int osg_FrameNumber; 
//   float osg_FrameTime; 
//   float osg_DeltaFrameTime;
//   mat4 osg_ViewMatrix;
//   mat4 osg_InverseViewMatrix; 


/**
 * Vertex program responsible for displacing verticies based on 
 * heightmap and creating an updated vertex normal for the fragment shader.
 * Uniforms are defined in VertexHeightUpdater.h.
 */
varying vec3 normal, lightDir, eyeVec;
uniform vec2 gridSize;
uniform sampler2D heightMap;
uniform float osg_FrameTime;
void main(void) {
	const float offset = 0.01;
    	// First take care of passing on 
    	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec2 currTexCoord = gl_TexCoord[0].st;
	
	vec4 heightTexl = texture2D(heightMap, currTexCoord);
    	float height = length(heightTexl) * 5.0;

	// Use matrix as a cheap array of vectors to set up
	// offsets around current texture coordinate. Last (w) coordinate is ignored.
	mat4 neighbors = mat4(
		vec3(0, offset, 0), 0,
		vec3(0, -offset, 0), 0,
		vec3(-offset, 0, 0), 0,
		vec3(offset, 0, 0), 0
	);


	for(int i = 0; i < 4; i++) {
		// Set the neighbors z component to the magnitude of the neighbor's texel.
		neighbors[i].z = length(texture2D(heightMap, currTexCoord + neighbors[i].st));
	}
    
    vec3 dir1 = neighbors[0].xyz - neighbors[1].xyz;
    vec3 dir2 = neighbors[2].xyz - neighbors[3].xyz;

	normal = gl_NormalMatrix  * normalize(cross(dir1,dir2));

	vec4 origVertex = gl_ModelViewMatrix * gl_Vertex;

	lightDir = gl_LightSource[0].position.xyz - origVertex.xyz;
	eyeVec = -origVertex.xyz;

    	vec4 newVertex = gl_Vertex;
    	//height = sin(osg_FrameTime) * 5.0;
    	newVertex.z += height;

    	gl_Position = gl_ModelViewProjectionMatrix * newVertex;
    
}
