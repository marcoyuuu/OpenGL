#include <GL/glew.h>     // Biblioteca para manejar extensiones de OpenGL.
#include <GLFW/glfw3.h>  // Biblioteca para crear ventanas y manejar eventos del sistema.
#include <iostream>      // Biblioteca estándar para entrada y salida.
#include <fstream>       // Biblioteca para manejar archivos.
#include <sstream>       // Biblioteca para manejar flujos de cadenas.
#include <glm/glm.hpp>   // Biblioteca para vectores y matrices matemáticas.
#include <glm/gtc/matrix_transform.hpp> // Incluye funciones para transformar matrices
#include <glm/gtc/type_ptr.hpp> // Incluye funciones para convertir matrices a punteros

// Función que carga el contenido de un archivo y lo devuelve como un string.
std::string loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf(); // Lee el contenido del archivo y lo almacena en el buffer.
    return buffer.str();    // Convierte el contenido a string y lo devuelve.
}

// Función para compilar un shader a partir de su código fuente.
unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();  // Convierte el código fuente a un puntero de tipo char.
    glShaderSource(shader, 1, &src, nullptr);  // Carga el código fuente en el objeto shader.
    glCompileShader(shader);  // Compila el shader.

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error al compilar el shader: " << infoLog << std::endl;
    }
    return shader;  // Devuelve el identificador del shader compilado.
}

// Función para crear un programa de shader que combina un shader de vértices y uno de fragmentos.
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);  // Enlaza el programa completo de shaders.

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error al enlazar el programa de shaders: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;  // Devuelve el identificador del programa de shaders.
}

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "No se pudo inicializar GLFW" << std::endl;
        return -1;
    }

    // Crear una ventana de 800x600 píxeles
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triángulo con Phong Shading", NULL, NULL);
    if (!window) {
        std::cerr << "No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "No se pudo inicializar GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Habilitar el buffer de profundidad desde el inicio para Phong Shading

    // Definir las coordenadas de los vértices de los triángulos, incluyendo las normales
    float vertices[] = {
        // Primer triángulo
        // posiciones         // normales
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
        
        // Segundo triángulo (cercano al primero para generar un efecto de sombra o reflejo)
        // posiciones         // normales
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Crear el programa de shaders
    unsigned int shaderProgram = createShaderProgram("phong_vertex_shader.glsl", "phong_fragment_shader.glsl");

    // Definir la posición y el color de la luz y de la cámara
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.2f);
    glm::vec3 viewPos(0.0f, 0.0f, 3.0f); // Posición de la cámara en el espacio del mundo

    // Definir las matrices de transformación (modelo, vista, proyección)
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        // Limpiar la pantalla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Usar el programa de shaders
        glUseProgram(shaderProgram);

        // Definir las matrices de modelo para cada triángulo
        glm::mat4 model1 = glm::mat4(1.0f); // Primer triángulo sin transformación
        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -0.5f)); // Mover el segundo triángulo a la derecha y hacia atrás

        // Pasar las matrices de transformación al vertex shader para el primer triángulo
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Pasar la posición y el color de la luz al fragment shader
        int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, &lightPos[0]);

        int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, &lightColor[0]);

        int objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        glUniform3fv(objectColorLoc, 1, &objectColor[0]);

        int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        glUniform3fv(viewPosLoc, 1, &viewPos[0]);

        // Dibujar el primer triángulo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Dibujar el segundo triángulo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glDrawArrays(GL_TRIANGLES, 3, 3);

        // Intercambiar buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpiar y terminar
    glfwTerminate();
    return 0;
}