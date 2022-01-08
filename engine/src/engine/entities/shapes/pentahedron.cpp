#include "pch.h"
#include "pentahedron.h"
#include <engine.h>

engine::pentahedron::pentahedron(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) -
		vertices.at(1)));
	// left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) -
		vertices.at(2)));
	// right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) -
		vertices.at(3)));
	// back normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) -
		vertices.at(4)));
	// bottom normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) -
		vertices.at(4)));

	std::vector<mesh::vertex> pentahedron_vertices
	{
		//front
		//  position     normal           tex coord       
		{ vertices.at(0),    normals.at(0),    { 0.f,  0.f } },
		{ vertices.at(2),    normals.at(0),    { 1.f,  0.f } },
		{ vertices.at(1),    normals.at(0),    { 0.5f, 1.f } },

		//left
		{ vertices.at(0),    normals.at(1),    { 0.f,  0.f } },
		{ vertices.at(3),    normals.at(1),    { 1.f,  0.f } },
		{ vertices.at(2),    normals.at(1),    { 0.5f, 1.f } },

		//right
		{ vertices.at(0),	 normals.at(2),    {0.f,   0.f } },
		{ vertices.at(4),	 normals.at(2),    {1.f,   0.f } },
		{ vertices.at(3),	 normals.at(2),    {0.5f,  1.f } },

		//back
		{ vertices.at(0),	 normals.at(3),    {0.f,   0.f } },
		{ vertices.at(1),	 normals.at(3),    {1.f,   0.f } },
		{ vertices.at(4),	 normals.at(3),    {0.5f,  1.f } },

		//bottom
		{ vertices.at(1),	 normals.at(4),    {0.f,   0.f } },
		{ vertices.at(2),	 normals.at(4),    {1.f,   0.f } },
		{ vertices.at(3),	 normals.at(4),    {1.f,  1.f } },
		{ vertices.at(4),	 normals.at(4),    {0.f,  1.f } },
	};

	const std::vector<uint32_t> pentahedron_indicies
	{
		0, 1, 2,    //front
		3, 4, 5,	//left
		6, 7, 8,	//right
		9, 10, 11,	//back
		12, 13, 14, 12, 14, 15 //bottom
	};

	m_mesh = engine::mesh::create(pentahedron_vertices, pentahedron_indicies);


}

engine::pentahedron::~pentahedron() {}

engine::ref<engine::pentahedron> engine::pentahedron::create(std::vector<glm::vec3>
	vertices)
{
	return std::make_shared<engine::pentahedron>(vertices);
}
