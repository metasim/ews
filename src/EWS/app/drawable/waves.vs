// #version 120 compatibility

// osg predefined unifoms:
//   int osg_FrameNumber; 
//   float osg_FrameTime; 
//   float osg_DeltaFrameTime;
//   mat4 osg_ViewMatrix;
//   mat4 osg_InverseViewMatrix; 

uniform float osg_FrameTime;
uniform vec2 gridSize;
uniform float heightFactor;

/**
 * Uniforms are defined in VertexHeightUpdater.h.
 */
uniform sampler2D heightMap;


/**
 * Vertex program responsible for displacing verticies based on 
 * heightmap and creating an updated vertex normal for the fragment shader.
 */
varying vec3 normal, lightDir, eyeVec;

/**
 * Compute the height deformation offset from height map texture
 * at the given location in texture coordinates.
 * @param heightMapCoord location to sample height in texture coordinates.
 * @return height displacement amount.
 */
float sampleHeight(const in vec2 heightMapCoord) {
    vec4 heightTexl = texture2D(heightMap, heightMapCoord);
    return heightTexl.x;
}

/**
 * Determine how much in the z direction the given vertex with associated
 * texture coordinate should be displaced, including any exaggeration.
 */
float computeHeightDisplacement(const in vec2 heightMapCoord) {
    // Get the desired height value at the current vertex.
    float height = sampleHeight(heightMapCoord);
    // Exaggerate height using uniform value.
    return height * heightFactor;
}

/**
 * Given a vertex and its associated texture coordinate.
 * displace it in the z direction based on the texture value.
 */
vec4 displaceVertex(const in vec4 vertex, const in vec2 heightMapCoord) {
    // Add height offset to vertex and provide OUT data for fs
    vec4 newVertex = vertex;
    newVertex.z += computeHeightDisplacement(heightMapCoord);
    return newVertex;
} 


/**
 * Shader entry point.
 */
void main(void) {
    // Really shouldn't need to compute this each time.
    vec2 normTexOffset = 1.0/gridSize;

    // First take care of passing on regular color info to fs
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    vec2 currTexCoord = gl_TexCoord[0].st;
    
    vec4 newVertex = displaceVertex(gl_Vertex, currTexCoord);
    
    // Transform location for fragment shader.
    gl_Position = gl_ModelViewProjectionMatrix * newVertex;
    
    // Compute lighting vectors for fs. Normal handled below.
    vec4 origVertex = gl_ModelViewMatrix * gl_Vertex;
    lightDir = gl_LightSource[0].position.xyz - origVertex.xyz;
    eyeVec = -origVertex.xyz;

    float hUp = computeHeightDisplacement(currTexCoord + vec2(0, normTexOffset.t));
    float hDown = computeHeightDisplacement(currTexCoord + vec2(0, -normTexOffset.t));
    float hRight = computeHeightDisplacement(currTexCoord + vec2(normTexOffset.s, 0));
    float hLeft = computeHeightDisplacement(currTexCoord + vec2(-normTexOffset.s, 0));
    
    // Create vectors to neighbors
    float currZ = newVertex.z;
    vec3 up = vec3(0, gridSize.y, currZ - hUp);
    vec3 down = vec3(0, -gridSize.y, currZ - hDown);
    vec3 right = vec3(gridSize.x, 0, currZ - hRight);
    vec3 left = vec3(-gridSize.x, 0, currZ - hLeft);
    
    // Compute the magnatude normal of the triangles neighboring vertex
    vec3 uxr = cross(up, right);
    vec3 uxl = cross(up, left);
    vec3 dxr = cross(down, right);
    vec3 dxl = cross(down, left);
    
    // Combine and normalize
    vec3 newNorm = normalize(uxr + uxl + dxr + dxl);
//    vec3 newNorm = normalize(normalize(uxr) + normalize(uxl) + normalize(dxr) + normalize(dxl));

    // Assign value to varying normal for fs
    normal = gl_NormalMatrix  * newNorm;
}
