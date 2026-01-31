#pragma once
#include <functional>
#include <iostream>
#include <string>

#include "serialize.hpp"
#include "thirdParty/rapidXml/rapidxml.hpp"

class SerializeXMLWriter : public ISerialize
{
public:
    rapidxml::xml_document<>* root;
    rapidxml::xml_node<>* topNode;

    SerializeXMLWriter(rapidxml::xml_document<>* inRoot, rapidxml::xml_node<>* inTopNode)
     : root(inRoot), topNode(inTopNode) {}

    void propertyStruct(std::string inName, std::function<void(ISerialize* inSerialize)> inInner) override
    {
        const auto* name = root->allocate_string(inName.c_str(),inName.size());

        rapidxml::xml_node<>* node = root->allocate_node(rapidxml::node_element, name,nullptr,inName.size(),0);
        topNode->append_node(node);

        SerializeXMLWriter ctx {root,node};
        inInner(&ctx);
    }
    void propertyString(std::string inName, std::string& inValue) override
    {
        std::string valueStr = inValue;
        auto* name = root->allocate_string(inName.c_str(),inName.size());
        auto* value = root->allocate_string(valueStr.c_str(),valueStr.size());

        rapidxml::xml_node<>* node = root->allocate_node(rapidxml::node_element, name, value,inName.size(),valueStr.size());
        topNode->append_node(node);
    }

    void propertyFloat(std::string inName, float& inValue) override
    {
        std::string valueStr = std::to_string(inValue);
        auto* name = root->allocate_string(inName.c_str(),inName.size());
        auto* value = root->allocate_string(valueStr.c_str(),valueStr.size());

        rapidxml::xml_node<>* node = root->allocate_node(rapidxml::node_element, name, value,inName.size(),valueStr.size());
        topNode->append_node(node);
    }
    void propertyInt(std::string inName, int& inValue) override
    {
        std::string valueStr = std::to_string(inValue);
        auto* name = root->allocate_string(inName.c_str(),inName.size());
        auto* value = root->allocate_string(valueStr.c_str(),valueStr.size());

        rapidxml::xml_node<>* node = root->allocate_node(rapidxml::node_element, name, value,inName.size(),valueStr.size());
        topNode->append_node(node);
    }

    void propertyBool(std::string inName, bool& inValue) override
    {
        std::string valueStr = inValue ? "true" : "false";
        propertyString(inName,valueStr);
    }

    void propertyEnum(std::string inName, std::vector<std::string> options, std::string& selected) override
    {
        std::string valueStr = selected;
        auto* name = root->allocate_string(inName.c_str(),inName.size());
        auto* value = root->allocate_string(valueStr.c_str(),valueStr.size());

        rapidxml::xml_node<>* node = root->allocate_node(rapidxml::node_element, name, value,inName.size(),valueStr.size());
        topNode->append_node(node);
    }
};


class SerializeXMLReader : public ISerialize
{
public:
    rapidxml::xml_document<>* root;
    rapidxml::xml_node<>* topNode;

    SerializeXMLReader(rapidxml::xml_document<>* inRoot, rapidxml::xml_node<>* inTopNode)
     : root(inRoot), topNode(inTopNode) {}

    void propertyStruct(std::string inName, std::function<void(ISerialize* inSerialize)> inInner) override
    {
        rapidxml::xml_node<>* node = topNode->first_node(inName.c_str());
        if(!node) { std::cout << "[WRN] Couldn't read "<<inName<<"\n";  return;};
        SerializeXMLReader ctx = {root,node};
        inInner(&ctx);
    }

    void propertyListStruct(std::string inName, std::function<void(ISerialize* inSerialize)> inInner) override
    {
        for(rapidxml::xml_node<>* node = topNode->first_node(inName.c_str()); node != nullptr; node = node->next_sibling(inName.c_str()))
        {
            SerializeXMLReader ctx = {root,node};
            inInner(&ctx);
        }
    }

    void propertyString(std::string inName, std::string& inValue) override
    {
        rapidxml::xml_node<>* node = topNode->first_node(inName.c_str());
        if(!node) {std::cout << "[WRN] Couldn't read "<<inName<<"\n"; return;};

        inValue = "";
        for (int idx = 0; idx !=  node->value_size(); idx++)
        {
            inValue+= node->value()[idx];
        }
    }

    void propertyBool(std::string inName, bool& inValue) override
    {
        std::string valueStr = inValue ? "true" : "false";
        propertyString(inName,valueStr);
        inValue = valueStr == "true" ? true : false;
    }

    void propertyFloat(std::string inName, float& inValue) override
    {
        rapidxml::xml_node<>* node = topNode->first_node(inName.c_str());
        if(node) inValue = std::stof(node->value());
        else std::cout << "[WRN] Couldn't read "<<inName<<"\n";
    }
    void propertyInt(std::string inName, int& inValue) override
    {
        rapidxml::xml_node<>* node = topNode->first_node(inName.c_str());
        if(node) inValue = std::stoi(node->value());
        else std::cout << "[WRN] Couldn't read "<<inName<<"\n";
    }
    void propertyEnum(std::string inName, std::vector<std::string> options, std::string& selected) override
    {
        propertyString(inName,selected);
    }
};
