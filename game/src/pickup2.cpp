#include "pickup2.h"

pickup2::pickup2(const engine::game_object_properties props) : engine::game_object(props)
{}

pickup2::~pickup2()
{}

void pickup2::init2()
{
	m_is_active = true;
	set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	set_rotation_amount(0.0f);
}

void pickup2::update2(glm::vec3 c, float dt)
{
	set_rotation_amount(rotation_amount() + dt * 1.5f);

	if (glm::distance(c, position()) < 4.0f) {
		m_is_active = false;
	}

	if (glm::distance(c, position()) > 6.0f) {
		m_is_active = true;
	}
}

engine::ref<pickup2> pickup2::create(const engine::game_object_properties& props)
{
	return std::make_shared<pickup2>(props);
}
