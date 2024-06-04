#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 center; 

void main()
{
    // ambient
    float ambientStrength = 0.1;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(- FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f,0.5f,0.1f);
            
    FragColor = vec4(diffuse, 1.0);
} 

