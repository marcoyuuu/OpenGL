#version 330 core
layout(location = 0) in vec3 aPos;  // Especifica que la entrada (input) de este shader es un vector de 3 componentes llamado aPos, que se encuentra en la ubicación 0 del VAO.

void main() {
    // La variable `gl_Position` es una variable reservada de OpenGL que define la posición final del vértice.
    // Aquí convertimos el `aPos` de un vec3 (x, y, z) a un vec4 añadiendo el valor `1.0` como la componente w.
    gl_Position = vec4(aPos, 1.0);  
}
