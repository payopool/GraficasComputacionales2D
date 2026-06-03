#pragma once
namespace ECS {
	class Registry;
	class System {
	public:
		virtual ~System() = default;
		// métodos de ciclo de vida do sistema
		virtual void OnStart(Registry& /*registry*/) {}
		// método de atualização do sistema, onde a lógica principal do sistema é executada
		virtual void OnUpdate(Registry& registry, float deltaTime) = 0;
		// método de limpeza do sistema, onde recursos alocados podem ser liberados
			virtual void OnDestroy(Registry & /*registry*/) {}
			//  se active  o desactive el sistema
			void SetAEnable(bool enable) noexcept { m_enable = enable; }
			[[nodiscard]] bool IsEnable() const noexcept { return m_enable; }
	private:
		bool m_enable = true; // por padrão, o sistema está habilitado
	};
}