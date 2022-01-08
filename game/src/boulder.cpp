#include "pch.h"
#include "boulder.h"

boulder::boulder() : m_timer(0.0f), m_speed(2.f)
{
}

boulder::~boulder()
{
}

void boulder::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	glm::vec3 position(0.f, 2.f, 20.f);
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(position);
}

void boulder::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);
}
