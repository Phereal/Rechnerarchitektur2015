#ifdef GL_ES
    precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_texCoords;
uniform sampler2D u_texture;

varying float cosThetaDiffuse;
varying float cosAlphaSpecular;

void main()

{
    vec4 lightPosition = vec4 (1,1,1,1);

    vec4 ambientLightPower = 0.1;

    float diffuseLightPower = 1;
    float specularLightPower = 1;


    gl_FragColor = v_color * texture2D(u_texture, v_texCoords) * ambientLightPower
                +clamp((v_color * diffuseLightPower * cosThetaDiffuse),0,1)
                + v_color * specularLightPower * pow(cosAlphaSpecular,3);
}