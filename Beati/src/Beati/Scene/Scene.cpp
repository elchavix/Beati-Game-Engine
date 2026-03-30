#include "bepch.h"
#include "Scene.h"

#include "Beati/Core/Input.h"
#include "Beati/Core/KeyCodes.h"

#include <glm/glm.hpp>
#include "Beati/Renderer/Renderer2D.h"

#include "Entity.h"

#include "Physics2D.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_revolute_joint.h" // RopeComponent

#include <random>
#include <ImGui.h>

namespace Beati
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		m_EntityMap[uuid] = entity;
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep delta)
	{
		// ---- Input System ----
		{
			auto view = m_Registry.view<TransformComponent, InputMovementComponent>();
			for (auto e : view)
			{
				auto [transform, movement] = view.get<TransformComponent, InputMovementComponent>(e);

				Entity entity = { e, this };

				CameraComponent* camera = nullptr;
				if (entity.HasComponent<CameraComponent>())
				{
					if (camera && !camera->IsFocused)
					{
						continue;
					}
				}
				glm::vec3 movement_direction = glm::vec3(0.0f);

				// Mapea tu sistema de input (Input::IsKeyPressed, etc.)
				if (Input::IsKeyPressed(Key::W)) movement_direction.y += 1.0f;
				if (Input::IsKeyPressed(Key::S)) movement_direction.y -= 1.0f;
				if (Input::IsKeyPressed(Key::A)) movement_direction.x -= 1.0f;
				if (Input::IsKeyPressed(Key::D)) movement_direction.x += 1.0f;

				// Normaliza y aplica velocidad
				if (glm::length(movement_direction) > 0.0f)
				{
					movement_direction = glm::normalize(movement_direction);
					transform.Translation += movement_direction * movement.Speed * (float)delta;
				}
			}
		}

		// ---- Physics 2D ---- 
		if (m_PhysicsWorld)
		{
			m_PhysicsWorld->Step(delta, 6, 2);

			auto view = m_Registry.view<Rigidbody2DComponent, TransformComponent>();
			for (auto e : view)
			{
				auto [rb2d, transform] = view.get<Rigidbody2DComponent, TransformComponent>(e);

				if (rb2d.RuntimeBody)
				{
					b2Body* body = (b2Body*)rb2d.RuntimeBody;
					const auto& position = body->GetPosition();

					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
		}

		// ---- Particle System ----
		{
			auto view = m_Registry.view<TransformComponent, ParticleEmitterComponent>();
			for (auto entity : view)
			{
				auto [transform, emitter] = view.get<TransformComponent, ParticleEmitterComponent>(entity);

				if (emitter.IsEmitting)
				{
					// Emitir nuevas partículas
					emitter.Accumulator += emitter.EmissionRate * (float)delta;
					while (emitter.Accumulator >= 1.0f && emitter.Particles.size() < emitter.MaxParticles)
					{
						Particle newParticle;
						// Randomizar velocidad inicial
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_real_distribution<float> distX(emitter.InitialVelocityMin.x, emitter.InitialVelocityMax.x);
						std::uniform_real_distribution<float> distY(emitter.InitialVelocityMin.y, emitter.InitialVelocityMax.y);
						newParticle.Velocity.x = distX(gen);
						newParticle.Velocity.y = distY(gen);
						newParticle.Position = transform.Translation;  // Posicion absoluta
						newParticle.Lifetime = emitter.ParticleLifetime;
						newParticle.MaxLifetime = emitter.ParticleLifetime;
						newParticle.Color = emitter.Color;
						newParticle.Size = emitter.Size;
						newParticle.GravityScale = emitter.GravityScale;
						newParticle.AirFriction = emitter.AirFriction;

						emitter.Particles.push_back(newParticle);
						emitter.Accumulator -= 1.0f;
					}
				}

				// Actualizar partículas existentes
				for (auto it = emitter.Particles.begin(); it != emitter.Particles.end(); )
				{
					Particle& p = *it;

					// Aplicar física simple (similar a PhysicComponent)
					p.Velocity.y -= 9.81f * p.GravityScale * (float)delta;  // Gravedad
					p.Velocity -= p.Velocity * p.AirFriction * (float)delta;  // Fricción de aire
					p.Position += p.Velocity * (float)delta;  // Mover

					// Actualizar lifetime y fade
					p.Lifetime -= (float)delta;
					if (p.Lifetime <= 0.0f)
					{
						it = emitter.Particles.erase(it);  // Eliminar partícula muerta
						continue;
					}

					// Fade out alpha basado en lifetime restante
					float lifeFactor = p.Lifetime / p.MaxLifetime;
					p.Color.a = lifeFactor;  // Alpha de 1.0 a 0.0

					++it;
				}
			}
		}

		// ---- Render 2D ---- 
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			// Draw sprites
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			// Draw circles
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}

			// Draw lines
			{
				auto view = m_Registry.view<TransformComponent, LineRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, line] = view.get<TransformComponent, LineRendererComponent>(entity);
					Renderer2D::DrawLine(line.p0, line.p1, line.Color, (int)entity);
				}
			}

			// Draw particles
			{
				auto view = m_Registry.view<TransformComponent, ParticleEmitterComponent>();
				for (auto entity : view)
				{
					auto [transform, emitter] = view.get<TransformComponent, ParticleEmitterComponent>(entity);

					for (const auto& p : emitter.Particles)
					{
						glm::mat4 particleTransform = glm::translate(glm::mat4(1.0f), p.Position) *
							glm::scale(glm::mat4(1.0f), glm::vec3(p.Size, p.Size, 1.0f));

						Renderer2D::DrawQuad(particleTransform, p.Color, (int)entity); // TODO: Agregar que si al Emitter se agrega un Sprite dibujar el sprite (circulo o cuadrado con textura)
					}
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnPhysics2DStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			OnPhysics2DBodyCreated(entity);
		}

		// Crear sogas
		auto ropeView = m_Registry.view<RopeComponent>();
		for (auto e : ropeView)
		{
			Entity entity = { e, this };
			OnRopeCreated(entity);
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnPhysics2DBodyCreated(Entity entity)
	{
		if (!m_PhysicsWorld) return;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		
		bodyDef.angle = transform.Rotation.z;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		rb2d.RuntimeBody = body;

		AddFixtureToBody(entity);
	}

	void Scene::AddFixtureToBody(Entity entity)
	{
		if (!m_PhysicsWorld) return;

		if (!entity.HasComponent<Rigidbody2DComponent>()) return;

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		if (rb2d.RuntimeBody == nullptr) return;

		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		auto& transform = entity.GetComponent<TransformComponent>();

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			if (bc2d.RuntimeFixture != nullptr) return;

			b2PolygonShape boxShape;
			float hx = (bc2d.Size.x * transform.Scale.x) / 2.0f;
			float hy = (bc2d.Size.y * transform.Scale.y) / 2.0f;

 			boxShape.SetAsBox(hx, hy, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);
			//boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);

			// TODO: Fix this shit

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = bc2d.Density;
			fixtureDef.friction = bc2d.Friction;
			fixtureDef.restitution = bc2d.Restitution;
			bc2d.RuntimeFixture = body->CreateFixture(&fixtureDef);
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

			if (cc2d.RuntimeFixture != nullptr) return;

			b2CircleShape circleShape;
			circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
			circleShape.m_radius = transform.Scale.x * cc2d.Radius;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = cc2d.Density;
			fixtureDef.friction = cc2d.Friction;
			fixtureDef.restitution = cc2d.Restitution;
			fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
			cc2d.RuntimeFixture = body->CreateFixture(&fixtureDef);
		}
	}

	void Scene::OnRopeCreated(Entity entity)
	{
		if (!m_PhysicsWorld) return;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rope = entity.GetComponent<RopeComponent>();

		glm::vec3 startPos = transform.Translation;
		glm::vec3 segmentStep = rope.EndOffset / (float)rope.NumSegments;
		float segLength = glm::length(segmentStep);
		rope.SegmentLength = segLength;

		// Calcular ángulo: atan2 nos da la dirección del vector. 
		// Sumamos 90 grados (PI/2) porque la caja de Box2D crece en el eje Y local.
		float angle = glm::atan(segmentStep.y, segmentStep.x) - glm::radians(90.0f);
		float overlapFactor = 1.008f;

		Entity prevEntity = entity; // El ancla inicial

		for (int i = 0; i < rope.NumSegments; ++i)
		{
			Entity segEntity = CreateEntity("RopeSegment_" + std::to_string(i));

			auto& segTransform = segEntity.GetComponent<TransformComponent>();
			// POSICIÓN: start + step * i sitúa la unión. Sumamos 0.5 step para el centro.
			segTransform.Translation = startPos + segmentStep * (float(i) + 0.5f);
			segTransform.Scale = { rope.SegmentWidth, rope.SegmentLength * overlapFactor, 1.0f };
			segTransform.Rotation = { 0.0f, 0.0f, angle };

			// FÍSICA: Usamos el tipo dinámico (excepto el primero si está anclado)
			auto type = (i == 0 && rope.IsAnchored) ? Rigidbody2DComponent::BodyType::Static : Rigidbody2DComponent::BodyType::Dynamic;
			auto& rb2d = segEntity.AddComponent<Rigidbody2DComponent>(type);
			rb2d.FixedRotation = false;

			// COLLIDER: Size 1.0 porque ya usamos el Scale del Transform para dimensionar
			segEntity.AddComponent<BoxCollider2DComponent>(
				glm::vec2(1.0f, 1.0f),
				rope.Density,
				rope.Friction,
				rope.Restitution
			);

			// VISUALIZACIÓN: Para ver el ancho en el Draw
			auto& sprite = segEntity.AddComponent<SpriteRendererComponent>();
			sprite.Color = rope.Color;

			// JOINTS: Conectar con el cuerpo anterior
			if (i > 0)
			{
				b2Body* bodyA = (b2Body*)prevEntity.GetComponent<Rigidbody2DComponent>().RuntimeBody;
				b2Body* bodyB = (b2Body*)rb2d.RuntimeBody;

				if (bodyA && bodyB)
				{
					b2RevoluteJointDef jointDef;
					// El punto de anclaje es exactamente donde termina el segmento anterior
					glm::vec3 anchorPos = startPos + segmentStep * (float)i;
					jointDef.Initialize(bodyA, bodyB, b2Vec2(anchorPos.x, anchorPos.y));
					m_PhysicsWorld->CreateJoint(&jointDef);
				}
			}

			rope.Segments.push_back(segEntity);
			prevEntity = segEntity;
		}
	}

	void Scene::OnEvent(Event& e)
	{
		// ---- Input System ----
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& cameraComponent = view.get<CameraComponent>(entity);
				cameraComponent.Camera.OnEvent(e);
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	// --- On Components Added ---
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}
	
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<LineRendererComponent>(Entity entity, LineRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<InputMovementComponent>(Entity entity, InputMovementComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
		OnPhysics2DBodyCreated(entity);
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		AddFixtureToBody(entity);
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
		AddFixtureToBody(entity);
	}

	template<>
	void Scene::OnComponentAdded<ParticleEmitterComponent>(Entity entity, ParticleEmitterComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<RopeComponent>(Entity entity, RopeComponent& component)
	{
		OnRopeCreated(entity);
	}
}

