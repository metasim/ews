/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 */

/**
 * Vertex program responsible for displacing verticies based on 
 * heightmap and creating an updated vertex normal for the fragment shader.
 */

// #version 120 compatibility

/**
 * Uniforms are set in VertexHeightUpdater.h.
 */
uniform sampler2D heightMap;
uniform vec2 gridSize;
uniform float heightFactor;

/** 
 * osg predefined unifoms
 */
//   int osg_FrameNumber; 
//   float osg_FrameTime; 
//   float osg_DeltaFrameTime;
//   mat4 osg_ViewMatrix;
//   mat4 osg_InverseViewMatrix; 

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector, debug;

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
 *  Compute lighting vectors for fs. Normal handled below.
 * Copied from http://www.lighthouse3d.com/opengl/glsl/index.php?dirlightpix
 */
void doFragmentLightingSetup() {
		/* first transform the normal into eye space and 
		normalize the result */
//		normal = normalize(gl_NormalMatrix * gl_Normal);
		/* Normalize the light's direction. Note that 
		according to the OpenGL specification, the light 
		is stored in eye space. Also since we're talking about 
		a directional light, the position field is actually direction */
		lightDir = normalize(vec3(gl_LightSource[0].position));
	
		/* Normalize the halfVector to pass it to the fragment shader */
		halfVector = normalize(gl_LightSource[0].halfVector.xyz);
					
		/* Compute the diffuse, ambient and globalAmbient terms */
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
		ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
}


/**
 * Shader entry point.
 */
void main(void) {
	debug = vec3(0.0);
    // Really shouldn't need to compute this each time.
    vec2 texOffset = 1.0/gridSize;
	texOffset.x =0.;
	texOffset.y =0.;

    // First take care of passing on regular color info to fs
    gl_FrontColor = gl_Color;
//    gl_TexCoord[0] = gl_MultiTexCoord0;
    doFragmentLightingSetup();
    vec2 currTexCoord = gl_MultiTexCoord0.st;
    
    vec4 newVertex = displaceVertex(gl_Vertex, currTexCoord);
    
    // Transform location for fragment shader.
    gl_Position = gl_ModelViewProjectionMatrix * newVertex;
//    gl_Position = ftransform();
    
    float hUp = computeHeightDisplacement(currTexCoord + vec2(0, texOffset.t));
    float hDown = computeHeightDisplacement(currTexCoord + vec2(0, -texOffset.t));
    float hRight = computeHeightDisplacement(currTexCoord + vec2(texOffset.s, 0));
    float hLeft = computeHeightDisplacement(currTexCoord + vec2(-texOffset.s, 0));
//	debug.r = hUp;
//	hUp = val;
//	hDown = -val;
//	hRight = val;
//	hLeft = -val;

    // Create vectors to neighbors
    float currZ = newVertex.z;
    vec3 up = vec3(0, gridSize.y, currZ - hUp);
    vec3 down = vec3(0, -gridSize.y, currZ - hDown);
    vec3 right = vec3(gridSize.x, 0, currZ - hRight);
    vec3 left = vec3(-gridSize.x, 0, currZ - hLeft);

    // Compute the magnatude normal of the triangles neighboring vertex
    vec3 uxl = cross(up, left);
    vec3 lxd = cross(left, down);
    vec3 dxr = cross(down, right);
    vec3 rxu = cross(right, up);
	
    
    // Combine and normalize
    vec3 newNorm = uxl + lxd + dxr + rxu + gl_Normal;
	// newNorm = gl_Normal;

    // Assign value to varying normal for fs
    normal = normalize(gl_NormalMatrix  * newNorm);
}
