#version 330 core

layout(location = 0) in vec3 aPos;      // Position of the vertex
layout(location = 1) in vec2 aTexCoord; // Texture coordinate of the vertex

out vec2 TexCoord; // Pass texture coordinates to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord; // Pass the texture coordinates to the fragment shader
}





/*#version 330 core

layout(location = 0) in vec3 aPos;  // Position of the vertex
layout(location = 1) in vec3 aNormal; // Normal of the vertex

out vec3 FragPos;   // Position of the fragment, passed to fragment shader
out vec3 Normal;    // Normal of the fragment, passed to fragment shader

uniform mat4 model; // Model transformation matrix
uniform mat4 view;  // View transformation matrix
uniform mat4 projection; // Projection transformation matrix

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));  // Transform vertex position to world space
    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normal to world space

    gl_Position = projection * view * vec4(FragPos, 1.0); // Final position of the vertex
}*/
