attribute vec4 a_color;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
uniform mat4 u_worldView;
varying vec4 v_color;
varying vec2 v_texCoords;

uniform float time;
attribute vec4 a_normal;

void main()
{
   v_color = a_color;
   v_texCoords = a_texCoord0;

   float distort = sin ( time + a_position.y * 8)*0.5;

   float xDistort = distort * a_normal.x;
   float yDistort = distort * a_normal.y;
   float zDistort = distort * a_normal.z;
   gl_Position =  u_worldView * a_position + vec4(xDistort, yDistort, zDistort,1);
}