#version 330 core
out vec4 FragColor;  // Especifica que la salida (output) de este shader es un vector de 4 componentes llamado FragColor.

void main() {
    // Asigna el color a cada fragmento. Aquí se define el color naranja (RGB: 1.0, 0.5, 0.2) con una opacidad (alfa) de 1.0.
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);  
}
