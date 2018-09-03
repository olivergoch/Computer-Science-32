#include <iostream>
#include <string>
using namespace std;

/////////
//FILE//
/////////
class File
{
public:
    File(string nm);
    string name() const;
    virtual void open() const = 0;
    virtual void redisplay() const;
    virtual ~File();
private:
    string nam;
};

File::File(string nm)
{
    nam = nm;
}

string File::name() const
{
    return nam;
}

void File::redisplay() const
{
    cout << "refresh the screen";
}

File::~File()
{}

////////////
//TEXTMSG//
//////////
class TextMsg : public File
{
public:
    TextMsg(string nm);
    virtual void open() const;
    ~TextMsg();
};

TextMsg::TextMsg(string nm):File(nm)
{}

void TextMsg::open() const
{
    cout << "open text message";
}

TextMsg::~TextMsg()
{
    cout << "Destroying " << this->name() << ", a text message" << endl;
}

//////////
//VIDEO//
////////

class Video : public File
{
public:
    Video(string nm, int rt);
    virtual void open() const;
    virtual void redisplay() const;
    ~Video();
private:
    int runningtime;
};

Video::Video(string nm, int rt):File(nm)
{
    runningtime = rt;
}

void Video::open() const
{
    cout << "play " << runningtime << " second video";
}

void Video::redisplay() const
{
    cout << "replay video";
}

Video::~Video()
{
    cout << "Destroying " << this->name() << ", a video" << endl;
}

////////////
//PICTURE//
//////////
class Picture : public File
{
public:
    Picture(string nm);
    virtual void open() const;
    ~Picture();
};

Picture::Picture(string nm):File(nm)
{}

void Picture::open() const
{
    cout << "show picture";
}

Picture::~Picture()
{
    cout << "Destroying the picture " << this->name() << endl;
}
////////////
//O&REDIS//
//////////
void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}