#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D al_tex;
uniform vec3 tint;
uniform float time;

varying vec4 varying_color;
varying vec2 varying_texcoord;

void main()
{
    float freq = 32.0;
    float amp = 0.02;

    vec2 pulse = sin(time - freq * varying_texcoord);
    float dist = 2.0 * length(varying_texcoord.y - 0.5);
    
    vec2 newCoord = varying_texcoord + amp * vec2(0.0, pulse.x);
    vec2 interpCoord = mix(newCoord, varying_texcoord, dist);
    vec4 newCol = texture2D(al_tex, interpCoord);

    gl_FragColor = newCol;
}