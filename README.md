# OpenGL Implementaciones - Marco Yuuu

## Objetivo

El objetivo de este proyecto es aprender a implementar OpenGL de forma práctica, reforzando conceptos teóricos desde el primer día. Los participantes desarrollarán ejemplos interactivos, comenzando con ejercicios simples y avanzando hacia aplicaciones más complejas. Se proporcionan códigos base, ejercicios en vivo y discusiones para consolidar conocimientos.

Este repositorio contiene diversos ejemplos de implementaciones de OpenGL, organizados en directorios que reflejan el avance del aprendizaje desde lo más básico hasta ejemplos más avanzados. A continuación, se describen los contenidos de cada carpeta para guiar al usuario a través del aprendizaje de gráficos 3D.

## Estructura del Proyecto

### 1. **main1**

- **Archivo Principal**: `main1.cpp`
- **Ejecutable**: `OpenGLTriangle1.exe`
- **Descripción**: Primer ejemplo de OpenGL donde se dibuja un triángulo simple en la pantalla. Este ejercicio está diseñado para ayudar a comprender la configuración básica de OpenGL, incluidos los conceptos de buffer de vértices y shaders.

### 2. **main2**

- **Archivo Principal**: `main2.cpp`
- **Ejecutable**: `OpenGLTriangle2.exe`
- **Descripción**: En este ejemplo se amplía el primer triángulo con configuraciones adicionales, tales como manipulación del color de los vértices y la introducción de funciones más avanzadas de control de la ventana.

### 3. **main3**

- **Archivo Principal**: `main3.cpp`
- **Shaders**: `fragment_shader.glsl`, `vertex_shader.glsl`
- **Ejecutable**: `OpenGLTriangle3.exe`
- **Descripción**: Implementación que incluye el uso de shaders personalizados para mejorar el aspecto del triángulo. Los shaders proporcionan mayor control sobre cómo se renderizan los vértices y los fragmentos, incluyendo cálculos de color más complejos.

### 4. **main4**

- **Archivo Principal**: `main4.cpp`
- **Shaders**: `fragment_shader.glsl`, `vertex_shader.glsl`
- **Ejecutable**: `OpenGLTriangleWithLighting.exe`
- **Descripción**: Este ejemplo introduce la iluminación básica en la escena utilizando el modelo de iluminación de Phong. Se cubren los conceptos de luz ambiental, difusa y especular para dar un mayor realismo al objeto.

### 5. **main5**

- **Archivo Principal**: `main5.cpp`
- **Shaders**: `phong_fragment_shader.glsl`, `phong_vertex_shader.glsl`
- **Ejecutable**: `OpenGLTriangleWithLighting.exe`
- **Descripción**: Esta versión mejora la iluminación de Phong, incorporando múltiples fuentes de luz y cámara controlable para permitir la exploración de la escena. Los usuarios pueden experimentar con diferentes posiciones de luz y observar el impacto en los objetos.

## Presentación

Se incluye una presentación detallada que explica paso a paso la implementación de cada proyecto. Esta presentación guía a los usuarios desde los primeros conceptos de OpenGL clásico hasta la implementación moderna utilizando VAOs, VBOs y shaders. Es un recurso ideal para aquellos que buscan una comprensión profunda y progresiva de cómo se desarrollan aplicaciones gráficas con OpenGL.

## Requisitos Previos

- **OpenGL**: Asegúrese de que su sistema soporta OpenGL 3.3 o superior.
- **GLFW**: Utilizado para la gestión de ventanas e interacción con el usuario.
- **GLEW**: Para manejar las extensiones de OpenGL.
- **GLM**: Biblioteca matemática para transformaciones y cálculos vectoriales.
- **CMake**: Para compilar el proyecto de una forma multiplataforma.

## Compilar el Proyecto

1. **Clonar el Repositorio**:
   ```bash
   git clone https://github.com/marcoyuuu/OpenGL.git
   cd OpenGL
   ```
2. **Ir a carpeta deseada como:**
   ```bash
   cd main1
   ```
3. **Compilar si hay cambios como**:
   ```bash
   g++ main1.cpp -o OpenGLTriangle1 -I"C:/msys64/mingw64/include" -L"C:/msys64/mingw64/lib" -lfreeglut -lopengl32 -lglu32
   ```
4. **Ejecutar el Ejecutable**:
   ```bash
   ./OpenGLTriangle1.exe
   ```

## Estructura de Carpetas

- **main1, main2, main3, main4, main5**: Cada una de estas carpetas contiene un ejemplo progresivo de implementaciones en OpenGL.
- **.vscode/**: Configuraciones de entorno para Visual Studio Code, incluyendo configuración de tareas y propiedades del compilador.

## Contribuir

Si deseas contribuir a este repositorio, puedes ayudar agregando más ejemplos, corrigiendo errores o mejorando las implementaciones actuales. Por favor, envía un pull request y estaré encantado de revisarlo.

## Licencia

Este proyecto está bajo la licencia MIT - mira el archivo [LICENSE](LICENSE) para más detalles.

