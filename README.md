# Beati Engine

Beati es un motor gráfico (Game Engine) desarrollado desde cero en C++ con el objetivo de comprender en profundidad cómo funcionan los sistemas internos de un motor moderno.

El proyecto comenzó como una implementación siguiendo la serie de desarrollo de motores de juego de TheCherno (Hazel Engine), y luego fue extendido de manera independiente con nuevas funcionalidades y mejoras propias.

Actualmente, Beati se utiliza como base para proyectos personales que requieren visualización gráfica y simulación.

---

## Estado del proyecto

El motor se encuentra en desarrollo activo.

Estado actual:
- Sistema de entidades implementado (ECS)
- Integración de interfaz gráfica (ImGui)
- Física 2D en desarrollo utilizando Box2D

Actualmente trabajando en:
- Finalización del sistema de físicas 2D
- Mejora de integración entre física y render
- Optimización general del motor

---

## Objetivo

El objetivo principal de este proyecto es educativo y práctico:

- Comprender arquitectura de motores gráficos
- Implementar sistemas fundamentales desde bajo nivel
- Experimentar con renderizado, físicas y estructuras de datos
- Servir como base para proyectos propios

---

## Tecnologías y librerías utilizadas

El motor está desarrollado en C++ y utiliza las siguientes librerías:

- **OpenGL** → Renderizado gráfico
- **GLFW** → Manejo de ventanas y entradas
- **GLAD** → Carga de funciones OpenGL
- **ImGui** → Interfaz gráfica de depuración
- **Box2D** → Sistema de físicas 2D
- **glm** → Matemática (vectores, matrices)
- **entt** → Sistema ECS (Entity Component System)
- **spdlog** → Sistema de logging
- **stb_image** → Carga de imágenes

---

## Características principales

- Motor modular basado en capas (Layer System)
- Sistema de entidades basado en ECS (entt)
- Renderizado con OpenGL
- Sistema de eventos y entradas
- Consola de logs integrada
- Interfaz de usuario para debugging (ImGui)
- Base para simulaciones y visualización en tiempo real

---

## Plataforma soportada

Actualmente el proyecto soporta únicamente:

- Windows (64 bits)

---

### Requisitos

- Windows 64-bit
- Visual Studio (recomendado 2022)
- Git

### Pasos de instalación

1. Clonar el repositorio:
   ```bash
   git clone <repo-url>
   ```
2. Generar los archivos del proyecto:
  ```bash
  scripts/Win-GenProjetcs.bat
  ```
3. Abrir la solución:
  ```bash
  Beati.sln
  ```
  Compilar y ejecutar desde Visual Studio

---

## Origen del proyecto

Este motor está inspirado en la serie de desarrollo de motores de juego de TheCherno (Hazel Engine):
https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

El proyecto comenzó como una implementación guiada y evolucionó hacia un desarrollo independiente con modificaciones y funcionalidades propias.

## Uso actual

Se utiliza como herramienta para:

- Prototipado de sistemas gráficos
- Visualización de datos/simulaciones
- Desarrollo de proyectos personales
## Notas
- El motor está en desarrollo activo, por lo que pueden existir cambios frecuentes
- Algunas funcionalidades pueden estar incompletas o en proceso de refactorización
