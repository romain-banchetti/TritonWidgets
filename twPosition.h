#ifndef TWPOSITION_H_INCLUDED
#define TWPOSITION_H_INCLUDED

class twPosition
{
    public:
		twPosition();
        twPosition(int x, int y);
        int X;
        int Y;
        twPosition operator+(const twPosition &point);
        twPosition operator-(const twPosition &point);
        twPosition operator/(const twPosition &point);
    private:

};

class twPositionf
{
    public:
		twPositionf();
        twPositionf(float x, float y);
        float X;
        float Y;
        twPositionf operator+(const twPositionf &point);
        twPositionf operator-(const twPositionf &point);
        twPositionf operator/(const twPositionf &point);
    private:

};

#endif // TWPOSITION_H_INCLUDED
