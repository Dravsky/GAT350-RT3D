#pragma once
#include "Component.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/quaternion.hpp>

namespace nc
{
	class RotateComponent : public Component
	{
	public:
		CLASS_DECLARATION(RotateComponent)

		bool Initialize();
		void Update(float dt) override;
		void ProcessGui();

	public:
		glm::vec3 euler;
	};
}