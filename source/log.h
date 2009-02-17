#ifndef _H_LOG
#define _H_LOG

class Log
{
private:
    ofstream os;

    boost::mutex mutex_;

public:
    Log(const char* filename);

//    void log(NodePtr node, char* format, ...);
};

typedef boost::shared_ptr<Log> LogPtr;

#endif
