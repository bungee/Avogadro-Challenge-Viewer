#version 330

in vec3 light;
in vec4 Color;
in mat3 Matrix;
out vec4 output;

void main(void)
{
  vec3 n;
  
  n.xy = gl_PointCoord * 2.0 - 1.0;
  n.z = 1.0 - dot(n.xy, n.xy);
  if (n.z < 0.0) discard;

  output = Color;  
  n.z = sqrt(n.z);
  vec3 m = normalize(transpose(inverse(Matrix)) * n);

  float d = dot(light, m);
  float s = pow(clamp(-reflect(light, m).z, 0.0, 1.0), 20.0); 
  output += vec4(vec3(mix(d, s, 0.6)),0.0);

}
