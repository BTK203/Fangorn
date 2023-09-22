#include "fangorn/util/fangornconfig.hpp"

FangornConfig::FangornConfig(const std::string& file)
 : config(fileToDocument(file))
{
    FANGORN_GENERIC_ASSERT(!config.firstChildElement("fangornconfig").isNull(), "Malformed config XML! Root tag should be \"fangornconfig\"");
}


std::map<std::string, std::string> FangornConfig::getNamesAndTypes()
{
    std::map<std::string, std::string> vals;
    QDomElement child = config.firstChildElement("fangornconfig").firstChildElement("value");
    while(!child.isNull())
    {
        if(child.hasAttribute("name") && child.hasAttribute("type"))
        {
            vals.insert({ child.attribute("name").toStdString(), child.attribute("type").toStdString() });
        }
    }

    return vals;
}


bool FangornConfig::hasName(const std::string& name)
{
    std::map<std::string, std::string> namesAndTypes = getNamesAndTypes();
    return namesAndTypes.find(name) != namesAndTypes.end();
}


QDomElement FangornConfig::findValueElementByName(const std::string& name)
{
    QDomElement child = config.firstChildElement("fangornconfig").firstChildElement("value");
    while(!child.isNull())
    {
        if(child.hasAttribute("name"))
        {
            return child;
        }

        child = child.nextSiblingElement("value");
    }

    return QDomElement(); // null qdomelement
}
