#version 330 core

in vec3 FragPos;    // Posición del fragmento en el espacio del mundo.
in vec3 Normal;     // Normal del fragmento en el espacio del mundo.
in vec3 vertexColor; // Color del vértice pasado desde el vertex shader.

out vec4 FragColor;

// Variables uniformes de luz y cámara
uniform vec3 lightPos;     // Posición de la luz puntual.
uniform vec3 viewPos;      // Posición de la cámara.
uniform vec3 lightColor;   // Color de la luz.

// Nuevas variables uniformes para ajustar la intensidad de los componentes de iluminación
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

// Luz direccional
uniform vec3 lightDir; // Dirección de la luz direccional (como si fuera el sol)

// Luz puntual
uniform vec3 pointLightPos; // Posición de la luz puntual

void main() {
    // Componente ambiental
    vec3 ambient = ambientStrength * lightColor;

    // Componente difusa para la luz puntual
    vec3 norm = normalize(Normal);
    vec3 lightDirPoint = normalize(lightPos - FragPos);
    float diffPoint = max(dot(norm, lightDirPoint), 0.0);
    vec3 diffusePoint = diffuseStrength * diffPoint * lightColor;

    // Componente difusa para la luz direccional
    vec3 dirLightDir = normalize(-lightDir); // Luz viene en dirección opuesta a lightDir
    float dirDiff = max(dot(norm, dirLightDir), 0.0);
    vec3 diffuseDir = dirDiff * lightColor;

    // Componente especular (aplicada tanto para luz puntual como para direccional)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDirPoint = reflect(-lightDirPoint, norm);
    float specPoint = pow(max(dot(viewDir, reflectDirPoint), 0.0), 64.0); // Valor de brillo ajustado
    vec3 specularPoint = specularStrength * specPoint * lightColor;

    vec3 reflectDirDir = reflect(-dirLightDir, norm);
    float specDir = pow(max(dot(viewDir, reflectDirDir), 0.0), 32.0); // Especular para la luz direccional
    vec3 specularDir = specularStrength * specDir * lightColor;

    // Sumar los componentes para obtener el color final
    vec3 result = (ambient + diffusePoint + diffuseDir + specularPoint + specularDir) * vertexColor; // Usar el color del vértice
    FragColor = vec4(result, 1.0);
}
