#pragma once
class SCVector2i
{
public:
	int X, Y;

    SCVector2i Multiply(const SCVector2i& other) const;
    SCVector2i Divide(const SCVector2i& other) const;
    SCVector2i Add(const SCVector2i& other) const;
    SCVector2i Subtract(const SCVector2i& other) const;
    int Dot(const SCVector2i& other) const;
    int Cross(const SCVector2i& other) const;
    SCVector2i Normalize() const;
    SCVector2i ScalarMultiply(float scalar) const;
    SCVector2i Clamp(int min, int max) const;

	SCVector2i(int x = 0, int y = 0) : X(x), Y(y) {}
};

class SCVector2f
{
public:
    float X, Y;

    SCVector2f Multiply(const SCVector2f& other) const;
    SCVector2f Divide(const SCVector2f& other) const;
    SCVector2f Add(const SCVector2f& other) const;
    SCVector2f Subtract(const SCVector2f& other) const;
    float Dot(const SCVector2f& other) const;
    float Cross(const SCVector2f& other) const;
    SCVector2f Normalize() const;
    SCVector2f ScalarMultiply(float scalar) const;
    SCVector2f Clamp(float min, float max) const;

    SCVector2f(float x = 0, float y = 0) : X(x), Y(y) {}
};

class SCVector3f
{
public:
    float X, Y, Z;

    SCVector3f Multiply(const SCVector3f& other) const;
    SCVector3f Divide(const SCVector3f& other) const;
    SCVector3f Add(const SCVector3f& other) const;
    SCVector3f Subtract(const SCVector3f& other) const;
    float Dot(const SCVector3f& other) const;
    SCVector3f Cross(const SCVector3f& other) const;
    SCVector3f Normalize() const;
    SCVector3f ScalarMultiply(float scalar) const;
    SCVector3f Clamp(float min, float max) const;

    SCVector3f(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
};

class SCVector4f
{
public:
    float X, Y, Z, W;
    SCVector4f(float x = 0, float y = 0, float z = 0, float w = 0) : X(x), Y(y), Z(z), W(w) {}
    // to-do: add in extra functions, prob.
};