#pragma once

// Librerías estándar
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <limits>
#include <tuple>
#include <cstddef>
#include <queue>
#include <imgui.h>
#include <imgui-SFML.h>
// Librería externa: SFML
#include <SFML/Graphics.hpp>

/**
 * @brief Macro para liberar punteros de forma segura.
 *
 * Elimina el recurso si no es nullptr y lo asigna a nullptr.
 */
#define SAFE_PTR_RELEASE(x) if(x != nullptr) { delete x; x = nullptr; }

 /**
  * @brief Macro para mostrar mensajes de creación de recursos.
  *
  * @param classObj Nombre de la clase.
  * @param method   Nombre del método.
  * @param state    Estado del recurso.
  */
#define MESSAGE(classObj, method, state)                      \
{                                                             \
    std::ostringstream os_;                                   \
    os_ << classObj << "::" << method << " : "                \
        << "[CREATION OF RESOURCE" << ": " << state << "]\n"; \
    std::cerr << os_.str();                                   \
}

  /**
   * @brief Macro para mostrar mensajes de error y terminar ejecución.
   *
   * @param classObj Nombre de la clase.
   * @param method   Nombre del método.
   * @param errorMSG Mensaje de error.
   */
#define ERROR(classObj, method, errorMSG)                         \
{                                                                 \
    std::ostringstream os_;                                       \
    os_ << "ERROR : " << classObj << "::" << method << " : "      \
        << "  Error in data from params [" << errorMSG << "]\n";  \
    std::cerr << os_.str();                                       \
    exit(1);                                                      \
}

   /**
    * @brief Enumeración para tipos de figuras geométricas.
    */
enum ShapeType {
    EMPTY = 0, ///< Sin figura
    CIRCLE = 1, ///< Círculo
    RECTANGLE = 2, ///< Rectángulo
    TRINAGLE = 3, ///< Triángulo 
    POLYGON = 4, ///< Polígono
    SQUARE = 5  ///< Cuadrado
};
