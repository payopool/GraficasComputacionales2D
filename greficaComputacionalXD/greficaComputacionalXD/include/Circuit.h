#pragma once
#include "Prerequisitos.h"


/**
 * @brief Genera los puntos del circuito de la pista con suavizado.
 *
 * - Usa los puntos originales como control.
 * - Aplica Catmull-Rom spline para suavizar las curvas.
 * - Escala y centra la pista en la ventana.
 */
inline std::vector<sf::Vector2f> GetCircuitPoints(sf::Vector2u windowSize) {
    std::vector<sf::Vector2f> controlPoints = {
        {200.f, 650.f}, {280.f, 600.f}, {360.f, 550.f}, {440.f, 500.f},
        {520.f, 480.f}, {600.f, 500.f}, {680.f, 560.f},
        {660.f, 460.f}, {580.f, 400.f}, {500.f, 360.f},
        {560.f, 320.f}, {640.f, 340.f}, {700.f, 400.f},
        {660.f, 280.f}, {560.f, 240.f}, {460.f, 220.f},
        {380.f, 260.f}, {300.f, 320.f}, {240.f, 400.f}, {220.f, 500.f},
    };

    // Escalar moderado para curvas más largas
    for (auto& p : controlPoints) {
        p *= 1.3f;
    }

    // Función Catmull-Rom
    auto CatmullRom = [](const sf::Vector2f& p0, const sf::Vector2f& p1,
        const sf::Vector2f& p2, const sf::Vector2f& p3, float t) {
            float t2 = t * t;
            float t3 = t2 * t;
            return 0.5f * ((2.f * p1) +
                (-p0 + p2) * t +
                (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * t2 +
                (-p0 + 3.f * p1 - 3.f * p2 + p3) * t3);
        };

    // Generar puntos suavizados
    std::vector<sf::Vector2f> points;
    int subdivisions = 20; // más subdivisiones = curva más suave
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        const sf::Vector2f& p0 = controlPoints[(i + controlPoints.size() - 1) % controlPoints.size()];
        const sf::Vector2f& p1 = controlPoints[i];
        const sf::Vector2f& p2 = controlPoints[(i + 1) % controlPoints.size()];
        const sf::Vector2f& p3 = controlPoints[(i + 2) % controlPoints.size()];

        for (int j = 0; j <= subdivisions; ++j) {
            float t = j / static_cast<float>(subdivisions);
            points.push_back(CatmullRom(p0, p1, p2, p3, t));
        }
    }

    // Calcular bounding box
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
