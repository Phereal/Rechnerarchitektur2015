#ifdef GL_ES
     precision mediump float;
 #endif

 varying vec4 v_color;

 float ambient = 1;    //Allgemeine Beleuchtung aller Objekte von allen Richtungen (momentan zu hoch für Testzwecke)


 void main()
 {
     gl_FragColor = v_color * ambient;
 }