#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

class WrappedXML
{
private:
    boost::property_tree::ptree xml;
public:
    WrappedXML(const char* _filename)
    {
        boost::property_tree::xml_parser::read_xml(_filename, xml);
    }
    operator boost::property_tree::ptree() const { return xml; } //Conversion from wrapper type to wrapped type. Allows objects of WrappedResource to be used in functions which required the resource which have been wrapped.
};
