#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader ml;
    AttractionMapper am;
    SegmentMapper sm;
    bool checkIfAtEnd(const GeoCoord& start, const GeoCoord& end, NavSegment& last) const;
    string getStreetName(const GeoCoord& start, const GeoCoord& end) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    //if the map can't be loaded return false
    if(!ml.load(mapFile))
        return false;
    //otherwise load the mapdata into attraction mapper and segment mapper and return true
    am.init(ml);
    sm.init(ml);
	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    for(int i = 0; i < start.size(); i++)
        start[i] = tolower(start[i]);
    
    for(int i = 0; i < end.size(); i++)
        end[i] = tolower(end[i]);
    
    directions.clear();
    GeoCoord startingPoint;
    GeoCoord endingPoint;
    //if the starting attraction can't be found, return
    if(!am.getGeoCoord(start, startingPoint))
        return NAV_BAD_SOURCE;
    
    //if the ending attraction can't be found, return
    if(!am.getGeoCoord(end, endingPoint))
        return NAV_BAD_DESTINATION;
    
    NavSegment onlySeg;
    if(checkIfAtEnd(startingPoint, endingPoint, onlySeg))
    {
        directions.push_back(onlySeg);
        return NAV_SUCCESS;
    }
    list <GeoCoord> openList;
    list <GeoCoord> closedList;
    vector <GeoCoord> pastPlaces;
    openList.push_front(startingPoint);
    while(!openList.empty())
    {
        //reset value
        GeoCoord current = openList.front();
        NavSegment overNow;
        //check if at end
        if(checkIfAtEnd(current, endingPoint, overNow))
        {
            for(int i = 0; i < pastPlaces.size()-1; i++)
            {
                GeoSegment thisGeoSeg(pastPlaces[i], pastPlaces[i+1]);
                NavSegment thisNavSeg(angleToString(angleOfLine(thisGeoSeg)), getStreetName(pastPlaces[i], pastPlaces[i+1]), distanceEarthMiles(pastPlaces[i], pastPlaces[i+1]), thisGeoSeg);
                directions.push_back(thisNavSeg);
            }
            directions.push_back(overNow);
            for(int j = 0; j < directions.size()-1; j++)
            {
                if(directions[j].m_streetName != directions[j+1].m_streetName)
                {
                    if(angleBetween2Lines(directions[j].m_geoSegment, directions[j+1].m_geoSegment) < 180)
                        directions.insert(directions.begin()+j+1, NavSegment("left", directions[j+1].m_streetName));
                    else
                        directions.insert(directions.begin()+j+1, NavSegment("right", directions[j+1].m_streetName));
                    j++;
                }
            }
            return NAV_SUCCESS;
        }
        openList.pop_front();
        closedList.push_front(current);
        pastPlaces.push_back(current);
        
        vector <StreetSegment> getTheSegs = sm.getSegments(current);
        for(auto iter = getTheSegs.begin(); iter != getTheSegs.end(); iter++)
        {
            if(openList.size() == 0 && *find(closedList.begin(), closedList.end(), current) != current)
                openList.push_front(iter->segment.start);
            else if(openList.size() == 0 && *find(closedList.begin(), closedList.end(), current) != current)
                openList.push_front(iter->segment.end);
            
            if(distanceEarthMiles(iter->segment.start, endingPoint) < distanceEarthMiles(openList.front(), endingPoint)
               && *find(closedList.begin(), closedList.end(), iter->segment.start) != iter->segment.start)
                openList.push_front(iter->segment.start);
            else if(*find(closedList.begin(), closedList.end(), iter->segment.start) != iter->segment.start)
                openList.push_back(iter->segment.start);
            if(distanceEarthMiles(iter->segment.end, endingPoint) < distanceEarthMiles(openList.front(), endingPoint)
               && *find(closedList.begin(), closedList.end(), iter->segment.end) != iter->segment.end)
                openList.push_front(iter->segment.end);
            else if(*find(closedList.begin(), closedList.end(), iter->segment.end) != iter->segment.end)
                openList.push_back(iter->segment.end);
            /*GeoCoord output = openList.front();
            cout << output.latitudeText << endl;
            cout << output.longitudeText << endl;
            cout << iter->streetName << endl;*/
        }
        
    }
    
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

bool NavigatorImpl::checkIfAtEnd(const GeoCoord &start, const GeoCoord &end, NavSegment& last) const
{
    vector<StreetSegment> s1 = sm.getSegments(start);
    vector<StreetSegment> s2 = sm.getSegments(end);
    auto iter = s1.begin();
    for(;iter != s1.end(); iter++)
    {
        for(auto rator = s2.begin(); rator != s2.end(); rator++)
        {
            if(iter->segment.start.longitude == rator->segment.start.longitude
               && iter->segment.start.latitude == rator->segment.start.latitude
               && iter->segment.end.longitude == rator->segment.end.longitude
               && iter->segment.end.latitude == rator->segment.end.latitude)
            {
                GeoSegment oneSeg(start, end);
                NavSegment copy(angleToString(angleOfLine(oneSeg)), iter->streetName, distanceEarthMiles(start, end), oneSeg);
                last = copy;
                return true;
            }
        }
    }
    return false;
}

string NavigatorImpl::getStreetName(const GeoCoord &start, const GeoCoord &end) const
{
    vector<StreetSegment> s1 = sm.getSegments(start);
    vector<StreetSegment> s2 = sm.getSegments(end);
    auto iter = s1.begin();
    for(;iter != s1.end(); iter++)
    {
        for(auto rator = s2.begin(); rator != s2.end(); rator++)
        {
            if(iter->segment.start.longitude == rator->segment.start.longitude
               && iter->segment.start.latitude == rator->segment.start.latitude
               && iter->segment.end.longitude == rator->segment.end.longitude
               && iter->segment.end.latitude == rator->segment.end.latitude)
            {
                return iter->streetName;
            }
        }
    }
    return "nothing";
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
