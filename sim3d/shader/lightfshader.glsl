#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 center; 
uniform float light;
uniform vec3 cameraPos;

void main()
{
    vec3 objectColor = vec3(0.45f,0.6f,1.0f);
    // ambient
    float ambientStrength = 0.2f;
    vec3 ambient = vec3(ambientStrength);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    //specular
    float specularStrength = 0.3f;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 25);
    float specular = specularStrength * spec;  

    if(light == 1.0f)
    {
        vec3 diffuse = diff * vec3(light) + ambient;
        FragColor = vec4(diffuse,1.0f);
    }
    else
    {
    
        vec3 diffuse = (diff + specular) * objectColor + ambient;
        FragColor = vec4(diffuse, 1.0);
    }
} 

