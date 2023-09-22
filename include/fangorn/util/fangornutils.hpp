#pragma once

#include "fangorn/fangornbase.hpp"
#include "fangorn/model/fangornbtnode.hpp"
#include "fangorn/util/fangornlogging.hpp"

//
// ASSERTIONS
//
#define SUB(...) __VA_ARGS__

#define FANGORN_GENERIC_ASSERT(condition, ...) \
    do \
    { \
        if(!(condition)) \
        { \
            FANGORN_FATAL(__VA_ARGS__); \
            FANGORN_FATAL("^ ^ ^ Failed: " #condition); \
            throw std::runtime_error(#condition); \
        } \
    } while(0)

#define FANGORN_ASSERT_BASE(type, base) \
    FANGORN_GENERIC_ASSERT(SUB(std::is_base_of<base, type>::value), "Expected " #type " (which is %s) to be a child-class of " #base, typeid(type).name())

//
// Types
//

class FangornXmlError {
    public:
    FangornXmlError(int errLine, const std::string& errMsg)
     : errLine(errLine), errMsg(errMsg) { }

    const int errLine;
    const std::string errMsg;
};

//
// Helpful functions
//

int findInArgs(int argc, char **argv, const char *arg);
std::string nodePathToString(std::vector<FangornBtNode::ConstSharedPtr> path);
QString fileToString(const QString& path);
std::string fileToString(const std::string& path);
QDomDocument fileToDocument(const QString& path);
QDomDocument fileToDocument(const std::string& path);


//
// SERIALIZATION / DESERIALIZATION
//
template<typename T>
inline std::string serialize(T value)
{
    FANGORN_FATAL("Cannot serialize type %s because there is no specialization for it! Please add one in util.hpp", typeid(T).name());
    throw std::runtime_error("no template specialization");
}


template<typename T>
inline T deserialize(const std::string& value)
{
    FANGORN_FATAL("Cannot deserialize type %s because there is no specialization for it! Please add one in util.hpp", typeid(T).name());
    throw std::runtime_error("no template specialization");
}

template<typename T>
inline std::string serializableTypeName()
{
    FANGORN_FATAL("Cannot determine demangled name of serializable type %s because there is no specialization for it! Please add one in util.hpp", typeid(T).name());
    throw std::runtime_error("no template specialization");
}

#define SPECIALIZE_SERIALIZE_FOR_TYPE(type, func) \
    template<> \
    inline std::string serialize<type>(type value) \
    { \
        return func(value); \
    }

#define SPECIALIZE_DESERIALIZE_FOR_TYPE(type, func) \
    template<> \
    inline type deserialize<type>(const std::string& value) \
    { \
        return func(value); \
    }

#define SPECIALIZE_TYPENAME_FOR_TYPE(type) \
    template<> \
    inline std::string serializableTypeName<type>() \
    { \
        return #type; \
    }

#define DEF_SERIALIZABLE_TYPE(type, serialize_func, deserialize_func) \
    SPECIALIZE_SERIALIZE_FOR_TYPE(type, serialize_func) \
    SPECIALIZE_DESERIALIZE_FOR_TYPE(type, deserialize_func) \
    SPECIALIZE_TYPENAME_FOR_TYPE(type)


template<typename T>
std::string serializeVector(std::vector<T> v)
{
    std::string s = "[";
    if(v.size() > 0)
    {
        s += serialize<T>(v[0]);
        for(int i = 1; i < v.size(); i++)
        {
            s += ",";
            s += serialize<T>(v[i]);
        }
    }

    s += "]";
    return s;
}

template<typename T>
std::vector<T> deserializeVector(const std::string& s)
{
    std::string s2 = s;
    std::remove_if(s2.begin(), s2.end(), iswspace);
    FANGORN_GENERIC_ASSERT(*s2.begin() == '[', "Vector string being deserialized must begin with a \"[\" (it actually begins with %c)", *s2.begin());
    FANGORN_GENERIC_ASSERT(*(s2.end() - 1) == ']', "Vector string being deserialized must end with a \"]\" (it actually ends with %c)", *(s2.end() - 1));
    
    s2 = s.substr(1, s.size() - 2); //clip off []
    std::vector<T> v;
    if(s2.length() > 0)
    {
        size_t i = 0;
        size_t next = s2.find(',', i);
        while(next != std::string::npos)
        {
            v.push_back(deserialize<T>(s2.substr(i, next - i)));
            i = next + 1;
            next = s2.find(',', i);
        }
        
        //deserialize and store the rest of the data
        v.push_back(deserialize<T>(s2.substr(i, std::string::npos)));
    }

    return v;
}

DEF_SERIALIZABLE_TYPE(int, std::to_string, std::stoi);
DEF_SERIALIZABLE_TYPE(long, std::to_string, std::stol);
DEF_SERIALIZABLE_TYPE(double, std::to_string, std::stod);
DEF_SERIALIZABLE_TYPE(std::string,,);
DEF_SERIALIZABLE_TYPE(std::vector<int>, serializeVector<int>, deserializeVector<int>);
DEF_SERIALIZABLE_TYPE(std::vector<double>, serializeVector<double>, deserializeVector<double>);
DEF_SERIALIZABLE_TYPE(std::vector<long>, serializeVector<long>, deserializeVector<long>);
DEF_SERIALIZABLE_TYPE(std::vector<std::string>, serializeVector<std::string>, deserializeVector<std::string>);

