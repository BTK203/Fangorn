#include "fangorn/util/fangornutils.hpp"
#include "fangorn/util/fangornlogging.hpp"
#include <string.h>
#include <QFile>


int findInArgs(int argc, char **argv, const char *arg)
{
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], arg) == 0) {
            return i;
        }
    }

    return -1;
}


std::string nodePathToString(std::vector<FangornBtNode::ConstSharedPtr> path)
{
    std::string str = "";

    if(path.size() > 0) {
        str = path.at(0)->getName();
        for(int i = 1; i < path.size(); i++) {
            str += "/" + path.at(i)->getName();
        }
    }

    return str;
}


QString fileToString(const QString& path)
{
    QFile f(path);
    if(f.exists())
    {
        f.open(QIODevice::ReadOnly);
        return f.readAll();
    }

    FANGORN_WARN("File %s requested but doesnt exist!");
    return "";
}
