attribute vec4 a_color;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
uniform mat4 u_worldView;
varying vec4 v_color;
varying vec2 v_texCoords;
uniform float time;

void main()
{
   v_color = a_color;
   v_texCoords = a_texCoord0;
   gl_Position =  u_worldView * a_position + vec4(sin(time+a_position.y*2),1,1,1);
}