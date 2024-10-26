#pragma once
#include <Graphics/Vertex.h>
#include <vector>

class SCMaterial { 
public:
	virtual ~SCMaterial() = default;
};

class Mesh
{
public:
	std::vector<SCVertex> Vertices;
	std::vector<unsigned int> Indices;
	SCMaterial Material;

	virtual ~Mesh() = default;
};