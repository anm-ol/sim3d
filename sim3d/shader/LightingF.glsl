#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
 
 struct pointLight {
	vec3 pos;
	vec3 color;
	float intensity;
};

uniform pointLight ourlight;
uniform float light;
uniform vec3 cameraPos;

void main()
{
    vec3 objectColor = vec3(0.45f,0.6f,1.0f);
    // ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * ourlight.color;
  	
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

    if(light == 1.0f)
    {
        vec3 diffuse = diff * vec3(light) + ambient;
        FragColor = vec4(diffuse,1.0f);
    }
    else
    {
    
        vec3 result = (diffuseLight + specularLight + ambient) * objectColor;
        FragColor = vec4(result, 1.0);
    }
} 

