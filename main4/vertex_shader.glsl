#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 FragPos;   // Posición del fragmento en el espacio del mundo.
out vec3 Normal;    // Normal del vértice.

void main() {
    FragPos = aPos; // Pasar la posición del vértice al fragment shader.
    Normal = aNormal; // Pasar la normal al fragment shader.
    gl_Position = vec4(aPos, 1.0);
}