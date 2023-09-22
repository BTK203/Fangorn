#pragma once

#include "fangorn/fangorncommon.hpp"

class FangornConfig {
    public:
    FangornConfig(const std::string& file);
    
    template<typename T>
    void putValue(const std::string& name, const T& value)
    {
        QDomElement newElement = config.createElement("value");
        QDomText newElemText = config.createTextNode(QString::fromStdString(serialize<T>(value)));
        newElement.appendChild(newElemText);

        std::map<std::string, std::string> namesAndTypes = getNamesAndTypes();
        if(namesAndTypes.find(name) != namesAndTypes.end())
        {
            //already have a value with that name. make sure types match
            FANGORN_GENERIC_ASSERT(namesAndTypes[name] == serializableTypeName<T>(), "Already have a value with name %s that has type %s. Cannot put a value with conflicting type %s", name, namesAndTypes[name], serializableTypeName<T>());
            
        } else
        {
            config.firstChildElement("fangornconfig").appendChild(newElement);
        }
    }

    template<typename T>
    T getValue(const std::string& name, const T& defaultValue)
    {
        QDomElement child = config.firstChildElement("fangornconfig").firstChildElement("value");
        while(!child.isNull())
        {
            if(child.hasAttribute("name") && child.hasAttribute("type"))
            {
                if(child.attribute("name").toStdString() == name)
                {
                    std::string 
                        requestedType = serializableTypeName<T>(),
                        actualType = child.attribute("type").toStdString();

                    if(actualType == requestedType)
                    {
                        return deserialize<T>(child.text());
                    }

                    FANGORN_ERROR("Conflicting types for config value %s: stored value has type %s but requested type is %s", name, actualType, requestedType);
                }
            }

            child = child.nextSiblingElement("value");
        }

        //value doesnt exist. Create it and return it
        FANGORN_DEBUG("Config value %s with type %s does not exist. Creating it.", name, serializableTypeName<T>());
        putValue<T>(name, defaultValue);
        return defaultValue;
    }

    std::map<std::string, std::string> getNamesAndTypes();
    bool hasName(const std::string& name);

    private:
    QDomElement findValueElementByName(const std::string& name);
    QDomDocument config;
};
