#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 localPos;
 
 struct pointLight {
	vec3 pos;
	vec3 color;
	float intensity;
    float attenuation;
};

uniform pointLight ourlight;
uniform float light;
uniform vec3 cameraPos;

void main()
{
    vec3 objectColor = vec3(0.45f,0.6f,1.0f);
    float att = ourlight.attenuation;
    float dist = length(ourlight.pos - localPos);
    // ambient

    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * vec3(1);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(ourlight.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * ourlight.color;

    //specular
    float specularStrength = ourlight.intensity;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 25); //higher the last value stronger the reflection
    vec3 specularLight = (specularStrength * spec) * ourlight.color;  

    //attenuation calculation
    float factor = 1/(1.0 + att * dist);
    if(light == 1.0f)
    {
        vec3 diffuse = factor * diff * vec3(light) + ambient;
        FragColor = vec4(diffuse,0.7f);
    }
    else
    {
    
        vec3 result = factor * (diffuseLight + specularLight + ambient) * objectColor;
        FragColor = vec4(result, 1.0);
    }
} 

