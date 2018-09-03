#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> attractionsMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++)
    {
        StreetSegment getSeg;
        ml.getSegment(i, getSeg);
        for(int j = 0; j < getSeg.attractions.size(); j++)
        {
            for(int k = 0; k < getSeg.attractions[j].name.size(); k++)
                getSeg.attractions[j].name[k] = towlower(getSeg.attractions[j].name[k]);
            attractionsMap.associate(getSeg.attractions[j].name, getSeg.attractions[j].geocoordinates);
        }
        
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    if(attractionsMap.find(attraction) == nullptr)
        return false;
    
    gc = *attractionsMap.find(attraction);
	return true;
}


//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
