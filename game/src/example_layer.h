#pragma once
#include <engine.h>
#include "player.h"
#include "boulder.h"
#include "Intro.h"
#include "billboard.h"
#include "bounding_box.h"

class pickup;

class pickup2;

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	void check_bounce();
	void hexagon_shape();

	int i_score = 0;		//Current score
	std::string d_score;

	int h_score = 0;		//High score
	std::string hs_score;

	int c_speed = 2;		//Current Speed
	std::string sc_speed;

	bool is_invincible = false;

	float m_old_speed = 0;

	enum State				//Menu or game state
	{
		Menu,
		Active
	};

	State C_state = Menu;

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_boulder_shape{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_tetrahedron{};
	engine::ref<engine::game_object>	m_pentahedron{};
	engine::ref<engine::game_object>	m_jeep{};
	engine::ref<engine::game_object>	m_rock{};
	engine::ref<engine::game_object>	m_rock2{};
	player								m_player{};
	boulder								m_boulder{};
	bounding_box						m_player_box;
	bounding_box						m_boulder_box;
	bounding_box						m_rock_box;
	bounding_box						m_rock_box2;
	bounding_box						m_rock_box3;
	bounding_box						m_rock_box4;
	bounding_box						m_jeep_box;
	bounding_box						m_block_box;
	bounding_box						m_block_box2;
	bounding_box						m_block_box3;
	bounding_box						m_block_box4;
	bounding_box						m_powerup_box;
	bounding_box						m_powerup_box2;
	bounding_box						m_powerup_box2_end;
	bounding_box						m_boundary_box; //start walls
	bounding_box						m_boundary_box2; //start walls
	bounding_box						m_boundary_box3; //left walls
	bounding_box						m_boundary_box4; //left walls
	bounding_box						m_boundary_box5; //right walls
	bounding_box						m_boundary_box6; //right walls
	bounding_box						m_boundary_box_death; //box that triggers death if no turn is taken
	bounding_box						m_boundary_box_deathL;
	bounding_box						m_boundary_box_deathR;
	bounding_box						m_pturn_box; //box that allows player to turn
	bounding_box						m_bturn_box;
	bounding_box						m_pturn_boxL;
	bounding_box						m_bturn_boxL;
	bounding_box						m_pturn_boxR;
	bounding_box						m_bturn_boxR;
	bounding_box						m_speedup_box; //box that speeds player and boulder up
	bounding_box						m_speedup_box2;
	bounding_box						m_fog_box; //box that initialises fog

	engine::ref<pickup>					m_pickup{};
	engine::ref<pickup>					m_powerup{};
	engine::ref<pickup2>				m_powerup2{};

	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_lightsource_material{};
	engine::ref<engine::material>		m_tetrahedron_material{};
	engine::ref<engine::material>		m_fog_material{};


	engine::DirectionalLight            m_directionalLight;

	engine::PointLight					m_pointLight;
	uint32_t							num_point_lights = 1;

	engine::SpotLight					m_spotLight;
	uint32_t							num_spot_lights = 1;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	engine::ref<Intro>					m_Intro{};
	engine::ref<billboard>				m_billboard{};

	bool m_fog = false;
	glm::vec3 m_fog_colour;
	int32_t m_fog_factor_type;
};
