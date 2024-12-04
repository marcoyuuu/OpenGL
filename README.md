# OpenGL Implementaciones

## Objetivo

Este proyecto forma parte del curso de **Gráficos por Computadoras (COMP 4046)**, de la **Universidad de Puerto Rico - Recinto de Mayagüez**. El objetivo principal es aprender y practicar la implementación de OpenGL, reforzando los conceptos teóricos desde el primer día. Los participantes desarrollarán ejemplos interactivos, comenzando con ejercicios simples y avanzando hacia aplicaciones más complejas. Se proporcionan códigos base, ejercicios en vivo y discusiones para consolidar los conocimientos.

Este repositorio contiene diversos ejemplos de implementaciones de OpenGL, organizados en directorios que reflejan el progreso en el aprendizaje de gráficos 3D. A continuación se describen los contenidos de cada carpeta para guiar al usuario a través del aprendizaje y uso de OpenGL.

## Estructura del Proyecto

### 1. **main1**

- **Archivo Principal**: `main1.cpp`
- **Ejecutable**: `OpenGLTriangle1.exe`
- **Descripción**: Primer ejemplo de OpenGL donde se dibuja un triángulo simple en la pantalla. Este ejercicio está diseñado para comprender la configuración básica de OpenGL, incluidos los conceptos de buffer de vértices y shaders.

### 2. **main2**

- **Archivo Principal**: `main2.cpp`
- **Ejecutable**: `OpenGLTriangle2.exe`
- **Descripción**: Se amplía el primer triángulo con configuraciones adicionales, como manipulación de colores de los vértices y la introducción de funciones más avanzadas para el control de la ventana.

### 3. **main3**

- **Archivo Principal**: `main3.cpp`
- **Shaders**: `fragment_shader.glsl`, `vertex_shader.glsl`
- **Ejecutable**: `OpenGLTriangle3.exe`
- **Descripción**: Implementación que utiliza shaders personalizados para mejorar el aspecto del triángulo, brindando mayor control sobre la renderización de vértices y fragmentos, incluyendo cálculos de color más complejos.

### 4. **main4**

- **Archivo Principal**: `main4.cpp`
- **Shaders**: `fragment_shader.glsl`, `vertex_shader.glsl`
- **Ejecutable**: `OpenGLTriangleWithLighting.exe`
- **Descripción**: Introduce iluminación básica en la escena mediante el modelo de iluminación de Phong. Se exploran conceptos de luz ambiental, difusa y especular para dar realismo al objeto.

### 5. **main5**

- **Archivo Principal**: `main5.cpp`
- **Shaders**: `phong_fragment_shader.glsl`, `phong_vertex_shader.glsl`
- **Ejecutable**: `OpenGLTriangleWithLighting.exe`
- **Descripción**: Mejora la iluminación de Phong incorporando múltiples fuentes de luz y una cámara controlable para explorar la escena. Los usuarios pueden experimentar con diferentes posiciones de luz y observar el impacto en los objetos.

### 6. **main6**

- **Archivo Principal**: `main6.cpp`
- **Shaders**: `phong_fragment_shader.glsl`, `phong_vertex_shader.glsl`
- **Ejecutable**: `OpenGLTrianglesWithMovCamara.exe`
- **Descripción**: Añade una escena 3D interactiva con múltiples triángulos de colores y texturas variadas. La cámara se controla mediante las teclas **WASD** para el movimiento y el ratón para la rotación. Los shaders se han optimizado para utilizar los colores de los vértices, mejorando la diversidad visual. También se ajusta la iluminación para mejorar la visualización de los materiales.

## Presentación

Este proyecto incluye dos presentaciones relacionadas con el desarrollo y aprendizaje de OpenGL:

1. **Presentación Teórica**: Esta presentación cubre los conceptos fundamentales sobre gráficos por computadora y OpenGL. Fue presentada durante el primer día del curso y está disponible en [este enlace](https://docs.google.com/presentation/d/19d-l4Ma-K9AuGWEKK29KpL0AwqQMla4g_-AauQap0QA/edit?usp=sharing).

2. **Presentación de Implementación**: Esta presentación cubre el proceso práctico de implementación de OpenGL, detallando los ejemplos y ejercicios desarrollados en el curso. Está disponible como un archivo PDF dentro del repositorio, llamado **Presentación Implementacion OpenGL.pdf**.

Ambas presentaciones proporcionan una guía completa para entender y aplicar OpenGL, desde la teoría básica hasta la implementación avanzada.

## Requisitos Previos

- **OpenGL**: Asegúrate de que tu sistema soporta OpenGL 3.3 o superior.
- **GLFW**: Para la gestión de ventanas e interacción con el usuario.
- **GLEW**: Para manejar las extensiones de OpenGL.
- **GLM**: Biblioteca matemática para transformaciones y cálculos vectoriales.
- **CMake**: Para compilar el proyecto de manera multiplataforma.

Aquí tienes el README actualizado con la mención de los detalles de compilación, según lo solicitado:

---

## Compilar el Proyecto

1. **Clonar el Repositorio**:
   ```bash
   git clone https://github.com/marcoyuuu/OpenGL.git
   cd OpenGL
   ```
2. **Ir a la carpeta deseada**:
   ```bash
   cd main1
   ```
3. **Compilar si hay cambios**:
   ```bash
   g++ main1.cpp -o OpenGLTriangle1 -I"C:/msys64/mingw64/include" -L"C:/msys64/mingw64/lib" -lfreeglut -lopengl32 -lglu32
   ```
4. **Ejecutar el ejecutable**:
   ```bash
   ./OpenGLTriangle1.exe
   ```

Los detalles sobre cómo compilar el proyecto, incluyendo las bibliotecas necesarias como GLFW, GLEW y GLM, están más detallados en la **Presentación de Implementación** disponible en el repositorio, y también se ilustran en la imagen adjunta a continuación.

## Estructura de Carpetas

- **main1, main2, main3, main4, main5, main6**: Cada carpeta contiene un ejemplo progresivo de implementaciones en OpenGL.
- **.vscode/**: Configuraciones de entorno para Visual Studio Code, incluyendo la configuración de tareas y propiedades del compilador.
- **LICENSE**: Archivo que contiene la licencia del proyecto.
- **Presentación Implementacion OpenGL.pdf**: Documento PDF con la presentación sobre la implementación de OpenGL.
- **README.md**: Este archivo de documentación.

---

Este formato incluye la estructura de carpetas en un formato claro y fácil de entender, con el comando `ls` que muestra la disposición actual de los archivos en el repositorio.

## Contribuir

Si deseas contribuir a este repositorio, puedes ayudar agregando más ejemplos, corrigiendo errores o mejorando las implementaciones actuales. Por favor, envía un pull request y estaré encantado de revisarlo.

## Licencia

Este proyecto está bajo la licencia MIT - consulta el archivo [LICENSE](LICENSE) para más detalles.
