#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"
#include "example_layer.h"

player::player() : m_timer(0.0f), m_speed(2.0f)
{
}

player::~player()
{
}

void player::initialise(engine::ref<engine::game_object> object)
{
		m_object = object;
		m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
		m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
		m_object->animated_mesh()->set_default_animation(1);
	
}

void player::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));

	//Intial forward (z axis - 1)
	if (m_axis == 0) {
		if (is_turn == false) {
			if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
				m_object->set_position((m_object->position() + glm::vec3{ -1.f, 0.f, 0.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
				m_object->set_position((m_object->position() + glm::vec3{ 1.f, 0.f, 0.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_W)) // jump
				jump();
		}
	}

	//Initial right (x axis + 1)
	if (m_axis == 1) {
		if (is_turn == false) {
			if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
				m_object->set_position((m_object->position() + glm::vec3{ 0.f, 0.f, 1.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
				m_object->set_position((m_object->position() + glm::vec3{ 0.f, 0.f, -1.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_W)) // jump
				jump();
		}
	}

	//Initial left (x axis - 1)
	if (m_axis == 2) {
		if (is_turn == false) {
			if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
				m_object->set_position((m_object->position() + glm::vec3{ 0.f, 0.f, -1.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
				m_object->set_position((m_object->position() + glm::vec3{ 0.f, 0.f, 1.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_W)) // jump
				jump();
		}
	}

	//Intial backwards (z axis + 1)
	if (m_axis == 3) {
		if (is_turn == false) {
			if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
				m_object->set_position((m_object->position() + glm::vec3{ 1.f, 0.f, 0.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
				m_object->set_position((m_object->position() + glm::vec3{ -1.f, 0.f, 0.f }) += m_object->forward() * m_speed * (float)time_step);
			else if (engine::input::key_pressed(engine::key_codes::KEY_W)) // jump
				jump();
		}
	}

	m_object->animated_mesh()->on_update(time_step);

	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			m_object->animated_mesh()->switch_root_movement(false);

			m_object->animated_mesh()->switch_animation(m_object->animated_mesh()->default_animation());
			m_speed = 2.0f;
		}
	}

}

void player::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void player::update_camera(engine::perspective_camera& camera)
{
	glm::vec3 cam_pos = m_object->position() - glm::normalize(m_object->forward()) * 3.0f;
	cam_pos.y += 2.f;

	glm::vec3 cam_look_at = m_object->position() + glm::normalize(m_object->forward()) * 6.0f;
	cam_look_at.y = 0.f;
	camera.set_view_matrix(cam_pos, cam_look_at);
}

void player::jump()
{
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);
	m_speed = 0.0f;

	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
}
