#ifndef ___2Project4__support__
#define ___2Project4__support__
#include "provided.h"
#include <string>


bool operator<(const GeoCoord& lhs, const GeoCoord& rhs);
bool operator>(const GeoCoord& lhs, const GeoCoord& rhs);
bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);
bool operator!=(const GeoCoord& lhs, const GeoCoord& rhs);
std::string angleToString(double angle);

#endif /* defined(___2Project4__support__) */
