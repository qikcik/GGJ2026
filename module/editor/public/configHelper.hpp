#pragma once


#include "thirdParty/rapidXml/rapidxml.hpp"
#include <thirdParty/rapidXml/rapidxml_print.hpp>
#include <thirdParty/rapidXml/rapidxml_utils.hpp>

#include "reflection/serializeXML.hpp"


inline void ConfigSave(std::string name, Config& state)
{
    rapidxml::xml_document<> root {};
    rapidxml::xml_node<> *node = root.allocate_node(rapidxml::node_element, "config", nullptr );
    root.append_node(node);

    SerializeXMLWriter serializer{&root,node};
    state.onSerialize(&serializer);


    std::ofstream out(name+".xml");
    out << root;
    root.clear();
}

inline void ConfigLoad(std::string name, Config& state)
{
    try {
        rapidxml::file<> in((name+".xml").c_str());
        rapidxml::xml_document<> root;
        root.parse<0>(in.data());
        SerializeXMLReader reader{&root,&root};

        reader.propertyStruct("config",[&](ISerialize* ctx)
        {
            state.onSerialize(ctx);
        });

        root.clear();
    } catch (std::exception& e) {
        return;
    }
}