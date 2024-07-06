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
    vec3 viewDir = normalize(cameraPos - FragPos);

    for (int i = 0; i < numlights; i++){
        pointLight ourlight = ourlights[i];
        float att = ourlight.attenuation;
        float dist = length(ourlight.pos - FragPos);
        // ambient
        float ambientStrength = 0.15;
        vec3 ambient = ambientStrength * vec3(1);

        // diffuse 
        vec3 norm = (Normal);
        vec3 lightDir = normalize(ourlight.pos - FragPos);
        float viewside = dot(viewDir, norm);
        float lightside = dot(lightDir, norm);
        float frontside = clamp(ceil(lightside * viewside), 0, 1);
        float diff = frontside * abs(dot(norm, lightDir));
        vec3 diffuseLight = diff * ourlight.color;

        //specular
        float specularStrength = ourlight.intensity;
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 12); //higher the last value stronger the reflection
        vec3 specularLight = (specularStrength * spec) * ourlight.color;

        //attenuation calculation
        float factor = 1 / (1.0 + att * dist * dist);
        result += factor * (diffuseLight + specularLight + ambient) * Color;
    }
    //result = result / (result + vec3(2));
    FragColor = vec4(result, alpha);
}