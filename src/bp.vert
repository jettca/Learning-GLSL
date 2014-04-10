varying vec3 lightDir;
varying vec3 normal;
varying vec3 halfViewDir;
void main()
{
    vec3 viewDir = normalize(gl_ModelViewProjectionMatrix[3].xyz);
    lightDir = normalize(vec3(gl_LightSource[0].position));
    normal = normalize(gl_NormalMatrix*gl_Normal);
    halfViewDir = normalize(lightDir + viewDir);

    gl_Position = ftransform();
}
