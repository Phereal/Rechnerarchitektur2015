attribute vec4 a_color;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
uniform mat4 u_worldView;
varying vec4 v_color;
varying vec2 v_texCoords;

uniform float time;
attribute vec4 a_normal;

varying float cosThetaDiffuse;

varying float cosThetaSpecular;

void main()
{
   v_color = a_color;
   v_texCoords = a_texCoord0;

   float distort = sin ( time*4 + a_position.y * 4)*0.1;

   float xDistort = distort * a_normal.x;
   float yDistort = distort * a_normal.y;
   float zDistort = distort * a_normal.z;

   vec4 old_gl_Position = gl_Position;

   gl_Position =  u_worldView * ( vec4(xDistort + a_position.x, yDistort +a_position.y, zDistort+a_position.z,1));

   vec4 new_a_normal = vec4 (0, distort * 0.1,0,0) - a_normal;


    //Das Licht bewegt sich und beleuchtet die Objekte von verschiedenen Seiten.
    vec4 u_lightCoords = vec4 (cos(time)*3,1,sin(time)*2,1);

    cosThetaDiffuse =  ( dot(a_normal, u_lightCoords));

}