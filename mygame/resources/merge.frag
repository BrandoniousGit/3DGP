uniform sampler2D u_Texture;
uniform sampler2D u_Texture1;

varying vec2 v_TexCoord;

void main()
{
  vec4 texA = texture2D(u_Texture, v_TexCoord);
  vec4 texB = texture2D(u_Texture1, v_TexCoord);

  vec4 rtn = vec4(
    min(texA.x + texB.x, 1.0),
    min(texA.y + texB.y, 1.0),
    min(texA.z + texB.z, 1.0),
    min(texA.w, texB.w));

  gl_FragColor = rtn;
}

