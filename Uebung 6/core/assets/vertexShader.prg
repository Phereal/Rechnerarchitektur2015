attribute vec4 a_color;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
uniform mat4 u_worldView;
varying vec4 v_color;
varying vec2 v_texCoords;

uniform float time;
uniform vec4 v_camDirection;
attribute vec4 a_normal;



varying float cosThetaDiffuse;
varying float cosAlphaSpecular;

void main()
{
    v_color = a_color;
    v_texCoords = a_texCoord0;


    float distort = sin ( time*3 + a_position.y * 3)*0.1;

    float xDistort = distort * a_normal.x;
    float yDistort = distort * a_normal.y;
    float zDistort = distort * a_normal.z;

    //Das Licht bewegt sich und beleuchtet die Objekte von verschiedenen Seiten.
    vec4 u_lightCoords = vec4 (cos(time)*3,1,sin(time)*2,1);

    vec4 lightImpactNormal = normalize(u_lightCoords-gl_Position);

    gl_Position =  u_worldView * ( vec4(xDistort + a_position.x, yDistort +a_position.y, zDistort+a_position.z,1));

    cosThetaDiffuse =  clamp( dot(a_normal, lightImpactNormal),0,1);


    //lightReflectVector = lightImpactNormal;
    cosAlphaSpecular = dot (-v_camDirection, normalize(a_normal));   //clamp?
}