attribute vec2 a_Position;

varying vec2 v_TexCoord;

void main()
{
  gl_Position = vec4(a_Position, 0, 1);
  v_TexCoord = vec2(0, 0);

  if(a_Position.x > 0.0)
  {
    v_TexCoord.x = 1.0;
  }

  if(a_Position.y > 0.0)
  {
    v_TexCoord.y = 1.0;
  }
}

