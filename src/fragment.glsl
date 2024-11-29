#version 330 core // had to use a lot of yt vids and online references to get the frag to work how i wanted
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D texture1; // texture sampler
uniform vec3 lightPos;
uniform vec3 viewPos; // cam pos
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = (ambient + diffuse + specular); // combine ambient, diffused, and specular lighting
    vec4 texColor = texture(texture1, TexCoord);
    FragColor = vec4(lighting, 1.0) * texColor; // final result
}
