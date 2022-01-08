#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "pickup.h"
#include "pickup2.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{
		m_fog = false;
		glm::vec3 m_fog_colour = glm::vec3(0.5f);
		int m_fog_factor_type = 2;

		// Hide the mouse and lock it inside the window
		//engine::input::anchor_mouse(true);
		engine::application::window().hide_mouse_cursor();

		// Initialise audio and play background music
		m_audio_manager = engine::audio_manager::instance();
		m_audio_manager->init();
		//m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce"); // Royalty free sound from freesound.org
		//m_audio_manager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");  // Royalty free music from http://www.nosoapradio.us/
		m_audio_manager->load_sound("assets/audio/DST-NoReturn.mp3", engine::sound_type::track, "music"); // Royalty free music from http://www.nosoapradio.us/
		m_audio_manager->load_sound("assets/audio/395428__ihitokage__bone-break-crack-5.ogg", engine::sound_type::event, "death"); // Royalty free sound from freesound.org
		m_audio_manager->load_sound("assets/audio/414071__inspectorj__dialogue-pained-yelp-loud-c.wav", engine::sound_type::event, "death_scream"); // Royalty free sound from freesound.org
		m_audio_manager->load_sound("assets/audio/431329__someguy22__8-bit-powerup.wav", engine::sound_type::event, "powerup"); // Royalty free sound from freesound.org
		m_audio_manager->load_sound("assets/audio/220173__gameaudio__spacey-1up-power-up.wav", engine::sound_type::event, "powerup2"); // Royalty free sound from freesound.org
		m_audio_manager->load_sound("assets/audio/325817__vincepest11__engine-running-by-exhaust-bmw-118i.wav", engine::sound_type::spatialised, "jeep"); // Royalty free sound from freesound.org
		m_audio_manager->play("music");


		// Initialise the shaders, materials and lights
		auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
		auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
		auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

		m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.25f;
		m_directionalLight.DiffuseIntensity = 0.6f;
		m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

		m_pointLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_pointLight.AmbientIntensity = 1.f;
		m_pointLight.DiffuseIntensity = 1.f;
		m_pointLight.Position = glm::vec3(13.f, 2.f, -25.f);

		m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_spotLight.AmbientIntensity = 1.f;
		m_spotLight.DiffuseIntensity = 1.f;
		m_spotLight.Position = glm::vec3(-17.f, 3.f, -25.f);
		m_spotLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
		m_spotLight.Cutoff = 0.5f;
		m_spotLight.Attenuation.Constant = 1.0f;
		m_spotLight.Attenuation.Linear = 0.1f;
		m_spotLight.Attenuation.Exp = 0.01f;


		// set color texture unit
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
		m_directionalLight.submit(animated_mesh_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity",
			1.f);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 10.f);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);

		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);

		std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
		std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
		m_directionalLight.submit(mesh__material_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity",
			1.f);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);

		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);

		std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
		m_directionalLight.submit(mesh_lighting_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity",
			0.5f);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);

		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);


		std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
		std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
			glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
			(float)engine::application::window().height()));
		m_material = engine::material::create(1.0f, glm::vec3(0.4f, 0.26f, 0.13f), glm::vec3(0.4f, 0.26f, 0.13f),
			glm::vec3(0.4f, 0.26f, 0.13f), 1.0f);
		m_fog_material = engine::material::create(1.0f, glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.3f, 0.3f, 0.3f),
			glm::vec3(0.3f, 0.3f, 0.3f), 1.0f);
		m_lightsource_material = engine::material::create(1.f, m_pointLight.Color, m_pointLight.Color, m_pointLight.Color, 1.0f);
		m_tetrahedron_material = engine::material::create(32.0f,
			glm::vec3(1.0f, 0.5f, 0.5f),
			glm::vec3(1.0f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			1.0f);

		// Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
		m_skybox = engine::skybox::create(50.f,
			{ engine::texture_2d::create("assets/textures/skybox/Daylight Box_Front.bmp", true),
			  engine::texture_2d::create("assets/textures/skybox/Daylight Box_Right.bmp", true),
			  engine::texture_2d::create("assets/textures/skybox/Daylight Box_Back.bmp", true),
			  engine::texture_2d::create("assets/textures/skybox/Daylight Box_Left.bmp", true),
			  engine::texture_2d::create("assets/textures/skybox/Daylight Box_Top.bmp", true),
			  engine::texture_2d::create("assets/textures/skybox/Daylight Box_Bottom.bmp", true)
			});

		engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
		m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
		m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
		m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
		m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
		m_skinned_mesh->switch_root_movement(false);

		engine::game_object_properties mannequin_props;
		mannequin_props.animated_mesh = m_skinned_mesh;
		mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
		mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
		mannequin_props.type = 0;
		mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
		m_mannequin = engine::game_object::create(mannequin_props);

		m_player.initialise(m_mannequin);
		m_player_box.set_box(mannequin_props.bounding_shape.x * mannequin_props.scale.x, mannequin_props.bounding_shape.y * mannequin_props.scale.x, mannequin_props.bounding_shape.z * mannequin_props.scale.x, mannequin_props.position);


		// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
		std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/grass_terrain.png", false) };
		engine::ref<engine::terrain> terrain_shape = engine::terrain::create(5.f, 0.5f, 5.f);
		engine::game_object_properties terrain_props;
		terrain_props.meshes = { terrain_shape->mesh() };
		terrain_props.textures = terrain_textures;
		terrain_props.is_static = true;
		terrain_props.type = 0;
		terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
		terrain_props.restitution = 0.92f;
		m_terrain = engine::game_object::create(terrain_props);

		// Load the jeep model. Create a jeep object. Set its properties
		engine::ref <engine::model> jeep_model = engine::model::create("assets/models/static/untitled.obj");
		engine::game_object_properties jeep_props;
		jeep_props.meshes = jeep_model->meshes();
		jeep_props.textures = jeep_model->textures();
		float jeep_scale = 1.f;
		jeep_props.position = { 0.5f,0.5f, -13.2f };
		jeep_props.scale = glm::vec3(jeep_scale);
		//jeep_props.bounding_shape = jeep_model->size() / 2.f * jeep_scale;
		jeep_props.bounding_shape = jeep_model->size() * 1.2f;
		m_jeep = engine::game_object::create(jeep_props);

		m_jeep->set_offset(jeep_model->offset());
		m_jeep_box.set_box(jeep_props.bounding_shape.x * jeep_scale, jeep_props.bounding_shape.y
			* jeep_scale, jeep_props.bounding_shape.z * jeep_scale, jeep_props.position + glm::vec3{ 0.2f, 0.f, 0.f });

		//Rock obstacles

		// Load the rock model. Create a jeep object. Set its properties
		engine::ref <engine::model> rock_model = engine::model::create("assets/models/static/Rock_6.OBJ");
		engine::game_object_properties rock_props;
		rock_props.meshes = rock_model->meshes();

		std::vector<engine::ref<engine::texture_2d>> rock_textures =
		{ engine::texture_2d::create("assets/textures/Rock_Texture2.jpg", false) };

		rock_props.textures = rock_textures;
		float rock_scale = 1.f;
		rock_props.position = { 0.f, 0.f, -3.f };
		rock_props.scale = glm::vec3(rock_scale);
		rock_props.bounding_shape = rock_model->size() / 2.f * rock_scale;
		m_rock = engine::game_object::create(rock_props);

		m_rock->set_offset(rock_model->offset());
		m_rock_box.set_box(rock_props.bounding_shape.x * rock_scale, rock_props.bounding_shape.y
			* rock_scale, rock_props.bounding_shape.z * rock_scale, rock_props.position);

		m_rock_box3.set_box(rock_props.bounding_shape.x * rock_scale, rock_props.bounding_shape.y
			* rock_scale, rock_props.bounding_shape.z * rock_scale, glm::vec3{ 8.f, 0.5f, -26.f });

		//Rock 2
		engine::ref <engine::model> rock_model2 = engine::model::create("assets/models/static/Rock_6.OBJ");
		engine::game_object_properties rock_props2;
		rock_props2.meshes = rock_model2->meshes();

		std::vector<engine::ref<engine::texture_2d>> rock_textures2 =
		{ engine::texture_2d::create("assets/textures/Rock_Texture2.jpg", false) };

		rock_props2.textures = rock_textures2;
		float rock_scale2 = 1.f;
		rock_props2.position = { -2.f, 0.f, -8.f };
		rock_props2.scale = glm::vec3(rock_scale2);
		rock_props2.bounding_shape = rock_model2->size() / 2.f * rock_scale2;
		m_rock2 = engine::game_object::create(rock_props2);

		m_rock2->set_offset(rock_model2->offset());
		m_rock_box2.set_box(rock_props2.bounding_shape.x * rock_scale2, rock_props2.bounding_shape.y
			* rock_scale2, rock_props2.bounding_shape.z * rock_scale2, rock_props2.position);

		m_rock_box4.set_box(rock_props.bounding_shape.x * rock_scale, rock_props.bounding_shape.y
			* rock_scale, rock_props.bounding_shape.z * rock_scale, glm::vec3{ 17.f, 0.5f, -24.f });


		//Tetrahedron properties
		std::vector<glm::vec3> tetrahedron_vertices;
		tetrahedron_vertices.push_back(glm::vec3(0.f, 1.f, 0.f));//0
		tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 1.f));//1
		tetrahedron_vertices.push_back(glm::vec3(-1.f, 0.f, -0.f)); //2
		tetrahedron_vertices.push_back(glm::vec3(1.f, 0.f, -1.f)); //3

		std::vector<engine::ref<engine::texture_2d>> tetrahedron_textures =
		{ engine::texture_2d::create("assets/textures/rock_texture.jpg", false) };

		engine::ref<engine::tetrahedron> tetrahedron_shape = engine::tetrahedron::create(tetrahedron_vertices);
		engine::game_object_properties tetrahedron_props;
		tetrahedron_props.position = { -3.f, 0.5f, 3.f };
		tetrahedron_props.meshes = { tetrahedron_shape->mesh() };
		tetrahedron_props.bounding_shape = glm::vec3(1.f);
		tetrahedron_props.textures = tetrahedron_textures;
		m_tetrahedron = engine::game_object::create(tetrahedron_props);

		//Boudning boxes for the boundary
		m_boundary_box.set_box(1.f, 1.f, 25.f, tetrahedron_props.position + glm::vec3{ 0.f, 0.f, -12.f });
		m_boundary_box2.set_box(1.f, 1.f, 25.f, tetrahedron_props.position + glm::vec3{ 6.f, 0.f, -12.f });

		m_boundary_box3.set_box(30.f, 1.f, 1.f, tetrahedron_props.position + glm::vec3{ -15.f, 0.f, -25.f });
		m_boundary_box4.set_box(30.f, 1.f, 1.f, tetrahedron_props.position + glm::vec3{ -15.f, 0.f, -31.f });

		m_boundary_box5.set_box(16.f, 1.f, 1.f, tetrahedron_props.position + glm::vec3{ 14.f, 0.f, -25.f });
		m_boundary_box6.set_box(16.f, 1.f, 1.f, tetrahedron_props.position + glm::vec3{ 14.f, 0.f, -31.f });

		//Bounding box for when the player fails to pick a turn
		m_boundary_box_death.set_box(7.f, 1.f, 1.f, glm::vec3{ -2.f, 0.f, -28.f });
		m_boundary_box_deathL.set_box(1.f, 1.f, 10.f, glm::vec3{ -39.f, 0.f, -22.f });
		m_boundary_box_deathR.set_box(1.f, 1.f, 10.f, glm::vec3{ 28.f, 0.f, -28.f });



		//pentahedron properties
		std::vector<glm::vec3> pentahedron_vertices;
		//pentahedron_vertices.push_back(glm::vec3(0.f, 1.f, 0.f));//0
		//pentahedron_vertices.push_back(glm::vec3(0.f, 0.f, 1.f)); //1
		//pentahedron_vertices.push_back(glm::vec3(-1.f, 0.f, 0.f)); //2
		//pentahedron_vertices.push_back(glm::vec3(0.f, 0.f, -1.f)); //4
		//pentahedron_vertices.push_back(glm::vec3(1.f, 0.f, 0.f)); //4
		pentahedron_vertices.push_back(glm::vec3(0.f, 1.f, 0.f)); //0
		pentahedron_vertices.push_back(glm::vec3(1.f, 0.f, 1.f)); //1
		pentahedron_vertices.push_back(glm::vec3(-1.f, 0.f, 1.f)); //2
		pentahedron_vertices.push_back(glm::vec3(-1.f, 0.f, -1.f)); //4
		pentahedron_vertices.push_back(glm::vec3(1.f, 0.f, -1.f)); //4

		std::vector<engine::ref<engine::texture_2d>> pentahedron_textures =
		{ engine::texture_2d::create("assets/textures/Pyramid.jpg", false) };

		engine::ref<engine::pentahedron> pentahedron_shape = engine::pentahedron::create(pentahedron_vertices);
		engine::game_object_properties pentahedron_props;
		pentahedron_props.position = { 0.f, 0.5f, -40.f };
		pentahedron_props.meshes = { pentahedron_shape->mesh() };
		pentahedron_props.bounding_shape = glm::vec3(1.f);
		pentahedron_props.textures = pentahedron_textures;
		m_pentahedron = engine::game_object::create(pentahedron_props);




		//Cuboid properties

		engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
		engine::ref<engine::texture_2d> pickup_texture =
			engine::texture_2d::create("assets/textures/woodTexture.jpg", true);
		engine::game_object_properties pickup_props;
		pickup_props.position = { 2.f, 1.f, -12.5f };
		pickup_props.meshes = { pickup_shape->mesh() };
		pickup_props.textures = { pickup_texture };
		m_pickup = pickup::create(pickup_props);

		m_block_box.set_box(pickup_props.bounding_shape.x * pickup_props.scale.x, pickup_props.bounding_shape.y
			* pickup_props.scale.y, pickup_props.bounding_shape.z * pickup_props.scale.z, pickup_props.position + glm::vec3{ 0.f, 0.f , 0.5f });

		//bounding boxes for blocks placed in scene
		m_block_box2.set_box(1.f, 1.f, 1.f, glm::vec3{ -24.f, 0.5f, -25.f });
		m_block_box3.set_box(1.f, 1.f, 1.f, glm::vec3{ -27.f, 0.5f, -26.f });

		m_block_box4.set_box(1.f, 1.f, 1.f, glm::vec3{ 13.f, 1.f, -25.f });

		//Power Up protperties
		//coin boost
		engine::ref<engine::cuboid> powerup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
		engine::ref<engine::texture_2d> powerup_texture =
			engine::texture_2d::create("assets/textures/question_mark.png", true);
		engine::game_object_properties powerup_props;
		powerup_props.position = { 0.f, 1.f, -18.f };
		powerup_props.meshes = { powerup_shape->mesh() };
		powerup_props.textures = { powerup_texture };
		m_powerup = pickup::create(powerup_props);
		m_powerup->init();

		//Invincibility
		engine::ref<engine::cuboid> powerup_shape2 = engine::cuboid::create(glm::vec3(0.5f), false);
		engine::ref<engine::texture_2d> powerup_texture2 =
			engine::texture_2d::create("assets/textures/question_mark.png", true);
		engine::game_object_properties powerup_props2;
		powerup_props2.position = { -20.f, 1.f, -25.f };
		powerup_props2.meshes = { powerup_shape2->mesh() };
		powerup_props2.textures = { powerup_texture2 };
		m_powerup2 = pickup2::create(powerup_props2);
		m_powerup2->init2();

		//bouding boxes for power ups 
		m_powerup_box.set_box(1.f, 1.f, 1.f, powerup_props.position - glm::vec3{ 0.f, 0.5f, 0.f });

		m_powerup_box2.set_box(1.f, 1.f, 1.f, glm::vec3{ -20.f, 0.5f , -25.f });
		m_powerup_box2_end.set_box(1.f, 1.f, 6.f, glm::vec3{ -31.f, 0.5f , -25.f });


		//Sphere properties 
		engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
		engine::ref<engine::texture_2d> sphere_texture =
			engine::texture_2d::create("assets/textures/boulder_texture.jpg", true);
		engine::game_object_properties sphere_props;
		sphere_props.position = { 0.f, 5.f, 2.f };
		sphere_props.meshes = { sphere_shape->mesh() };
		sphere_props.type = 1;
		sphere_props.bounding_shape = glm::vec3(0.5f);
		sphere_props.restitution = 0.92f;
		sphere_props.mass = 0.000001f;
		sphere_props.textures = { sphere_texture };
		m_ball = engine::game_object::create(sphere_props);

		//boulder properties
		engine::ref<engine::model> boulder_shape = engine::model::create("assets/models/static/only_quad_sphere.3ds");
		engine::ref<engine::texture_2d> boulder_texture =
			engine::texture_2d::create("assets/textures/boulder_texture.jpg", true);
		engine::game_object_properties boulder_props;
		float boulder_scale = 1.f;
		boulder_props.position = { 0.f, 2.f, 20.f };
		boulder_props.meshes = boulder_shape->meshes();
		boulder_props.textures = { boulder_texture };
		boulder_props.type = 1;
		boulder_props.scale = glm::vec3(boulder_scale);
		boulder_props.bounding_shape = glm::vec3(4.f);
		boulder_props.restitution = 0.92f;
		boulder_props.mass = 0.000001f;
		m_boulder_shape = engine::game_object::create(boulder_props);

		m_boulder.initialise(m_boulder_shape);

		//boudning box for boulder
		m_boulder_shape->set_offset(boulder_shape->offset());
		m_boulder_box.set_box(4.f, 4.f, 4.f, boulder_props.position - glm::vec3{ -1.f, 2.f, 0.f });

		//bounding box for speed up 
		m_speedup_box.set_box(12.f, 2.f, 1.f, glm::vec3{ -3.f, 0.f, 0.f });
		m_speedup_box2.set_box(1.f, 1.f, 8.f, glm::vec3{ -4.f, 0.f, -27.f });

		//bounding boxes for all path turns
		m_pturn_box.set_box(4.f, 3.f, 4.f, glm::vec3{ 0.f, 0.f, -25.f });
		m_bturn_box.set_box(3.f, 3.f, 1.f, glm::vec3{ 0.f, 0.f, -27.f });

		m_pturn_boxL.set_box(4.f, 3.f, 4.f, glm::vec3{ -36.f, 0.f, -25.f });
		m_bturn_boxL.set_box(1.f, 3.f, 3.f, glm::vec3{ -39.f, 0.f, -25.f });

		m_pturn_boxR.set_box(4.f, 3.f, 4.f, glm::vec3{ 25.f, 0.f, -25.f });
		m_bturn_boxR.set_box(1.f, 3.f, 3.f, glm::vec3{ 28.f, 0.f, -25.f });

		//bounding box for activating fog effect
		m_fog_box.set_box(1.f, 1.f, 8.f, glm::vec3{ 4.f, 0.f, -27.f });


		m_game_objects.push_back(m_terrain);
		m_game_objects.push_back(m_ball);
		//m_game_objects.push_back(m_cow);
		//m_game_objects.push_back(m_tree);
		//m_game_objects.push_back(m_pickup);
		m_physics_manager = engine::bullet_manager::create(m_game_objects);

		m_text_manager = engine::text_manager::create();

		m_skinned_mesh->switch_animation(1);

		m_billboard = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);

		hs_score = std::to_string(h_score);
	

	
		//Intro screen
		m_Intro = Intro::create("assets/textures/title_screen.png", 4.0f, 1.6f, 0.9f);
		m_Intro->activate();
	
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	if (C_state == Active) {

		m_player.update_camera(m_3d_camera);

		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

		m_powerup->update(m_3d_camera.position(), time_step);

		m_powerup2->update2(m_3d_camera.position(), time_step);

		//Player update
		glm::vec3 pos = m_player.object()->position();
		m_player.on_update(time_step);
		m_player_box.on_update(m_player.object()->position());

		//obstacle collisions
		if (is_invincible == false) {
			if (m_rock_box.collision(m_player_box))
				m_player.object()->set_position(pos);
			if (m_rock_box2.collision(m_player_box))
				m_player.object()->set_position(pos);
			if (m_rock_box3.collision(m_player_box))
				m_player.object()->set_position(pos);
			if (m_rock_box4.collision(m_player_box))
				m_player.object()->set_position(pos);
			if (m_block_box.collision(m_player_box))
				m_player.object()->set_position(pos + glm::vec3{ 0.f, 0.f, 3.f });
			if (m_jeep_box.collision(m_player_box)) {
				m_audio_manager->stop("music");
				m_player.m_speed = 2.f;
				m_boulder.m_speed = 2.f;
				m_player.m_axis = 0;
				m_audio_manager->play("death_scream");
				m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
				m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
				m_boulder.object()->set_position(glm::vec3(0.f, 2.f, 20.f));
				m_player.object()->set_position(glm::vec3(0.f, 0.5, 10.f));
				if (i_score > h_score) {
					h_score = i_score;
					hs_score = std::to_string(h_score);
				}
				i_score = i_score - i_score;
				c_speed = 2;
				m_audio_manager->play("music");
			}
			if (m_block_box2.collision(m_player_box))
				m_player.object()->set_position(pos);
			if (m_block_box3.collision(m_player_box))
				m_player.object()->set_position(pos);
			if (m_block_box4.collision(m_player_box))
				m_player.object()->set_position(pos);
		}

		//boundary collisions
		if (m_boundary_box.collision(m_player_box))
			m_player.object()->set_position(pos + glm::vec3{ 0.f, 0.f, -0.05f });
		if (m_boundary_box2.collision(m_player_box))
			m_player.object()->set_position(pos + glm::vec3{ 0.f, 0.f, -0.05f });
		if (m_boundary_box3.collision(m_player_box))
			m_player.object()->set_position(pos + glm::vec3{ -0.05f, 0.f, 0.f });
		if (m_boundary_box4.collision(m_player_box))
			m_player.object()->set_position(pos + glm::vec3{ -0.05f, 0.f, 0.f });
		if (m_boundary_box5.collision(m_player_box))
			m_player.object()->set_position(pos + glm::vec3{ 0.05f, 0.f, 0.f });
		if (m_boundary_box6.collision(m_player_box))
			m_player.object()->set_position(pos + glm::vec3{ 0.05f, 0.f, 0.f });
		if (m_boundary_box_death.collision(m_player_box)) {
			m_player.m_speed = 2.f;
			m_boulder.m_speed = 2.f;
			m_player.is_turn = false;
			m_player.m_axis = 0;
			m_audio_manager->play("death_scream");
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_boulder.object()->set_position(glm::vec3(0.f, 2.f, 20.f));
			m_player.object()->set_position(glm::vec3(0.f, 0.5, 10.f));
			if (i_score > h_score) {
				h_score = i_score;
				hs_score = std::to_string(h_score);
			}
			c_speed = 2;
			i_score = i_score - i_score;
		}
		if (m_boundary_box_deathL.collision(m_player_box)) {
			m_player.m_speed = 2.f;
			m_boulder.m_speed = 2.f;
			m_player.is_turn = false;
			m_player.m_axis = 0;
			m_audio_manager->play("death_scream");
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_boulder.object()->set_position(glm::vec3(0.f, 2.f, 20.f));
			m_player.object()->set_position(glm::vec3(0.f, 0.5, 10.f));
			if (i_score > h_score) {
				h_score = i_score;
				hs_score = std::to_string(h_score);
			}
			c_speed = 2;
			i_score = i_score - i_score;
		}
		if (m_boundary_box_deathR.collision(m_player_box)) {
			m_fog = false;
			m_player.m_speed = 2.f;
			m_boulder.m_speed = 2.f;
			m_player.is_turn = false;
			m_player.m_axis = 0;
			m_audio_manager->play("death_scream");
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_boulder.object()->set_position(glm::vec3(0.f, 2.f, 20.f));
			m_player.object()->set_position(glm::vec3(0.f, 0.5, 10.f));
			if (i_score > h_score) {
				h_score = i_score;
				hs_score = std::to_string(h_score);
			}
			c_speed = 2;
			i_score = i_score - i_score;
		}

		//Speed up collisions
		if (m_speedup_box.collision(m_player_box)) {
			m_player.m_speed = 3.f;
			c_speed = 3;
			m_audio_manager->play_spatialised_sound("jeep", m_3d_camera.position(), glm::vec3(0.5f, 0.5f, -13.2f));
			m_billboard->activate(glm::vec3(1.f, 1.f, -13.2f), 2.f, 6.f);
		}

		if (m_speedup_box2.collision(m_player_box)) {
			m_player.m_speed = 4.f;
			c_speed = 4;
		}

		//power up collisions
		if (m_powerup_box.collision(m_player_box)) {
			m_audio_manager->play("powerup");
			i_score = i_score + 20;
		}

		if (m_powerup_box2.collision(m_player_box)) {
			is_invincible = true;
			m_audio_manager->play("powerup2");
		}
		if (m_powerup_box2_end.collision(m_player_box))
			is_invincible = false;

		//fog collision
		if (m_fog_box.collision(m_player_box)) {
			m_fog = true;
			m_player.m_speed = 4.f;
			c_speed = 4;
		}

		//Score 
		i_score++;
		d_score = std::to_string(i_score);

		//Speed
		sc_speed = std::to_string(c_speed);


		//Boulder update
		glm::vec3 bpos = m_boulder.object()->position();
		m_boulder.on_update(time_step);
		m_boulder_box.on_update(m_boulder.object()->position() - glm::vec3{ 0.f, 2.f, 0.f });

		//Boulder collisions
		if ((m_bturn_box.collision(m_boulder_box)) && (m_player.m_axis == 1)) {
			m_boulder.object()->set_forward(glm::vec3(-1.f, 0.f, 0.f));
		}
		if ((m_bturn_box.collision(m_boulder_box)) && (m_player.m_axis == 2)) {
			m_boulder.object()->set_forward(glm::vec3(1.f, 0.f, 0.f));
		}
		if ((m_bturn_boxL.collision(m_boulder_box)) && (m_player.m_axis == 0)) {
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
		}
		if ((m_bturn_boxL.collision(m_boulder_box)) && (m_player.m_axis == 3)) {
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, 1.f));
		}
		if ((m_bturn_boxR.collision(m_boulder_box)) && (m_player.m_axis == 0)) {
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
		}
		if ((m_bturn_boxR.collision(m_boulder_box)) && (m_player.m_axis == 3)) {
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, 1.f));
		}
		if (m_player_box.collision(m_boulder_box)) {
			is_invincible = false;
			m_fog = false;
			m_boulder.m_speed = 2.f;
			m_player.m_speed = 2.f;
			m_player.is_turn = false;
			m_player.m_axis = 0;
			m_audio_manager->play("death");
			m_audio_manager->play("death_scream");
			m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			m_boulder.object()->set_position(glm::vec3(0.f, 2.f, 20.f));
			m_player.object()->set_position(glm::vec3(0.f, 0.5, 10.f));
			if (i_score > h_score) {
				h_score = i_score;
				hs_score = std::to_string(h_score);
			}
			c_speed = 2;
			i_score = i_score - i_score;
		}
		if (m_speedup_box.collision(m_boulder_box)) {
			m_boulder.m_speed = 3.f;
		}
		if (m_speedup_box2.collision(m_boulder_box)) {
			m_boulder.m_speed = 4.f;
		}
		if (m_fog_box.collision(m_boulder_box)) {
			m_boulder.m_speed = 4.f;
		}

		//check_bounce();

		//m_Intro->on_update(time_step);

		m_billboard->on_update(time_step);
	}
} 

void example_layer::on_render()
{
	
		engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
		engine::render_command::clear();

		//const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
		//engine::renderer::begin_scene(m_3d_camera, textured_shader);

		const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
		engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

		// Set up some of the scene's parameters in the shader
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("fog_on", m_fog);

	if (C_state == Active) {

		// Position the skybox centred on the player and render it
		glm::mat4 skybox_tranform(1.0f);
		skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
		for (const auto& texture : m_skybox->textures())
		{
			texture->bind();
		}
		engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

		//Terrain
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				glm::mat4 terrain_transform(1.0f);
				terrain_transform = glm::translate(terrain_transform, glm::vec3(-50.f + (i * 10.f), 0.f, 50.f - (j * 10.f)));
				terrain_transform = glm::scale(terrain_transform, m_terrain->scale());
				engine::renderer::submit(textured_lighting_shader, terrain_transform, m_terrain);
			}
		}

		//Background Pyramids

		glm::mat4 pentahedron_transform(1.0f);
		pentahedron_transform = glm::translate(pentahedron_transform, glm::vec3(0.f, 0.f, -35.f));
		pentahedron_transform = glm::scale(pentahedron_transform, m_pentahedron->scale()*4.f);
		engine::renderer::submit(textured_lighting_shader, pentahedron_transform, m_pentahedron);

		glm::mat4 pentahedron_transform2(1.0f);
		pentahedron_transform2 = glm::translate(pentahedron_transform2, glm::vec3(20.f, 0.f, -10.f));
		pentahedron_transform2 = glm::scale(pentahedron_transform2, m_pentahedron->scale() * 4.f);
		engine::renderer::submit(textured_lighting_shader, pentahedron_transform2, m_pentahedron);

		glm::mat4 pentahedron_transform3(1.0f);
		pentahedron_transform3 = glm::translate(pentahedron_transform3, glm::vec3(-20.f, 0.f, -10.f));
		pentahedron_transform3 = glm::scale(pentahedron_transform3, m_pentahedron->scale() * 4.f);
		engine::renderer::submit(textured_lighting_shader, pentahedron_transform3, m_pentahedron);

		//-----------------------------------------------------------------------------------------------------------------------

		//Bouding box renders
		//m_player_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_boulder_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_rock_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_rock_box2.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_rock_box3.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_rock_box4.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_block_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_block_box2.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_block_box3.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_block_box4.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_powerup_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_powerup_box2.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_powerup_box2_end.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);


		//m_boundary_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box2.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box3.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box4.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box5.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box6.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_boundary_box_death.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box_deathL.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_boundary_box_deathR.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_jeep_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_speedup_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_speedup_box2.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_pturn_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_bturn_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_pturn_boxL.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_bturn_boxL.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_pturn_boxR.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
		//m_bturn_boxR.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//m_fog_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

		//engine::renderer::submit(textured_lighting_shader, m_ball);

		engine::renderer::submit(textured_lighting_shader, m_boulder.object());


		//Game boundary

		for (int i = 0; i < 25; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-3.f, 0.5f, 3.f - i));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 25; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(3.f, 0.5f, 3.f - i));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 10; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-3.f - i, 0.5f, -22.f));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 19; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(3.f + i, 0.5f, -22.f));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 35; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-12.f + i, 0.5f, -28.f));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 15; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-19.f - i, 0.5f, -22.f));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 15; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-19.f - i, 0.5f, -28.f));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 15; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-39.f, 0.5f, -18.f - i));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		for (int i = 0; i < 15; i++) {
			glm::mat4 tetrahedron_transform(1.0f);
			tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(28.f, 0.5f, -18.f - i));
			tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale() * 0.5f);
			engine::renderer::submit(textured_lighting_shader, tetrahedron_transform, m_tetrahedron);
		}

		//Game boundary end--------------------------------------------------------------------------------------------

		//Cave

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(-13.f - (j / 2.f), 0.5f + (i / 2.f), -22.f));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(-13.f - (j / 2.f), 0.5f + (i / 2.f), -28.f));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(-13.f - (j / 2.f), 0.5f + (4 / 2.f), -22.f - (i / 2.f)));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(-13.f - (j / 2.f), 0.5f + (4 / 2.f), -28.f + (i / 2.f)));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 10; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(-13.f - (j / 2.f), 0.5f + (5 / 2.f), -22.5f - (i / 2.f)));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		//Cave end--------------------------------------------------------------------------------------------------------------------------------

		//Centre obstacles

		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position() - glm::vec3(m_pickup->offset().x, 0.f, m_pickup->offset().z) * m_pickup->scale().x);
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform = glm::scale(pickup_transform, m_pickup->scale());
		engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);

		glm::mat4 jeep_transform(1.0f);
		jeep_transform = glm::translate(jeep_transform, m_jeep->position() - glm::vec3(m_jeep->offset().x, 0.f, m_jeep->offset().z) * m_jeep->scale().x);
		jeep_transform = glm::rotate(jeep_transform, -1.5f, glm::vec3(0.f, 1.f, 0.f));
		jeep_transform = glm::scale(jeep_transform, m_jeep->scale());
		engine::renderer::submit(textured_lighting_shader, jeep_transform, m_jeep);

		glm::mat4 rock_transform(1.0f);
		rock_transform = glm::translate(rock_transform, m_rock->position() - glm::vec3(m_rock->offset().x, 0.f, m_rock->offset().z) * m_rock->scale().x);
		rock_transform = glm::rotate(rock_transform, m_rock->rotation_amount(), m_rock->rotation_axis());
		rock_transform = glm::scale(rock_transform, m_rock->scale() * 0.8f);
		engine::renderer::submit(textured_lighting_shader, rock_transform, m_rock);

		glm::mat4 rock_transform2(1.0f);
		rock_transform2 = glm::translate(rock_transform2, m_rock2->position() - glm::vec3(m_rock2->offset().x, 0.f, m_rock2->offset().z) * m_rock2->scale().x);
		rock_transform2 = glm::rotate(rock_transform2, 1.5f, glm::vec3(0.f, 1.f, 0.f));
		rock_transform2 = glm::scale(rock_transform2, m_rock2->scale() * 0.8f);
		engine::renderer::submit(textured_lighting_shader, rock_transform2, m_rock2);

		//Cave obstacles end-------------------------------------------------------------------------------------------------------------------------

		//left section (after cave)

		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform2(1.0f);
		pickup_transform2 = glm::translate(pickup_transform2, glm::vec3{ -24.f, 1.f, -25.f });
		pickup_transform2 = glm::rotate(pickup_transform2, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform2 = glm::scale(pickup_transform2, m_pickup->scale());
		engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform2);

		glm::mat4 tetrahedron_transform2(1.0f);
		tetrahedron_transform2 = glm::translate(tetrahedron_transform2, glm::vec3(-24.f, 1.5f, -25.f));
		tetrahedron_transform2 = glm::scale(tetrahedron_transform2, m_tetrahedron->scale() * 0.5f);
		engine::renderer::submit(textured_lighting_shader, tetrahedron_transform2, m_tetrahedron);



		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform3(1.0f);
		pickup_transform3 = glm::translate(pickup_transform3, glm::vec3{ -27.f, 1.f, -26.f });
		pickup_transform3 = glm::rotate(pickup_transform3, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform3 = glm::scale(pickup_transform3, m_pickup->scale());
		engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform3);

		glm::mat4 tetrahedron_transform3(1.0f);
		tetrahedron_transform3 = glm::translate(tetrahedron_transform3, glm::vec3(-27.f, 1.5f, -26.f));
		tetrahedron_transform3 = glm::scale(tetrahedron_transform3, m_tetrahedron->scale() * 0.5f);
		engine::renderer::submit(textured_lighting_shader, tetrahedron_transform3, m_tetrahedron);

		//left section end-------------------------------------------------------------------------------------------------------------------------

		//right section (fog part)

		glm::mat4 rock_transform3(1.0f);
		rock_transform3 = glm::translate(rock_transform3, glm::vec3{ 8.f, 0.5f, -26.f });
		rock_transform3 = glm::rotate(rock_transform3, m_rock->rotation_amount(), m_rock->rotation_axis());
		rock_transform3 = glm::scale(rock_transform3, m_rock->scale() * 0.8f);
		engine::renderer::submit(textured_lighting_shader, rock_transform3, m_rock);

		glm::mat4 rock_transform4(1.0f);
		rock_transform4 = glm::translate(rock_transform4, glm::vec3{ 17.f, 0.5f, -24.f });
		rock_transform4 = glm::rotate(rock_transform4, 1.5f, glm::vec3(0.f, 1.f, 0.f));
		rock_transform4 = glm::scale(rock_transform4, m_rock2->scale() * 0.8f);
		engine::renderer::submit(textured_lighting_shader, rock_transform4, m_rock2);

		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform4(1.0f);
		pickup_transform4 = glm::translate(pickup_transform4, glm::vec3{ 13.f, 1.f, -25.f });
		pickup_transform4 = glm::rotate(pickup_transform4, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform4 = glm::scale(pickup_transform4, m_pickup->scale());
		engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform4);

		//right section end------------------------------------------------------------------------------------------------------------------------


		//Power Up

		if (m_powerup->active()) {
			m_powerup->textures().at(0)->bind();
			glm::mat4 powerup_transform(1.0f);
			powerup_transform = glm::translate(powerup_transform, m_powerup->position());
			powerup_transform = glm::rotate(powerup_transform, m_powerup->rotation_amount(), m_powerup->rotation_axis());
			powerup_transform = glm::scale(powerup_transform, m_powerup->scale() * 0.75f);
			engine::renderer::submit(textured_lighting_shader, m_powerup->meshes().at(0), powerup_transform);
		}
		if (m_powerup2->active2()) {
			m_powerup2->textures().at(0)->bind();
			glm::mat4 powerup_transform2(1.0f);
			powerup_transform2 = glm::translate(powerup_transform2, m_powerup2->position());
			powerup_transform2 = glm::rotate(powerup_transform2, m_powerup2->rotation_amount(), m_powerup2->rotation_axis());
			powerup_transform2 = glm::scale(powerup_transform2, m_powerup2->scale() * 0.75f);
			engine::renderer::submit(textured_lighting_shader, m_powerup2->meshes().at(0), powerup_transform2);
		}

		//power up end------------------------------------------------------------------------------------------------------------------------------

		m_billboard->on_render(m_3d_camera, textured_lighting_shader);

		engine::renderer::end_scene();

		// Set up material shader. (does not render textures, renders materials instead)
		const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
		engine::renderer::begin_scene(m_3d_camera, material_shader);

		m_material->submit(material_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("fog_on", m_fog);

		//engine::renderer::submit(material_shader, m_ball);

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_spotLight.submit(material_shader, 0);


		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
		m_pointLight.submit(material_shader, 0);

		//ground under cave
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 11; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(-13.f - (i / 2.f), 0.3f, -22.5f - (j / 2.f)));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(material_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		m_fog_material->submit(material_shader);

		//foggy area ground
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 11; j++) {
				m_pickup->textures().at(0)->bind();
				glm::mat4 pickup_transform(1.0f);
				pickup_transform = glm::translate(pickup_transform, glm::vec3(15.f - (i / 2.f), 0.3f, -22.5f - (j / 2.f)));
				pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
				pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.5f);
				engine::renderer::submit(material_shader, m_pickup->meshes().at(0), pickup_transform);
			}
		}

		//engine::renderer::submit(material_shader, m_pentahedron);



		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);

		m_lightsource_material->submit(material_shader);
		engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_spotLight.Position));
		engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), glm::vec3(13.f, 2.f, -25.f)));

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

		//m_tetrahedron_material->submit(material_shader);
		//engine::renderer::submit(material_shader, m_tetrahedron);

		engine::renderer::end_scene();

		const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
		engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
		std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_on", m_fog);

		glm::mat4 aniTransform = glm::mat4(1.0f);

		//engine::renderer::submit(animated_mesh_shader, m_mannequin);
		engine::renderer::submit(animated_mesh_shader, m_player.object());


		engine::renderer::end_scene();

		// Render text
		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(text_shader, "Score: " + d_score, 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

		const auto high_score = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(high_score, "High Score: " + hs_score, 10.f, (float)engine::application::window().height() - 50.f, 0.5f, glm::vec4(1.f, 1.f, 0.f, 1.f));

		const auto current_speed = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(high_score, "Current Speed: " + sc_speed, 10.f, (float)engine::application::window().height() - 75.f, 0.5f, glm::vec4(0.f, 0.f, 1.f, 1.f));


	}

	if (C_state == Menu) {
		engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
		m_Intro->on_render(textured_lighting_shader);
	}

	engine::renderer::end_scene();

} 

void example_layer::on_event(engine::event& event) 
{ 
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		if (e.key_code() == engine::key_codes::KEY_SPACE)
		{
			C_state = Active;
			//m_audio_manager->play("music");
		}
		if (e.key_code() == engine::key_codes::KEY_1)
		{
			m_billboard->activate(glm::vec3(0.f, 5.f, -10.f), 4.f, 4.f);
		}
		//Turn decision for the centre fork
		if (m_pturn_box.collision(m_player_box)) {

			m_player.is_turn = true;

			if (e.key_code() == engine::key_codes::KEY_Q)
			{
				if (m_player.m_axis == 0) {
					m_player.m_axis = 1;
					m_player.object()->set_forward(glm::vec3(-1.f, 0.f, 0.f));
					m_player.is_turn = false;
				}
			}
			else if (e.key_code() == engine::key_codes::KEY_E)
			{
				if (m_player.m_axis == 0) {
					m_player.m_axis = 2;
					m_player.object()->set_forward(glm::vec3(1.f, 0.f, 0.f));
					m_player.is_turn = false;
				}
			}
		}
		//Turn decision for the left fork
		if (m_pturn_boxL.collision(m_player_box)) {

			m_player.is_turn = true;

			if (e.key_code() == engine::key_codes::KEY_Q)
			{
				if (m_player.m_axis == 1) {
					m_player.m_axis = 3;
					m_player.object()->set_forward(glm::vec3(0.f, 0.f, 1.f));
					m_player.is_turn = false;
				}
			}
			else if (e.key_code() == engine::key_codes::KEY_E)
			{
				if (m_player.m_axis == 1) {
					m_player.m_axis = 0;
					m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
					m_player.is_turn = false;
				}
			}
		}
		//Turn decision for the right fork
		if (m_pturn_boxR.collision(m_player_box)) {

			m_player.is_turn = true;

			if (e.key_code() == engine::key_codes::KEY_Q)
			{
				if (m_player.m_axis == 2) {
					m_player.m_axis = 0;
					m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
					m_player.is_turn = false;
				}
			}
			else if (e.key_code() == engine::key_codes::KEY_E)
			{
				if (m_player.m_axis == 2) {
					m_player.m_axis = 3;
					m_player.object()->set_forward(glm::vec3(0.f, 0.f, 1.f));
					m_player.is_turn = false;
				}
			}
		}
		//Make game easier 
		if (e.key_code() == engine::key_codes::KEY_Z)
		{
			if (m_player.m_speed != 2) {
				m_old_speed = m_player.m_speed;
				m_player.m_speed = 2.f;
				m_boulder.m_speed = 2.f;
				c_speed = 2;
			}
		}
		//Make game back to normal difficulty 
		if (e.key_code() == engine::key_codes::KEY_X)
		{
			if (m_player.m_speed == 2.f) {
				m_player.m_speed = m_old_speed;
				m_boulder.m_speed = m_old_speed;
				c_speed = m_old_speed;
			}
		}
		if (e.key_code() == engine::key_codes::KEY_0)
		{
			//m_audio_manager->stop("music");
			//m_player.is_turn = false;
			//is_invincible = false;
			//m_fog = false;
			//m_boulder.m_speed = 2.f;
			//m_player.m_speed = 2.f;
			//m_player.m_axis = 0;
			//m_audio_manager->play("death_scream");
			//m_boulder.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			//m_player.object()->set_forward(glm::vec3(0.f, 0.f, -1.f));
			//m_boulder.object()->set_position(glm::vec3(0.f, 2.f, 20.f));
			//m_player.object()->set_position(glm::vec3(0.f, 0.5, 10.f));
			//if (i_score > h_score) {
			//	h_score = i_score;
			//	hs_score = std::to_string(h_score);
			//}
			//i_score = i_score - i_score;
			//m_audio_manager->play("music");
			C_state = Menu;
		}
    } 
}

void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}


