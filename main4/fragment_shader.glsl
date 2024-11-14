#version 330 core
in vec3 FragPos;   // Posición del fragmento.
in vec3 Normal;    // Normal del fragmento.

out vec4 FragColor;

uniform vec3 lightPos;   // Posición de la fuente de luz.
uniform vec3 lightColor; // Color de la luz.
uniform vec3 objectColor; // Color del objeto.

void main() {
    // Calcular la dirección de la luz.
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Calcular la iluminación difusa usando el producto punto entre la normal y la dirección de la luz.
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Calcular el color final del fragmento.
    vec3 result = diffuse * objectColor;
    FragColor = vec4(result, 1.0);
}