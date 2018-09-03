#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> m_segs;
    int m_numSegs;
};

MapLoaderImpl::MapLoaderImpl()
{
    m_numSegs = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if(!infile)
    {
        return false;
        exit(1);
    }
    string line;
    int lineNum = 0;
    //total number of attractions
    int totalAttract = 0;
    //current number of attractions
    int numOfAttract = 0;
    while (getline(infile, line))
    {
        if(lineNum == 0){
            StreetSegment newSeg;
            m_segs.push_back(newSeg);
            m_segs[m_numSegs].streetName = line;
            lineNum++;
        }
        else if(lineNum == 1)
        {
            GeoCoord start(line.substr(0,10), line.substr(12,12));
            GeoCoord end(line.substr(25, 10), line.substr(36, 12));
            m_segs[m_numSegs].segment.start = start;
            m_segs[m_numSegs].segment.end = end;
            lineNum++;
        }
        else if(lineNum == 2)
        {
            totalAttract = stoi(line);
            if(totalAttract == 0)
            {
                lineNum = 0;
                m_numSegs++;
            }
            else
                lineNum++;
        }
        else if(lineNum > 2)
        {
            m_segs[m_numSegs].attractions.push_back(*new Attraction);
            size_t pos = line.find('|');
            m_segs[m_numSegs].attractions[numOfAttract].name = line.substr(0, pos);
            GeoCoord place(line.substr(pos+1, 10), line.substr(pos+13, 12));
            m_segs[m_numSegs].attractions[numOfAttract].geocoordinates = place;
            numOfAttract++;
            if(numOfAttract == totalAttract)
            {
                lineNum = 0;
                m_numSegs++;
                numOfAttract = 0;
            }
        }
    }
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_numSegs;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum >= m_numSegs)
        return false;
    
    seg = m_segs[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
