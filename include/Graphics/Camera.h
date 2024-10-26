#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <DirectXMath.h>

using namespace DirectX;

class Camera3D
{
public:
	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;
	float FOV;

	glm::mat4 ComputeProjectionMatrix(float aspectRatio);
	glm::mat4 ComputeViewMatrix();
	glm::mat4 ComputeProjectionViewMatrix(float aspRatio);

	Camera3D(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
		: Position(position), Target(target), Up(up) {}
};

class DXCamera3D
{
public:
	XMFLOAT3 Position;
	XMFLOAT3 Target;
	XMFLOAT3 Up;
	float FOV = 0;

	XMMATRIX ComputeProjectionMatrix(float aspectRatio);
	XMMATRIX ComputeViewMatrix();
	XMMATRIX ComputeProjectionViewMatrix(float aspectRatio);
	XMMATRIX ComputeMVPMatrix(float aspectRatio, XMMATRIX world);

	DXCamera3D(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up) : Position(pos), Target(target), Up(up) {}
};

inline XMFLOAT3 AddFloat3(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}