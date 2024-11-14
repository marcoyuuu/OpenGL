#version 330 core

in vec3 FragPos;   // Posición del fragmento en el espacio del mundo.
in vec3 Normal;    // Normal del fragmento en el espacio del mundo.

out vec4 FragColor;

uniform vec3 lightPos;    // Posición de la fuente de luz.
uniform vec3 viewPos;     // Posición de la cámara.
uniform vec3 lightColor;  // Color de la luz.
uniform vec3 objectColor; // Color del objeto.

void main() {
    // Componente ambiental
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Componente difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Componente especular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;

    // Sumar los componentes para obtener el color final
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}