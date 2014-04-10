varying vec3 lightDir;
varying vec3 normal;
varying vec3 halfViewDir;
void main()
{
    float diffuse = dot(lightDir, normal);
    float specular = pow(dot(normal, halfViewDir), 100.);

    gl_FragColor = gl_FrontMaterial.diffuse*diffuse + gl_FrontMaterial.specular*specular;
}
