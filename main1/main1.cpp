#include <GL/glut.h>  // Incluye la biblioteca GLUT para gestionar ventanas y dibujar gráficos en OpenGL.

// Función de callback que se encarga de renderizar la escena.
void display() {
    // Limpia el buffer de color para preparar la pantalla para dibujar.
    glClear(GL_COLOR_BUFFER_BIT);

    // Inicia el dibujo de un triángulo.
    glBegin(GL_TRIANGLES);
        // Define el primer vértice del triángulo en la posición (-0.5, -0.5).
        glVertex2f(-0.5f, -0.5f); 
        // Define el segundo vértice del triángulo en la posición (0.5, -0.5).
        glVertex2f(0.5f, -0.5f);
        // Define el tercer vértice del triángulo en la posición (0.0, 0.5).
        glVertex2f(0.0f, 0.5f);
    // Finaliza la secuencia de dibujo del triángulo.
    glEnd();

    // Asegura que todas las órdenes de dibujo se ejecuten y se envíen a la pantalla.
    glFlush();
}

// Función principal del programa.
int main(int argc, char** argv) {
    // Inicializa GLUT y procesa cualquier argumento que pueda ser necesario.
    glutInit(&argc, argv);

    // Configura el modo de display para utilizar el buffer simple de color (GLUT_SINGLE) y el espacio de color RGB.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Establece el tamaño inicial de la ventana en 640x480 píxeles.
    glutInitWindowSize(640, 480);

    // Crea una ventana con el título "Triángulo Simple".
    glutCreateWindow("Triangulo Simple");

    // Establece la función de display (renderizado), que se llama cada vez que es necesario redibujar.
    glutDisplayFunc(display);

    // Entra en el bucle principal de GLUT. Este bucle gestiona eventos como el redibujado y la interacción del usuario.
    glutMainLoop();

    return 0;  // Retorna 0 para indicar una ejecución exitosa.
}
