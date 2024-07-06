#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = aPos; // Pass the original position to the fragment shader
    Normal = normalize(aNormal);
    gl_Position = projection * view * vec4(aPos, 1.0);
    TexCoord = vec2( aTexCoord.x, aTexCoord.y);
}