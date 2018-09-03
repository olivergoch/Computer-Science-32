#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment>> segmentsMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++)
    {
        StreetSegment gotSeg;
        ml.getSegment(i, gotSeg);
        if(segmentsMap.find(gotSeg.segment.start) == nullptr)
        {
            vector<StreetSegment> streetSegs;
            streetSegs.push_back(gotSeg);
            segmentsMap.associate(gotSeg.segment.start, streetSegs);
        }
        else
        {
            segmentsMap.find(gotSeg.segment.start)->push_back(gotSeg);
        }
        if(segmentsMap.find(gotSeg.segment.end) == nullptr)
        {
            vector<StreetSegment> streetSegs;
            streetSegs.push_back(gotSeg);
            segmentsMap.associate(gotSeg.segment.end, streetSegs);
        }
        else
        {
            segmentsMap.find(gotSeg.segment.end)->push_back(gotSeg);
        }
        if(gotSeg.attractions.size() > 0)
        {
            for(auto iter = gotSeg.attractions.begin(); iter != gotSeg.attractions.end(); iter++)
            {
                if(segmentsMap.find(iter->geocoordinates) == nullptr)
                {
                    vector<StreetSegment> streetSegs;
                    streetSegs.push_back(gotSeg);
                    segmentsMap.associate(iter->geocoordinates, streetSegs);
                }
                else
                {
                    segmentsMap.find(iter->geocoordinates)->push_back(gotSeg);
                }
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
    if(segmentsMap.find(gc) == nullptr)
        return segments;
    segments = *segmentsMap.find(gc);
	return segments;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
