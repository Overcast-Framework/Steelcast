#pragma once
#include <iostream>
#include <Math/Vector.h>

class SCVertex
{
public:
	SCVector3f Position;
	SCVector3f Normal;
	SCVector2f TexCoord;

	SCVertex(SCVector3f pos, SCVector3f normal, SCVector2f texCoord) : Position(pos), Normal(normal), TexCoord(texCoord) {}
	SCVertex() = default;
};