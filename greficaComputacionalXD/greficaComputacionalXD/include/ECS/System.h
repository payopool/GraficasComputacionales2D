namespace ECS {

    class Registry; // Declaración adelantada

    /**
     * @brief Clase base para sistemas dentro del ECS.
     *
     * Define el ciclo de vida de un sistema:
     * - OnStart: inicialización.
     * - OnUpdate: lógica principal (obligatoria).
     * - OnDestroy: limpieza de recursos.
     *
     * También permite habilitar o deshabilitar el sistema.
     */
    class System {
    public:
        /// Destructor virtual por defecto.
        virtual ~System() = default;

        /**
         * @brief Método de inicialización del sistema.
         * @param registry Referencia al registro ECS.
         */
        virtual void OnStart(Registry& /*registry*/) {}

        /**
         * @brief Método de actualización principal del sistema.
         * @param registry Referencia al registro ECS.
         * @param deltaTime Tiempo transcurrido desde el último frame.
         */
        virtual void OnUpdate(Registry& registry, float deltaTime) = 0;

        /**
         * @brief Método de limpieza del sistema.
         * @param registry Referencia al registro ECS.
         */
        virtual void OnDestroy(Registry& /*registry*/) {}

        /**
         * @brief Activa o desactiva el sistema.
         * @param enable true para habilitar, false para deshabilitar.
         */
        void SetEnable(bool enable) noexcept { m_enable = enable; }

        /**
         * @brief Verifica si el sistema está habilitado.
         * @return true si está habilitado, false en caso contrario.
         */
        [[nodiscard]] bool IsEnable() const noexcept { return m_enable; }

    private:
        /// Estado del sistema (por defecto habilitado).
        bool m_enable = true;
    };

} // namespace ECS
