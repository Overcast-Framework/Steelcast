#include <iostream>
#include <algorithm>
#include <Math/Vector.h>

SCVector2i SCVector2i::Multiply(const SCVector2i& other) const
{
	return { this->X * other.X, this->Y * other.Y };
}

SCVector2i SCVector2i::Divide(const SCVector2i& other) const
{
	if (other.X != 0 && other.Y != 0)
		return { this->X / other.X, this->Y / other.Y };
	std::cout << "[ENGINE][MATH]: Vector2i division by zero is unavailable." << std::endl;
	return *this;
}

SCVector2i SCVector2i::Subtract(const SCVector2i& other) const
{
	return { this->X - other.X, this->Y - other.Y };
}

SCVector2i SCVector2i::Add(const SCVector2i& other) const
{
	return { this->X + other.X, this->Y + other.Y };
}

SCVector2i SCVector2i::Clamp(int min, int max) const
{
	return { std::clamp(X, min, max), std::clamp(Y, min, max) };
}

int SCVector2i::Dot(const SCVector2i& other) const
{
	return X * other.X + Y * other.Y;
}

int SCVector2i::Cross(const SCVector2i& other) const
{
	return X * other.Y - Y * other.X;
}

SCVector2i SCVector2i::Normalize() const 
{
	float length = std::sqrt(X * X + Y * Y);
	if (length != 0)
		return { static_cast<int>(X / length), static_cast<int>(Y / length) };
	std::cout << "[ENGINE][MATH]: Vector2i normalization of length 0 is unavailable." << std::endl;
	return *this;
}

SCVector2i SCVector2i::ScalarMultiply(float scalar) const {
	return { static_cast<int>(X * scalar), static_cast<int>(Y * scalar) };
}

// SCVector2f


SCVector2f SCVector2f::Multiply(const SCVector2f& other) const
{
	return { this->X * other.X, this->Y * other.Y };
}

SCVector2f SCVector2f::Divide(const SCVector2f& other) const
{
	if (other.X != 0 && other.Y != 0)
		return { this->X / other.X, this->Y / other.Y };
	std::cout << "[ENGINE][MATH]: Vector2f division by zero is unavailable." << std::endl;
	return *this;
}

SCVector2f SCVector2f::Subtract(const SCVector2f& other) const
{
	return { this->X - other.X, this->Y - other.Y };
}

SCVector2f SCVector2f::Add(const SCVector2f& other) const
{
	return { this->X + other.X, this->Y + other.Y };
}

SCVector2f SCVector2f::Clamp(float min, float max) const
{
	return { std::clamp(X, min, max), std::clamp(Y, min, max) };
}

float SCVector2f::Dot(const SCVector2f& other) const
{
	return X * other.X + Y * other.Y;
}

float SCVector2f::Cross(const SCVector2f& other) const
{
	return X * other.Y - Y * other.X;
}

SCVector2f SCVector2f::Normalize() const
{
	float length = std::sqrt(X * X + Y * Y);
	if (length != 0)
		return { (X / length), (Y / length) };
	std::cout << "[ENGINE][MATH]: Vector2f normalization of length 0 is unavailable." << std::endl;
	return *this;
}

SCVector2f SCVector2f::ScalarMultiply(float scalar) const {
	return { (X * scalar), (Y * scalar) };
}

// SCVector3f


SCVector3f SCVector3f::Multiply(const SCVector3f& other) const
{
	return { this->X * other.X, this->Y * other.Y, this->Z * other.Z };
}

SCVector3f SCVector3f::Divide(const SCVector3f& other) const
{
	if (other.X != 0 && other.Y != 0 && other.Z != 0)
		return { this->X / other.X, this->Y / other.Y, this->Z / other.Z };

	std::cerr << "[ENGINE][MATH]: Vector3f division by zero is unavailable." << std::endl;
	return *this;
}

SCVector3f SCVector3f::Subtract(const SCVector3f& other) const
{
	return { this->X - other.X, this->Y - other.Y, this->Z - other.Z };
}

SCVector3f SCVector3f::Add(const SCVector3f& other) const
{
	return { this->X + other.X, this->Y + other.Y, this->Z + other.Z };
}

SCVector3f SCVector3f::Clamp(float min, float max) const
{
	return { std::clamp(X, min, max), std::clamp(Y, min, max), std::clamp(Z, min, max) };
}

float SCVector3f::Dot(const SCVector3f& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z;
}

SCVector3f SCVector3f::Cross(const SCVector3f& other) const
{
	return {
		this->Y * other.Z - this->Z * other.Y,
		this->Z * other.X - this->X * other.Z,
		this->X * other.Y - this->Y * other.X
	};
}

SCVector3f SCVector3f::Normalize() const
{
	float length = std::sqrt(X * X + Y * Y + Z * Z);
	if (length != 0)
		return { (X / length), (Y / length), (Z / length) };

	std::cerr << "[ENGINE][MATH]: Vector3f normalization of length 0 is unavailable." << std::endl;
	return *this;
}

SCVector3f SCVector3f::ScalarMultiply(float scalar) const
{
	return { X * scalar, Y * scalar, Z * scalar };
}