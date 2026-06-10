# Proyecto ECS con SFML

Este proyecto implementa un **Entity Component System (ECS)** en C++ utilizando **SFML** para la parte gráfica.  
El objetivo es tener un motor modular donde las entidades, componentes y sistemas se gestionen de forma flexible y escalable.

# Estructura del proyecto

- **core/**
  - `Window.h`: Encapsula la ventana de SFML (`sf::RenderWindow`).
  - `CShape.h`: Clase para crear y dibujar figuras (`sf::Shape`).

- **ECS/**
  - `Types.h`: Define tipos fundamentales (`EntityID`, `EntityIndex`, `EntityVersion`, etc.).
  - `SparceSet.h`: Implementa el conjunto disperso para gestionar entidades activas.
  - `ComponentPool.h`: Maneja pools de componentes genéricos.
  - `View.h`: Permite iterar sobre entidades que poseen un conjunto de componentes.
  - `System.h`: Clase base para sistemas (ciclo de vida: `OnStart`, `OnUpdate`, `OnDestroy`).
  - `Registry.h`: Registro principal que gestiona entidades, componentes y sistemas.

- **main.cpp** : Punto de entrada del programa. Crea la ventana, inicializa una figura y ejecuta el bucle principal.

# Funcionamiento general

1. **Entidades**
   - Se crean con `Registry::CreateEntity()`.
   - Se destruyen con `Registry::DestroyEntity()`.

2. **Componentes**
   - Se añaden con `AddComponent<T>()`.
   - Se consultan con `GetComponent<T>()` o `TryGetComponent<T>()`.
   - Se eliminan con `RemoveComponent<T>()`.

3. **Sistemas**
   - Se añaden con `AddSystem<T>()`.
   - Se actualizan automáticamente con `UpdateSystems(deltaTime)`.
   - Se eliminan con `RemoveAllSystems()`.

4. **Renderizado**
   - `Window` encapsula la ventana SFML.
   - `CShape` permite crear figuras (`Circle`, `Rectangle`, etc.) y dibujarlas en pantalla.
   - El bucle principal (`main.cpp`) gestiona eventos, limpia la pantalla, dibuja y muestra el resultado.

#Ejemplo de uso

```cpp
// Crear entidad
EntityID player = registry.CreateEntity();

// Añadir componente
registry.AddComponent<Position>(player, 100, 200);

// Añadir sistema
auto& movementSystem = registry.AddSystem<MovementSystem>();

// Actualizar sistemas en el bucle principal
registry.UpdateSystems(deltaTime);
