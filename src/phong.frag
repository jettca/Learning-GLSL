varying vec3 lightDir;
varying vec3 normal;
varying vec3 viewDir;
void main()
{
    float diffuse = dot(lightDir, normal);
    vec3 reflectDir = normalize(reflect(lightDir, normal));
    float specular = pow(dot(reflectDir, viewDir), 100.);

    gl_FragColor = gl_FrontMaterial.diffuse*diffuse + gl_FrontMaterial.specular*specular;
}
