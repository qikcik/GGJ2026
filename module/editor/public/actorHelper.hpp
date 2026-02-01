#pragma once
#include <memory>

#include "actor.hpp"
#include "gameContext.hpp"
#include "reflection/serializeXML.hpp"
#include "thirdParty/rapidXml/rapidxml.hpp"

inline std::unique_ptr<ActorBase> duplicateActor(ActorBase* actor,GameContext& context)
{
    rapidxml::xml_document<> root {};
    rapidxml::xml_node<> *node = root.allocate_node(rapidxml::node_element, "state", nullptr );
    root.append_node(node);

    SerializeXMLWriter serializer{&root,node};
    actor->onSerialize(&serializer,context);

    auto fac = context.world.actorFactory.getByClassName(actor->getClassName());
    if (!fac) return {};

    std::unique_ptr<ActorBase> out = fac->construct();

    SerializeXMLReader reader{&root,node};
    out->onSerialize(&reader,context);
    root.clear();
    out->onPlaced(context);
    return out;

}
