varying vec3 lightDir;
varying vec3 normal;
varying vec3 halfViewDir;
varying vec2 UV;
uniform sampler2D texsampler;
void main()
{
    float diffuse = dot(lightDir, normal);
    float specular = pow(dot(normal, halfViewDir), 100.);

    vec4 color = vec4(texture2D(texsampler, UV).xyz, 1);
    gl_FragColor = color*(diffuse + specular);
}
