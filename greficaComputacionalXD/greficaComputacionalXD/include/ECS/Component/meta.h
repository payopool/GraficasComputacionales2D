/**
 *  Meta.h
 *  Componente ECS para marcar la posición de la meta en el circuito.
 *
 * Este componente se utiliza para identificar el índice de la meta dentro
 * del vector de puntos del circuito. El RenderSystem lo usa para dibujar
 * la línea de meta en la pista.
 */

#pragma once

namespace ECS {

    /**
     *  Meta
     *  Componente que define la posición de la meta en el circuito.
     *
     * Contiene un único campo:
     * - index: índice dentro del vector de puntos del circuito que corresponde
     *   a la ubicación de la meta.
     */
    struct Meta {
        size_t index; ///< Índice del punto de meta en el circuito
    };

}
