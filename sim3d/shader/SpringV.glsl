#version 330 core

layout(location = 0) in vec2 aPos; // Position of the vertex
uniform vec2 uStart; // Starting point of the line
uniform vec2 uEnd;   // Ending point of the line
uniform float uThickness; // Line thickness

out vec2 vPos;
out vec2 vStart;
out vec2 vEnd;
out float vThickness;

void main()
{
    vPos = aPos;
    vStart = uStart;
    vEnd = uEnd;
    vThickness = uThickness;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
