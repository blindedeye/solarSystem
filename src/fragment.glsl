#version 330 core

in vec3 FragPos;   // Fragment position
in vec3 Normal;    // Fragment normal
in vec2 TexCoord;  // Texture coordinate from vertex shader

out vec4 FragColor; // Output color

uniform sampler2D texture1; // Texture sampler
uniform vec3 lightPos;      // Position of the light source
uniform vec3 viewPos;       // Position of the camera/viewer
uniform vec3 lightColor;    // Color of the light

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine all lighting components
    vec3 lighting = (ambient + diffuse + specular);

    // Sample the texture color using TexCoord
    vec4 texColor = texture(texture1, TexCoord);

    // Final color combining lighting and texture color
    FragColor = vec4(lighting, 1.0) * texColor;
}
