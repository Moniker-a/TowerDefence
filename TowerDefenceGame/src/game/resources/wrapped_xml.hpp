#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

//Allows correct construction of ptree from XML files when used in conjunction with ResourceManager.
class WrappedXML
{
private:
    boost::property_tree::ptree xml;
public:
    WrappedXML(const char* _filename)
    {
        boost::property_tree::xml_parser::read_xml(_filename, xml);
    }

    template <typename T>
    void set_attribute(const char* _elementPath, T _value)
    {
        xml.put(_elementPath, _value);
    }

    template <typename T>
    void set_attribute(std::string _elementPath, T _value)
    {
        set_attribute(_elementPath.c_str(), _value);
    }
    operator boost::property_tree::ptree() const { return xml; } //Conversion from wrapper type to wrapped type. Allows objects of WrappedResource to be used in functions which required the resource which have been wrapped.
};
