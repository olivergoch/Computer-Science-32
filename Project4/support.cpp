#include "support.h"

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs)
{
    if(lhs.latitudeText < rhs.latitudeText)
        return true;
    if(lhs.latitudeText > rhs.latitudeText)
        return false;
    
    return lhs.longitudeText < rhs.longitudeText;
}

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs)
{
    if(lhs.latitudeText > rhs.latitudeText)
        return true;
    if(lhs.latitudeText < rhs.latitudeText)
        return false;
    
    return lhs.longitudeText > rhs.longitudeText;
}

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs)
{
    if(lhs.latitudeText != rhs.latitudeText)
        return false;
    
    return lhs.longitudeText == rhs.longitudeText;
}

bool operator!=(const GeoCoord& lhs, const GeoCoord& rhs)
{
    if(lhs.latitudeText == rhs.latitudeText)
        return false;
    
    return lhs.longitudeText != rhs.longitudeText;
}

std::string angleToString(double angle)
{
    if(angle >= 0 && angle <= 22.5)
        return "east";
    if(angle > 22.5 && angle <= 67.5)
        return "northeast";
    if(angle > 67.5 && angle <= 112.5)
        return "north";
    if(angle > 112.5 && angle <= 157.5)
        return "northwest";
    if(angle > 157.5 && angle <= 202.5)
        return "west";
    if(angle > 202.5 && angle <= 247.5)
        return "southwest";
    if(angle > 247.5 && angle <= 292.5)
        return "west";
    if(angle > 292.5 && angle <= 337.5)
        return "southeast";
    if(angle > 337.5 && angle < 360)
        return "east";
    
    return "error";
}