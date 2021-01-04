#include "twPosition.h"

twPosition::twPosition()
{
    X = 0;
    Y = 0;
}

twPosition::twPosition(int x, int y)
{
    X = x;
    Y = y;
}

twPosition twPosition::operator+(const twPosition &point)
{
	return twPosition(X + point.X, Y + point.Y);
}

twPosition twPosition::operator-(const twPosition &point)
{
	return twPosition(X - point.X, Y - point.Y);
}

twPosition twPosition::operator/(const twPosition &point)
{
	return twPosition(X / point.X, Y / point.Y);
}

//FLOAT

twPositionf::twPositionf()
{
    X = 0;
    Y = 0;
}

twPositionf::twPositionf(float x, float y)
{
    X = x;
    Y = y;
}

twPositionf twPositionf::operator+(const twPositionf &point)
{
	return twPositionf(X + point.X, Y + point.Y);
}

twPositionf twPositionf::operator-(const twPositionf &point)
{
	return twPositionf(X - point.X, Y - point.Y);
}

twPositionf twPositionf::operator/(const twPositionf &point)
{
	return twPositionf(X / point.X, Y / point.Y);
}
