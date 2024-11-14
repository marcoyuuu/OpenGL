#version 330 core

in vec3 FragPos;    // Posición del fragmento en el espacio del mundo.
in vec3 Normal;     // Normal del fragmento en el espacio del mundo.
in vec3 vertexColor; // Color del vértice pasado desde el vertex shader.

out vec4 FragColor;

uniform vec3 lightPos;     // Posición de la fuente de luz.
uniform vec3 viewPos;      // Posición de la cámara.
uniform vec3 lightColor;   // Color de la luz.

// Nuevas variables uniformes para ajustar la intensidad de los componentes de iluminación
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

void main() {
    // Componente ambiental
    vec3 ambient = ambientStrength * lightColor;

    // Componente difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    // Componente especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0); // Valor de brillo ajustado para un reflejo equilibrado
    vec3 specular = specularStrength * spec * lightColor;

    // Sumar los componentes para obtener el color final
    vec3 result = (ambient + diffuse + specular) * vertexColor; // Usar el color del vértice
    FragColor = vec4(result, 1.0);
}
