#version 330 core

layout(location = 0) in vec3 aPos;       // Posición del vértice
layout(location = 1) in vec3 aNormal;    // Normal del vértice
layout(location = 2) in vec3 aColor;     // Color del vértice

out vec3 FragPos;    // Posición del fragmento en el espacio del mundo.
out vec3 Normal;     // Normal del vértice en el espacio del mundo.
out vec3 vertexColor; // Color del vértice

// Matrices uniformes para transformar los vértices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calcular la posición del fragmento en el espacio del mundo
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transformar la normal al espacio del mundo, evitando deformaciones por escala no uniforme
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Pasar el color del vértice al fragment shader
    vertexColor = aColor;

    // Transformar el vértice al espacio de pantalla
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}