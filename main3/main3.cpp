#include <GL/glew.h>     // Biblioteca para manejar extensiones de OpenGL.
#include <GLFW/glfw3.h>  // Biblioteca para crear ventanas y manejar eventos del sistema.
#include <iostream>      // Biblioteca estándar para entrada y salida.
#include <fstream>       // Biblioteca para manejar archivos.
#include <sstream>       // Biblioteca para manejar flujos de cadenas.

// Función que carga el contenido de un archivo y lo devuelve como un string.
// Esto es útil para cargar el código fuente de los shaders.
std::string loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf(); // Lee el contenido del archivo y lo almacena en el buffer.
    return buffer.str();    // Convierte el contenido a string y lo devuelve.
}

// Función para compilar un shader a partir de su código fuente.
// Parámetros:
// - type: Tipo de shader (GL_VERTEX_SHADER o GL_FRAGMENT_SHADER).
// - source: Código fuente del shader en forma de string.
unsigned int compileShader(unsigned int type, const std::string& source) {
    // Crear el objeto shader en OpenGL del tipo especificado.
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();  // Convierte el código fuente a un puntero de tipo char.
    glShaderSource(shader, 1, &src, nullptr);  // Carga el código fuente en el objeto shader.
    glCompileShader(shader);  // Compila el shader.

    // Verificación de errores de compilación.
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        // Si hubo un error en la compilación, se obtiene el mensaje de error y se imprime.
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error al compilar el shader:\n" << infoLog << std::endl;
    }
    return shader;  // Devuelve el identificador del shader compilado.
}

// Función para crear un programa de shader que combina un shader de vértices y uno de fragmentos.
// Parámetros:
// - vertexPath: Ruta al archivo del shader de vértices.
// - fragmentPath: Ruta al archivo del shader de fragmentos.
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // Cargar los códigos fuente de los shaders desde archivos.
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    // Compilar los shaders.
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // Crear el programa de shaders y enlazar los shaders compilados.
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);  // Enlaza el programa completo de shaders.

    // Verificación de errores en el enlace del programa.
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        // Si hubo un error al enlazar el programa, se obtiene el mensaje de error y se imprime.
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error al enlazar el programa de shaders:\n" << infoLog << std::endl;
    }

    // Eliminar los shaders individuales porque ya han sido enlazados al programa y no se necesitan más.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;  // Devuelve el identificador del programa de shaders.
}

int main() {
    // Inicializar GLFW (una biblioteca para crear ventanas y contextos de OpenGL).
    if (!glfwInit()) {
        std::cerr << "No se pudo inicializar GLFW" << std::endl;
        return -1;
    }

    // Crear una ventana de 800x600 píxeles titulada "Triángulo con Shaders".
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triángulo con Shaders", NULL, NULL);
    if (!window) {
        std::cerr << "No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Hacer que el contexto de la ventana creada sea el contexto actual de OpenGL.
    glfwMakeContextCurrent(window);

    // Inicializar GLEW (necesario para manejar extensiones modernas de OpenGL).
    if (glewInit() != GLEW_OK) {
        std::cerr << "No se pudo inicializar GLEW" << std::endl;
        return -1;
    }

    // Definir las coordenadas de los vértices del triángulo.
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
    // Especificar cómo interpretar los datos del VBO: 3 componentes (x, y, z), tipo float.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  // Habilitar el atributo del vértice.

    // Crear el programa de shaders usando archivos de shaders para vértices y fragmentos.
    unsigned int shaderProgram = createShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

    // Bucle principal de renderizado: sigue ejecutándose hasta que se cierre la ventana.
    while (!glfwWindowShouldClose(window)) {
        // Limpiar la pantalla para prepararse para el próximo cuadro.
        glClear(GL_COLOR_BUFFER_BIT);

        // Utilizar el programa de shaders.
        glUseProgram(shaderProgram);
        // Enlazar el VAO y dibujar el triángulo.
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // Dibujar los vértices especificados como un triángulo.

        // Intercambiar los buffers de la ventana para mostrar el resultado en pantalla.
        glfwSwapBuffers(window);
        // Procesar los eventos de la ventana (como teclas y movimiento del ratón).
        glfwPollEvents();
    }

    // Terminar GLFW y limpiar todos los recursos utilizados.
    glfwTerminate();
    return 0;
}
