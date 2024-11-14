#include <GL/glew.h>     // Incluye GLEW para acceder a las funciones modernas de OpenGL.
#include <GLFW/glfw3.h>  // Incluye GLFW para gestionar la ventana y el contexto de OpenGL.

int main() {
    // Inicializa GLFW, una biblioteca para crear ventanas y manejar el contexto de OpenGL.
    if (!glfwInit()) {
        // Si la inicialización falla, retorna un valor de error.
        return -1;
    }

    // Crear una ventana de 800x600 píxeles titulada "Triángulo Moderno".
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triángulo Moderno", NULL, NULL);
    if (!window) {
        // Si la ventana no se pudo crear, termina GLFW y retorna un error.
        glfwTerminate();
        return -1;
    }

    // Hace que el contexto de OpenGL de esta ventana sea el actual.
    glfwMakeContextCurrent(window);

    // Inicializa GLEW para cargar todas las funciones modernas de OpenGL.
    if (glewInit() != GLEW_OK) {
        // Si la inicialización de GLEW falla, retorna un error.
        return -1;
    }

    // Configura un callback para ajustar el viewport si la ventana cambia de tamaño.
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        // Cada vez que la ventana cambia de tamaño, OpenGL ajusta el área de renderizado (viewport).
        glViewport(0, 0, width, height);
    });

    // Definición de los vértices del triángulo en coordenadas del espacio normalizado de OpenGL.
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // Vértice 1: esquina inferior izquierda.
         0.5f, -0.5f, 0.0f,  // Vértice 2: esquina inferior derecha.
         0.0f,  0.5f, 0.0f   // Vértice 3: parte superior.
    };

    // Crear un VBO (Vertex Buffer Object) para almacenar los datos de los vértices en la GPU.
    unsigned int VBO;
    glGenBuffers(1, &VBO);  // Genera un buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Enlaza el buffer generado como el buffer actual de tipo GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Carga los datos de los vértices en el buffer.

    // Crear un VAO (Vertex Array Object) para almacenar la configuración de los atributos de los vértices.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  // Genera un VAO.
    glBindVertexArray(VAO);  // Enlaza el VAO como el VAO actual.

    // Enlazar el VBO al VAO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Define cómo interpretar los datos de los vértices.
    // Parámetros: location 0, 3 componentes por vértice (x, y, z), tipo float, no normalizado, sin desplazamiento.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Habilita el atributo de los vértices definido en la ubicación 0.
    glEnableVertexAttribArray(0);

    // Bucle principal de renderizado: se ejecuta hasta que se cierre la ventana.
    while (!glfwWindowShouldClose(window)) {
        // Limpia el buffer de color para prepararse para dibujar el siguiente cuadro.
        glClear(GL_COLOR_BUFFER_BIT);

        // Enlaza el VAO (que contiene el VBO y la configuración de los vértices).
        glBindVertexArray(VAO);
        // Dibuja los vértices como un triángulo.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Intercambia los buffers de la ventana para mostrar el resultado en pantalla.
        glfwSwapBuffers(window);
        // Procesa los eventos de la ventana (como teclas y movimiento del ratón).
        glfwPollEvents();
    }

    // Limpia y finaliza GLFW, liberando todos los recursos utilizados.
    glfwTerminate();
    return 0;
}
