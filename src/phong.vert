varying vec3 lightDir;
varying vec3 normal;
varying vec3 viewDir;
void main()
{
    viewDir = normalize(gl_ModelViewProjectionMatrix[3].xyz);
    lightDir = normalize(vec3(gl_LightSource[0].position));
    normal = normalize(gl_NormalMatrix*gl_Normal);
    gl_Position = ftransform();
}
