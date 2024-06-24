#version 330 core

in vec2 vPos;
in vec2 vStart;
in vec2 vEnd;
in float vThickness;

out vec4 FragColor;

void main()
{
    // Calculate the vector from the start to the end of the line
    vec2 lineDir = vEnd - vStart;
    float lineLength = length(lineDir);
    vec2 lineDirNormalized = normalize(lineDir);

    // Calculate the perpendicular vector to the line
    vec2 perpDir = vec2(-lineDirNormalized.y, lineDirNormalized.x);

    // Calculate the vector from the start of the line to the current fragment
    vec2 fragDir = vPos - vStart;

    // Project the fragment position onto the line and perpendicular direction
    float projLine = dot(fragDir, lineDirNormalized);
    float projPerp = dot(fragDir, perpDir);

    // Check if the fragment is within the bounds of the line segment
    if (projLine >= 0.0 && projLine <= lineLength && abs(projPerp) <= vThickness / 2.0)
    {
        // If within the thickened line area, set the color
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White color for the line
    }
    else
    {
        // Discard the fragment if it's outside the thickened line area
        discard;
    }
}
