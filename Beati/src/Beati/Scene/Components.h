#pragma once
#include <string>
#include "glm/glm.hpp"
#include "Beati/Core/UUID.h"
#include "Beati/Renderer/Texture.h"
#include "SceneCamera.h"


namespace Beati
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::mat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::vec3 GetTranslation() const
		{
			return Translation;
		}
	};
		
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {
		}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct LineRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 p0{ 0.0f, 0.0f, 0.0f };
		glm::vec3 p1{ 1.0f, 1.0f, 0.0f };
		LineRendererComponent() = default;
		LineRendererComponent(const LineRendererComponent&) = default;
		LineRendererComponent(const glm::vec4& color)
			: Color(color) {
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;
		bool IsFocused = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct InputMovementComponent
	{
		float Speed = 5.0f;  // unidades/segundo
		
		InputMovementComponent() = default;
		InputMovementComponent(const InputMovementComponent&) = default;
		InputMovementComponent(float speed) : Speed(speed) {}
	};

	// Physics
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
		// To init
		Rigidbody2DComponent(BodyType type, bool fixedRotation = false)
			: Type(type), FixedRotation(fixedRotation) {
		}
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 1.0f, 1.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
		BoxCollider2DComponent(glm::vec2 size, float density = 1.0f, float friction = 0.5f, float restitution = 0.0f)
			: Size(size), Density(density), Friction(friction), Restitution(restitution) {
		}
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	struct Particle
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };  // Posición relativa al emisor
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };  // Velocidad inicial
		float Lifetime = 1.0f;                      // Tiempo de vida restante (en segundos)
		float MaxLifetime = 1.0f;                   // Tiempo de vida máximo (para fade)
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };  // Color (con alpha para fade)
		float Size = 0.1f;                          // Tamaño (escala del quad)
		float GravityScale = 1.0f;                  // Escala de gravedad (reutilizando de PhysicComponent)
		float AirFriction = 0.1f;                   // Fricción de aire
	};

	struct ParticleEmitterComponent
	{
		std::vector<Particle> Particles;            // Lista de partículas activas
		float EmissionRate = 10.0f;                 // Partículas por segundo
		int MaxParticles = 100;                     // Máximo de partículas activas
		float ParticleLifetime = 2.0f;              // Lifetime base por partícula
		glm::vec3 InitialVelocityMin = { -1.0f, 0.0f, 0.0f };  // Velocidad inicial mínima (para random)
		glm::vec3 InitialVelocityMax = { 1.0f, 2.0f, 0.0f };   // Velocidad inicial máxima
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };  // Color base
		float Size = 0.1f;                          // Tamaño base
		float GravityScale = 1.0f;                  // Gravedad global para partículas
		float AirFriction = 0.1f;                   // Fricción global
		float Accumulator = 0.0f;                   // Acumulador para emisión fraccional

		bool IsEmitting = true;                     // Flag para pausar emisión

		ParticleEmitterComponent() = default;
		ParticleEmitterComponent(const ParticleEmitterComponent&) = default;
	};

	class Entity;
	struct RopeComponent
	{
		int NumSegments = 10;                  // Número de segmentos (más = más flexible, pero más costo)
		float SegmentLength = 0.5f;            // Longitud por segmento
		float SegmentWidth = 0.05f;            // Ancho (para collider)

		glm::vec3 EndOffset = { 0.0f, -5.0f, 0.0f };  // Offset del final de la soga relativo al ancla (dirección inicial)
		bool IsAnchored = true;                // Si el primer segmento está fijo (Static)
		glm::vec4 Color = { 0.5f, 0.3f, 0.1f, 1.0f };  // Color para dibujo (marrón como soga)

		float Density = 1.0f;
		float Friction = 0.3f;
		float Restitution = 0.1f;

		std::vector<Entity> Segments;          // Almacena entidades de segmentos (creadas en runtime)
		void* RuntimeFirstBody = nullptr;      // Storage para el primer body (ancla)

		RopeComponent() = default;
		RopeComponent(const RopeComponent&) = default;
	};
}
