#version 330 core
#define MAXLIGHTS 10

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct pointLight {
    vec3 pos;
    vec3 color;
    float intensity;
    float attenuation;
};

uniform pointLight ourlights[MAXLIGHTS];
uniform int numlights;
uniform vec3 cameraPos;
uniform sampler2D texture1;
out vec4 FragColor;

void main()
{
    vec3 result = vec3(0);
    vec3 Color = vec3(texture(texture1, TexCoord));
    float alpha = texture(texture1, TexCoord).w;

    for (int i = 0; i < numlights; i++){
        pointLight ourlight = ourlights[i];
        float att = ourlight.attenuation;
        float dist = length(ourlight.pos - FragPos);
        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * vec3(1);

        // diffuse 
        vec3 norm = (Normal);
        vec3 lightDir = normalize(ourlight.pos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuseLight = diff * ourlight.color;

        //specular
        float specularStrength = ourlight.intensity;
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8); //higher the last value stronger the reflection
        vec3 specularLight = (specularStrength * spec) * ourlight.color;

        //attenuation calculation
        float factor = 1 / (1.0 + att * dist * dist);
        result += factor * (diffuseLight + specularLight + ambient) * Color;
    }
    FragColor = vec4(result, alpha);
}