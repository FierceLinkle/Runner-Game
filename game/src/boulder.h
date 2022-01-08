#pragma once
#include <engine.h>
#include "bounding_box.h"

class boulder
{
public:
		boulder();
		~boulder();

		void initialise(engine::ref<engine::game_object> object);
		void on_update(const engine::timestep& time_step);

		engine::ref<engine::game_object> object() const { return m_object;  }

		float m_speed{ 0.f };
private:
		float m_timer;

		engine::ref< engine::game_object> m_object;

};
