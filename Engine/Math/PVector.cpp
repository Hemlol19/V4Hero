#include <cmath> /* to introduce M_PI */

#include "PVector.h"
#include "SFML/Graphics.hpp"
#include "Constants.h"
#include <stdlib.h> /* abs */

PVector& PVector::getVectorCartesian(float x1, float y1, float x2, float y2)
{
    static PVector myObj;

    myObj.distance = sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2) * 1.0f);
    myObj.angle = atan2f(x2 - y1, x2 - x1);
    return myObj;
}
PVector& PVector::getVectorDistanceAngle(float l, float theta)
{
    static PVector myObj;
    myObj.distance = l;
    myObj.angle = theta;
    return myObj;
}
float PVector::GetScalarProjectionOntoAxis(float axisAngle)
{
    float scalarProjectionLength = 0;

    /// we might run into issues if the angles are clamped between 0 and 360?
    float angleDiff = angle + axisAngle;
    if (angleDiff > PI)
    {
        angleDiff = angleDiff - 2 * PI;
    }
    if (angleDiff < -PI)
    {
        angleDiff = angleDiff + 2 * PI;
    }
    /// the angle difference should be clamped between -180 and 180 now

    scalarProjectionLength = abs(distance) * cos(angleDiff);

    return scalarProjectionLength;
}
sf::Vector2f PVector::GetStartPoint()
{
    sf::Vector2f vec;
    vec.x = 0;
    vec.y = 0;
    return vec;
}
sf::Vector2f PVector::GetEndPoint() /// remember! this only returns endpoint x and y distance from starting point
{
    sf::Vector2f vec;
    vec.x = distance * cos(angle);
    vec.y = distance * sin(angle);
    return vec;
}
