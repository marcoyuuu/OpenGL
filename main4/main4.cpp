#include <GL/glew.h>     // Biblioteca para manejar extensiones de OpenGL.
#include <GLFW/glfw3.h>  // Biblioteca para crear ventanas y manejar eventos del sistema.
#include <iostream>      // Biblioteca estándar para entrada y salida.
#include <fstream>       // Biblioteca para manejar archivos.
#include <sstream>       // Biblioteca para manejar flujos de cadenas.
#include <glm/glm.hpp>   // Biblioteca para trabajar con vectores y matrices matemáticas (utilizada para coordenadas y colores en 3D).

// Función para cargar el contenido de un archivo (como un archivo de shader) y devolverlo como un string.
std::string loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);        // Abre el archivo con la ruta especificada.
    std::stringstream buffer;            // Crea un buffer de string.
    buffer << file.rdbuf();              // Lee el contenido del archivo y lo almacena en el buffer.
    return buffer.str();                 // Convierte el contenido del buffer a string y lo devuelve.
}

// Función que compila un "shader" (programa gráfico) a partir de su código fuente.
// Un "shader" es un pequeño programa ejecutado en la tarjeta gráfica.
unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);    // Crea un objeto shader y define su tipo (vértices o fragmentos).
    const char* src = source.c_str();              // Convierte el código fuente a un puntero de tipo char.
    glShaderSource(shader, 1, &src, nullptr);      // Carga el código fuente en el shader.
    glCompileShader(shader);                       // Compila el shader.

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Verifica si la compilación fue exitosa.
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);  // Obtiene el mensaje de error si hay problemas.
        std::cerr << "Error al compilar el shader: " << infoLog << std::endl; // Muestra el error en la consola.
    }
    return shader;  // Devuelve el identificador del shader compilado.
}

// Función para crear un "programa de shader" que combina un shader de vértices y uno de fragmentos.
// Un programa de shader es una combinación de shaders que OpenGL utiliza para dibujar en pantalla.
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // Cargar el código fuente de los shaders desde archivos.
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    // Compilar los shaders de vértices y de fragmentos.
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // Crear un programa de shader y enlazar ambos shaders.
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);  // Enlaza (combina) los shaders en un solo programa.

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Verifica si el enlace fue exitoso.
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog); // Obtiene el mensaje de error si hay problemas.
        std::cerr << "Error al enlazar el programa de shaders: " << infoLog << std::endl; // Muestra el error en la consola.
    }

    glDeleteShader(vertexShader);  // Borra el shader de vértices ya que está enlazado.
    glDeleteShader(fragmentShader); // Borra el shader de fragmentos ya que está enlazado.
    return shaderProgram;  // Devuelve el identificador del programa de shaders.
}

int main() {
    // Inicializar GLFW, que es una biblioteca para crear ventanas y manejar el contexto gráfico.
    if (!glfwInit()) {
        std::cerr << "No se pudo inicializar GLFW" << std::endl;
        return -1;  // Retorna -1 si la inicialización falla.
    }

    // Crear una ventana de 800x600 píxeles.
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triángulo con Iluminación Difusa", NULL, NULL);
    if (!window) {
        std::cerr << "No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();  // Termina GLFW si la creación de la ventana falla.
        return -1;
    }

    glfwMakeContextCurrent(window);  // Establece la ventana como el contexto actual para OpenGL.

    // Inicializar GLEW, que permite utilizar funciones de OpenGL extendidas.
    if (glewInit() != GLEW_OK) {
        std::cerr << "No se pudo inicializar GLEW" << std::endl;
        return -1;
    }

    // Definir las coordenadas de los vértices del triángulo, incluyendo las normales (para iluminación).
    float vertices[] = {
        // posiciones (x, y, z)       // normales (dirección de la luz)
        -0.5f, -0.5f, 0.0f,           1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,           0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,           0.0f, 0.0f, 1.0f
    };

    // Crear objetos para almacenar los datos de los vértices.
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);    // Crea un objeto para almacenar configuraciones de los vértices.
    glGenBuffers(1, &VBO);         // Crea un buffer para almacenar los datos de los vértices.

    glBindVertexArray(VAO);        // Vincula el VAO (configuraciones de vértices).

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                   // Vincula el buffer de vértices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copia los datos al buffer.

    // Especificar la posición de los atributos en los datos de los vértices.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // Atributo de posición.
    glEnableVertexAttribArray(0);                                                  // Activa el atributo.

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // Atributo de normales.
    glEnableVertexAttribArray(1);  // Activa el atributo.

    // Crear el programa de shaders.
    unsigned int shaderProgram = createShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

    // Definir la posición y el color de la luz, y el color del objeto.
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);  // Posición de la luz.
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // Color de la luz (blanco).
    glm::vec3 objectColor(1.0f, 0.5f, 0.2f); // Color del objeto (anaranjado).

    // Bucle principal para mantener la ventana abierta hasta que se cierre.
    while (!glfwWindowShouldClose(window)) {
        // Limpiar la pantalla.
        glClear(GL_COLOR_BUFFER_BIT);

        // Activar el programa de shaders.
        glUseProgram(shaderProgram);

        // Pasar la posición y el color de la luz al shader.
        int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, &lightPos[0]);

        int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, &lightColor[0]);

        int objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        glUniform3fv(objectColorLoc, 1, &objectColor[0]);

        // Dibujar el triángulo.
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Intercambiar los buffers para mostrar la imagen en pantalla.
        glfwSwapBuffers(window);
        glfwPollEvents();  // Verificar si hay eventos de usuario (como cerrar la ventana).
    }

    // Limpiar y cerrar el contexto de OpenGL.
    glfwTerminate();
    return 0;
}
