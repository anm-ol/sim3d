#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 center; 
uniform float light;

void main()
{
    // ambient
    float ambientStrength = 0.2f;
    vec3 ambient = vec3(ambientStrength);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    if(light == 1.0f)
    {
        vec3 diffuse = diff * vec3(light) + ambient;
        FragColor = vec4(diffuse,1.0f);
    }
    else
    {
    
        vec3 diffuse = diff * vec3(0.45f,0.6f,1.0f) + ambient;
        FragColor = vec4(diffuse, 1.0);
    }
} 

