uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main()
{
  vec4 rtn = texture2D(u_Texture, v_TexCoord);

  float t = 0.5;

  //if(rtn.x + rtn.y + rtn.z < 1.5)
  if(rtn.x < t || rtn.y < 0.1 && rtn.z < t)
  {
    discard;
  }

  //rtn.x = 1.0;
  //rtn.y = 1.0;
  //rtn.z = 1.0;

  rtn.x = min(rtn.x * 2.0, 1.0);
  rtn.y = min(rtn.y * 2.0, 1.0);
  rtn.z = min(rtn.z * 2.0, 1.0);

  gl_FragColor = rtn;
}

