#version 330 core
in vec3 fragPos;
in vec2 TexCoord;

uniform sampler2D texture1;
out vec4 FragColor;

void main()
{
    vec3 color = fract(vec3(.3 * fragPos));
    FragColor = vec4(texture(texture1, TexCoord));
    //FragColor = vec4(color, 1.0);
}