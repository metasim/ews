

varying vec3 normal, lightDir, eyeVec, debug;
uniform sampler2D heightMap; 

/**
 * Phong lighting model from:
 * http://www.ozone3d.net/tutorials/glsl_lighting_phong.php
 * If multi-light support is needed, see:
 * http://www.geeks3d.com/20091013/shader-library-phong-shader-with-multiple-lights-glsl/
 */
vec4 phong() {
    vec4 final_color = 
        (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
        (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);
    float lambertTerm = dot(N,L);
    if(lambertTerm > 0.0) {
        final_color += gl_LightSource[0].diffuse * 
            gl_FrontMaterial.diffuse * lambertTerm;
        vec3 E = normalize(eyeVec);
        vec3 R = reflect(-L, N);
        float specular = pow( max(dot(R, E), 0.0), 
            gl_FrontMaterial.shininess );
        final_color += gl_LightSource[0].specular * 
            gl_FrontMaterial.specular * specular;
    }

    return final_color;
}


void main (void) {
	if(length(debug) > 0.) {
	    gl_FragColor.rgb = debug;
	}
    else {
        gl_FragColor = phong();
    }
    
}
