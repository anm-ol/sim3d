#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec2 TexCoord;
void main()
{
    fragPos = aPos; // Pass the original position to the fragment shader
    gl_Position = projection * view * vec4(aPos, 1.0);
    TexCoord = vec2( aTexCoord.x, aTexCoord.y);
}