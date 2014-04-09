varying vec3 lightDir;
varying vec3 normal;
varying vec4 color;
void main()
{
    lightDir = vec3(gl_LightSource[0].position);
    normal = normalize(gl_NormalMatrix*gl_Normal);
    color = gl_Color;
    gl_Position = ftransform();
}
