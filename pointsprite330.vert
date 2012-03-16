#version 330

uniform mat4 viewMatrix, projMatrix;
uniform vec4 LightSource;

in vec4 position;
in vec4 color;

out vec3 light;
out vec4 Color;
out mat3 Matrix;

void main(void)
{
  Matrix = mat3(viewMatrix);
  vec4 transVec =viewMatrix*vec4(position[0],position[1],position[2], 1.0);
  Color = color;
  light = normalize(vec3(LightSource - transVec));
  gl_Position = projMatrix * transVec;
  gl_PointSize = position[3]/ gl_Position.w;
}
