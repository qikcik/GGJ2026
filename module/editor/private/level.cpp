#include "level.hpp"

#include <fstream>

#include "graphic3d/meshInstance.hpp"
#include "reflection/serializeXML.hpp"
#include "thirdParty/rapidXml/rapidxml.hpp"
#include <thirdParty/rapidXml/rapidxml_print.hpp>
#include <thirdParty/rapidXml/rapidxml_utils.hpp>

#include "gameContext.hpp"

void Level::saveToFile(std::string name,GameContext& ctx)
{
    std::cout << name << std::endl;
    rapidxml::xml_document<> root {};
    rapidxml::xml_node<> *node = root.allocate_node(rapidxml::node_element, "root", nullptr );
    root.append_node(node);


    SerializeXMLWriter serializer{&root,node};

    serializer.propertyStruct("instances",[&](ISerialize* inSerialize) {
        for (auto& it : actors) {
            inSerialize->propertyStruct("instance",[&](ISerialize* inSerialize) {
                std::string temp = it->getClassName();
                inSerialize->propertyString("TYPE",temp);
                it->onSerialize(inSerialize,ctx);
            });
        }
    });
    onSerialize(&serializer);


    std::ofstream out("levels/"+name+".xml");
    out << root;
    root.clear();
}

void Level::loadFromFile(std::string name,GameContext& ctx)
{
    actors.clear();

    rapidxml::file<> in(("levels/"+name+".xml").c_str());
    rapidxml::xml_document<> root;
    root.parse<0>(in.data());
    SerializeXMLReader reader{&root,&root};

    reader.propertyStruct("root",[&](ISerialize* inSerialize) {
        inSerialize->propertyStruct("instances",[&](ISerialize* inSerialize)
        {
            inSerialize->propertyListStruct("instance",[&](ISerialize* inSerialize)
            {
                std::string typeName;
                inSerialize->propertyString("TYPE",typeName);

                auto entry = ctx.world.actorFactory.getByClassName(typeName);
                if(!entry) return;

                auto qmodel = std::make_shared<QModelInstance>();
                auto actor = entry->construct();
                actor->onSerialize(inSerialize,ctx);
                actor->onPlaced(ctx);
                actors.push_back(std::move(actor));
            });
        });
        onSerialize(inSerialize);
    });

    root.clear();
}
