//
///**
// * This pixel shader is meant to be minimal since the vertex shader is
// * the focus of the sample.
// */
//float4 pixelShaderFunction(PixelShaderInput input) : COLOR {
//  float3 p = input.objectPosition;          // The point in question.
//  float3 l = normalize(lightWorldPos - p);  // Unit-length vector toward light.
//  float3 n = normalize(input.normal);       // Unit-length normal vector.
//  float3 v = normalize(cameraWorldPos - p); // Unit-length vector toward camera.
//  float3 r = normalize(-reflect(v, n));     // Reflection of v across n.
//
//  float3 q = (lightWorldPos - p);
//  float ldotr = dot(r, l);
//  float specular = clamp(ldotr, 0, 1) /
//      (1 + length(q - length(q) * ldotr * r));
//
//  return diffuseColor * dot(n, l) + specular;
//}

varying vec3 normal, lightDir, eyeVec;
void main (void) {
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
    gl_FragColor = final_color;
}
