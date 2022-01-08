#pragma once
#include <engine.h>

class pickup2 : public engine::game_object {
public:
	pickup2(const engine::game_object_properties props);
	~pickup2();
	void init2();
	void update2(glm::vec3 c, float dt);
	bool active2() { return m_is_active; }

	static engine::ref<pickup2> create(const engine::game_object_properties& props);
private:
	bool m_is_active;		// Indicates if pickup is active
};
