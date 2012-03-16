#version 120
varying vec3 light;

void main(void)
{
  vec3 n;
  
  n.xy = gl_PointCoord * 2.0 - 1.0;
  n.z = 1.0 - dot(n.xy, n.xy);
  if (n.z < 0.0) discard;
  
  gl_FragColor = gl_Color;
  n.z = sqrt(n.z);
  vec3 m = normalize(gl_NormalMatrix * n);

  float d = dot(light, m);
  float s = pow(clamp(-reflect(light, m).z, 0.0, 1.0), 20.0); 
  gl_FragColor.rgb += vec3(mix(d, s, 0.4));
  //gl_FragColor.a = 1.0;
}
