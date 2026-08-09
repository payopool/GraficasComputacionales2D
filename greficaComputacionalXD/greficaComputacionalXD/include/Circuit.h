/**
 *  Circuit.h
 *  Funciones auxiliares para generar el circuito de la pista.
 *
 * Contiene la función GetCircuitPoints, que devuelve una lista de puntos
 * representando el trazado de la pista, escalados y centrados en la ventana.
 */

#pragma once
#include "Prerequisitos.h"

 /**
  * @brief Genera los puntos del circuito de la pista.
  *
  * - Define un conjunto inicial de puntos que forman curvas y rectas.
  * - Escala los puntos para alargar las curvas.
  * - Calcula el bounding box de la pista para obtener su centro.
  * - Ajusta los puntos con un offset para centrar la pista en la ventana.
  *
  * @param windowSize Tamaño de la ventana (ancho y alto).
  * @return std::vector<sf::Vector2f> Lista de puntos del circuito centrado.
  */
inline std::vector<sf::Vector2f> GetCircuitPoints(sf::Vector2u windowSize) {
    std::vector<sf::Vector2f> points = {
        {200.f, 650.f}, {280.f, 600.f}, {360.f, 550.f}, {440.f, 500.f},
        {520.f, 480.f}, {600.f, 500.f}, {680.f, 560.f},
        {660.f, 460.f}, {580.f, 400.f}, {500.f, 360.f},
        {560.f, 320.f}, {640.f, 340.f}, {700.f, 400.f},
        {660.f, 280.f}, {560.f, 240.f}, {460.f, 220.f},
        {380.f, 260.f}, {300.f, 320.f}, {240.f, 400.f}, {220.f, 500.f},
    };

    // Escalar moderado para curvas más largas
    for (auto& p : points) {
        p *= 1.3f;
    }

    // Calcular bounding box de la pista
    float minX = 99999, maxX = -99999, minY = 99999, maxY = -99999;
    for (auto& p : points) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }
    sf::Vector2f pistaCentro((minX + maxX) / 2.f, (minY + maxY) / 2.f);

    // Centro de la ventana
    sf::Vector2f ventanaCentro(windowSize.x / 2.f, windowSize.y / 2.f);

    // Offset para centrar pista
    sf::Vector2f offset = ventanaCentro - pistaCentro;
    for (auto& p : points) {
        p += offset;
    }

    return points;
}
