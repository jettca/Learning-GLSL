varying vec3 lightDir;
varying vec3 normal;
varying vec4 color;
void main()
{
    float diffuse = dot(normalize(lightDir), normal);
    vec3 reflectDir = reflect(lightDir, normal);
    float specular = pow(dot(normalize(reflectDir), normal), 100.);

    gl_FragColor = gl_FrontMaterial.diffuse*diffuse + gl_FrontMaterial.specular*specular;
}
