#include <GL/glew.h>     // Biblioteca para manejar extensiones de OpenGL.
#include <GLFW/glfw3.h>  // Biblioteca para crear ventanas y manejar eventos del sistema.
#include <iostream>      // Biblioteca estándar para entrada y salida.
#include <fstream>       // Biblioteca para manejar archivos.
#include <sstream>       // Biblioteca para manejar flujos de cadenas.
#include <glm/glm.hpp>   // Biblioteca para vectores y matrices matemáticas.
#include <glm/gtc/matrix_transform.hpp> // Incluye funciones para transformar matrices
#include <glm/gtc/type_ptr.hpp> // Incluye funciones para convertir matrices a punteros

// Variables globales para el control de la cámara
glm::vec3 cameraPos(0.0f, 0.0f, 0.0f); // Cámara más cerca para una mejor visión de la escena
glm::vec3 cameraFront(0.0f, -0.3f, -1.0f); // Dirección hacia la escena para cubrir mejor el área visible
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // Tiempo entre fotogramas actual y anterior
float lastFrame = 0.0f; // Tiempo del último fotograma

// Variables para el movimiento del ratón
float lastX = 960.0f; // Posición inicial X del cursor
float lastY = 540.0f; // Posición inicial Y del cursor
float yaw = -90.0f;   // Inicialmente mirando hacia el eje -Z
float pitch = 0.0f;
bool firstMouse = true;

// Funciones para el movimiento de la cámara
void processInput(GLFWwindow* window) {
    float cameraSpeed = 5.0f * deltaTime; // Ajustar velocidad de la cámara
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // Cerrar la ventana si se presiona la tecla ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Función para manejar el movimiento del ratón
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // Invertir porque las coordenadas Y de la pantalla van de abajo hacia arriba
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Limitar el ángulo de pitch para evitar "girar la cabeza" completamente
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

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

// Función para crear el plano base infinito
void createGroundPlane(unsigned int &VAO, unsigned int &VBO) {
    float groundVertices[] = {
        // posiciones            // normales         // colores
        -100.0f, -1.0f,  100.0f,    0.0f, 1.0f, 0.0f,   0.3f, 0.3f, 0.3f,  // Gris
         100.0f, -1.0f,  100.0f,    0.0f, 1.0f, 0.0f,   0.3f, 0.3f, 0.3f,  // Gris
         100.0f, -1.0f, -100.0f,    0.0f, 1.0f, 0.0f,   0.3f, 0.3f, 0.3f,  // Gris

        -100.0f, -1.0f,  100.0f,    0.0f, 1.0f, 0.0f,   0.3f, 0.3f, 0.3f,  // Gris
         100.0f, -1.0f, -100.0f,    0.0f, 1.0f, 0.0f,   0.3f, 0.3f, 0.3f,  // Gris
        -100.0f, -1.0f, -100.0f,    0.0f, 1.0f, 0.0f,   0.3f, 0.3f, 0.3f   // Gris
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Atributo de color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "No se pudo inicializar GLFW" << std::endl;
        return -1;
    }

    // Activar MSAA antes de crear la ventana
    glfwWindowHint(GLFW_SAMPLES, 8); // Habilitar MSAA con 4 muestras

    // Crear una ventana de 1920x1080 píxeles
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Escena con Triángulos Texturizados y Coloreados", NULL, NULL);
    if (!window) {
        std::cerr << "No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback); // Configurar el callback para el movimiento del ratón
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Ocultar y capturar el cursor

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "No se pudo inicializar GLEW" << std::endl;
        return -1;
    }

    // Habilitar Multisampling en OpenGL
    glEnable(GL_MULTISAMPLE); // Activar MSAA

    glEnable(GL_DEPTH_TEST); // Habilitar el buffer de profundidad desde el inicio para Phong Shading

    // Definir las coordenadas de los vértices de los triángulos, incluyendo las normales y los colores
    float vertices[] = {
        // Primer triángulo - Brillante
        // posiciones         // normales           // colores
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,  // Rojo
        0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,  // Verde
        0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,  // Azul
        
        // Segundo triángulo - Textura como cemento
        // posiciones         // normales           // colores
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.5f, 0.5f, 0.5f,  // Gris
        0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.5f, 0.5f, 0.5f,  // Gris
        0.0f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.5f, 0.5f, 0.5f,  // Gris

        // Tercer triángulo - Mate, como cemento
        // posiciones         // normales           // colores
        -0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.6f, 0.6f, 0.6f,  // Cemento
        0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.6f, 0.6f, 0.6f,  // Cemento
        0.0f,  0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.6f, 0.6f, 0.6f,  // Cemento

        // Cuarto triángulo - Brillante, metálico
        // posiciones         // normales           // colores
        1.0f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.8f, 0.8f, 0.8f,  // Plata
        1.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.8f, 0.8f, 0.8f,  // Plata
        1.25f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.8f, 0.8f, 0.8f,  // Plata

        // Quinto triángulo - Amarillo brillante
        -1.0f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f,  // Amarillo
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f,  // Amarillo
        -0.75f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f,  // Amarillo

        // Sexto triángulo - Verde claro
        1.0f,  1.0f, -0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.5f,  // Verde claro
        1.5f,  1.0f, -0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.5f,  // Verde claro
        1.25f, 1.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.5f,  // Verde claro

        // Séptimo triángulo - Magenta
        -1.5f, -1.0f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f,  // Magenta
        -1.0f, -1.0f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f,  // Magenta
        -1.25f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f,  // Magenta

        // Octavo triángulo - Cyan
        -2.0f,  1.0f, -1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,  // Cyan
        -1.5f,  1.0f, -1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,  // Cyan
        -1.75f, 1.5f, -1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,  // Cyan

        // Noveno triángulo - Gris oscuro
        0.0f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,    0.3f, 0.3f, 0.3f,  // Gris oscuro
        0.5f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,    0.3f, 0.3f, 0.3f,  // Gris oscuro
        0.25f, -0.5f, -1.0f,  0.0f, 0.0f, 1.0f,    0.3f, 0.3f, 0.3f,  // Gris oscuro

        // Décimo triángulo - Azul cobalto
        2.0f,  0.0f, 0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.8f,  // Azul cobalto
        2.5f,  0.0f, 0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.8f,  // Azul cobalto
        2.25f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.8f,  // Azul cobalto

        // Undécimo triángulo - Verde oliva
        -2.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 0.5f, 0.0f,  // Verde oliva
        -3.0f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 0.5f, 0.0f,  // Verde oliva
        -2.75f, 0.0f, 0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 0.5f, 0.0f,  // Verde oliva

        // Duodécimo triángulo - Lila pastel
        -3.0f,  1.0f, -0.5f,   0.0f, 0.0f, 1.0f,    0.8f, 0.6f, 1.0f,  // Lila pastel
        -2.5f,  1.0f, -0.5f,   0.0f, 0.0f, 1.0f,    0.8f, 0.6f, 1.0f,  // Lila pastel
        -2.75f, 1.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.8f, 0.6f, 1.0f,  // Lila pastel

        // Decimotercer triángulo - Naranja quemado
        3.0f, -1.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.3f, 0.0f,  // Naranja quemado
        3.5f, -1.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.3f, 0.0f,  // Naranja quemado
        3.25f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.3f, 0.0f,  // Naranja quemado

        // Decimocuarto triángulo - Azul claro
        4.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.4f, 0.7f, 1.0f,  // Azul claro
        4.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.4f, 0.7f, 1.0f,  // Azul claro
        4.25f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.4f, 0.7f, 1.0f,  // Azul claro

        // Decimoquinto triángulo - Rojo oscuro
        -4.0f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.5f, 0.0f, 0.0f,  // Rojo oscuro
        -4.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.5f, 0.0f, 0.0f,  // Rojo oscuro
        -4.25f, 0.0f, -0.5f,   0.0f, 0.0f, 1.0f,    0.5f, 0.0f, 0.0f,  // Rojo oscuro

        // Decimosexto triángulo - Rosa brillante
        3.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.2f, 0.5f,  // Rosa brillante
        3.5f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.2f, 0.5f,  // Rosa brillante
        3.25f, 1.5f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.2f, 0.5f,  // Rosa brillante

        // Decimoséptimo triángulo - Verde esmeralda
        -3.0f, -1.0f, 1.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.8f, 0.4f,  // Verde esmeralda
        -3.5f, -1.0f, 1.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.8f, 0.4f,  // Verde esmeralda
        -3.25f, -0.5f, 1.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.8f, 0.4f,  // Verde esmeralda

        // Decimooctavo triángulo - Azul marino
        2.5f,  0.5f, -1.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.5f,  // Azul marino
        3.0f,  0.5f, -1.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.5f,  // Azul marino
        2.75f, 1.0f, -1.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.5f,  // Azul marino

        // Decimonoveno triángulo - Amarillo dorado
        -2.0f, -2.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.9f, 0.0f,  // Amarillo dorado
        -1.5f, -2.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.9f, 0.0f,  // Amarillo dorado
        -1.75f, -1.5f, 1.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.9f, 0.0f,  // Amarillo dorado

        // Vigésimo triángulo - Blanco brillante
        4.5f,  2.0f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,  // Blanco brillante
        5.0f,  2.0f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,  // Blanco brillante
        4.75f, 2.5f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f   // Blanco brillante
        };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Atributo de color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Crear el programa de shaders
    unsigned int shaderProgram = createShaderProgram("phong_vertex_shader.glsl", "phong_fragment_shader.glsl");

    // Definir la posición y el color de la luz y de la cámara
    glm::vec3 lightPos(5.0f, 10.0f, 10.0f); // Luz elevada para iluminar desde arriba
    glm::vec3 lightColor(0.9f, 0.9f, 1.0f); // Luz ligeramente azulada
    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f); // Luz direccional descendente
    glm::vec3 pointLightPos(2.0f, 1.0f, 1.0f);

    // Crear el plano base
    unsigned int groundVAO, groundVBO;
    createGroundPlane(groundVAO, groundVBO);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        // Tiempo para calcular deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Procesar entradas
        processInput(window);

        // Limpiar la pantalla
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Usar el programa de shaders
        glUseProgram(shaderProgram);

        // Definir la matriz de vista basada en la posición de la cámara
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Pasar las matrices de transformación al vertex shader
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 model = glm::mat4(1.0f); // Matriz identidad
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Pasar la posición, el color y la dirección de la luz al fragment shader
        int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, &lightPos[0]);

        int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, &lightColor[0]);

        int lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
        glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

        int pointLightPosLoc = glGetUniformLocation(shaderProgram, "pointLightPos");
        glUniform3fv(pointLightPosLoc, 1, glm::value_ptr(pointLightPos));

        int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        glUniform3fv(viewPosLoc, 1, &cameraPos[0]);

        // Pasar las intensidades de los componentes de iluminación al fragment shader
        glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), 0.2f);
        glUniform1f(glGetUniformLocation(shaderProgram, "diffuseStrength"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "specularStrength"), 0.5f);

        // Pasar el color del objeto al fragment shader
        glm::vec3 objectColor(1.0f, 0.5f, 0.3f); // Color base del objeto (naranja)
        int objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));

        // Dibujar los triángulos
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 60);

        // Dibujar el plano base
        glBindVertexArray(groundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Intercambiar buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpiar los recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);
    glfwTerminate();
    return 0;
}