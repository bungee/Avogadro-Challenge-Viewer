#version 120

uniform float size;
varying vec3 light;

void main(void)
{
  light = normalize(vec3(gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex));
  gl_Position = ftransform();
  gl_PointSize = gl_Color.a / gl_Position.w;
  gl_FrontColor = gl_Color;
}
