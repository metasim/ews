
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
    vec3 origNormal = gl_NormalMatrix * gl_Normal;
	vec4 origVertex = gl_ModelViewMatrix * gl_Vertex;
    
    gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	lightDir = gl_LightSource[0].position.xyz - origVertex.xyz;
    eyeVec = -origVertex.xyz;
    normal = origNormal;
    vec4 tex = texture2D(heightMap, gl_TexCoord[0].st);

    vec4 newVertex = gl_Vertex;
    float height = length(tex) * 5.0;
    //height = sin(osg_FrameTime) * 5.0;
    newVertex.z += height;

    gl_Position = gl_ModelViewProjectionMatrix * newVertex;
    
}
