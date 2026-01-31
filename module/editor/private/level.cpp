#include "level.hpp"

#include <fstream>

#include "graphic3d/meshInstance.hpp"
#include "reflection/serializeXML.hpp"
#include "thirdParty/rapidXml/rapidxml.hpp"
#include <thirdParty/rapidXml/rapidxml_print.hpp>
#include <thirdParty/rapidXml/rapidxml_utils.hpp>

void Level::saveToFile(std::string name)
{
    std::cout << name << std::endl;
    rapidxml::xml_document<> root {};
    rapidxml::xml_node<> *node = root.allocate_node(rapidxml::node_element, "root", nullptr );
    root.append_node(node);


    SerializeXMLWriter serializer{&root,node};

    serializer.propertyStruct("instances",[&](ISerialize* inSerialize) {
        for (auto& it : instances) {
            inSerialize->propertyStruct("instance",[&](ISerialize* inSerialize) {
                it->onSerialize(inSerialize,{});
            });
        }
    });


    std::ofstream out("levels/"+name+".xml");
    out << root;
    root.clear();
}

void Level::loadFromFile(std::string name)
{
    instances.clear();

    rapidxml::file<> in(("levels/"+name+".xml").c_str());
    rapidxml::xml_document<> root;
    root.parse<0>(in.data());
    SerializeXMLReader reader{&root,&root};

    reader.propertyStruct("root",[&](ISerialize* inSerialize) {
        inSerialize->propertyStruct("instances",[&](ISerialize* inSerialize)
        {
            inSerialize->propertyListStruct("instance",[&](ISerialize* inSerialize)
            {
                auto qmodel = std::make_shared<QModelInstance>();
                qmodel->onSerialize(inSerialize,{});
                instances.push_back(qmodel);
            });
        });
    });

    root.clear();
}
