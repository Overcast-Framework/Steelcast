#include <Graphics/Camera.h>

glm::mat4 Camera3D::ComputeProjectionMatrix(float aspectRatio)
{
	return glm::perspectiveFovLH<float>(glm::radians(45.0f), 800, 600, 0.1f, 100.0f);
}

glm::mat4 Camera3D::ComputeViewMatrix()
{
	return glm::lookAt(glm::vec3(Position), glm::vec3(Target), glm::vec3(Up));
}

glm::mat4 Camera3D::ComputeProjectionViewMatrix(float aspectRatio)
{
	return ComputeProjectionMatrix(aspectRatio) * ComputeViewMatrix();
}

XMMATRIX DXCamera3D::ComputeProjectionMatrix(float aspectRatio)
{
	return XMMatrixPerspectiveFovLH(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

XMMATRIX DXCamera3D::ComputeViewMatrix()
{
	return XMMatrixLookAtLH(XMLoadFloat3(&Position), XMLoadFloat3(&Target), XMLoadFloat3(&Up));
}

XMMATRIX DXCamera3D::ComputeProjectionViewMatrix(float aspectRatio)
{
	return XMMatrixMultiply(ComputeProjectionMatrix(aspectRatio), ComputeViewMatrix());
}

XMMATRIX DXCamera3D::ComputeMVPMatrix(float aspectRatio, XMMATRIX world)
{
	return world * ComputeViewMatrix() * ComputeProjectionMatrix(aspectRatio);
}