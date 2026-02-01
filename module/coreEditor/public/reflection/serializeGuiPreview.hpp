#pragma once

#include "imgui.h"
#include "reflection/serialize.hpp"

class SerializeGuiPreview : public ISerialize
{
public:
    void propertyStruct(std::string inName, std::function<void(ISerialize* inSerialize)> inInner) override
    {
        if (ImGui::TreeNode(inName.c_str()))
        {
            inInner(this);
            ImGui::TreePop();
        }
    }

    void propertyString(std::string inName, std::string& inValue) override
    {
        char buffer[1024];
        strncpy(buffer,inValue.c_str(),1024);
        ImGui::InputText(inName.c_str(),buffer,1024);
        inValue = buffer;
    }

    void propertyBool(std::string inName, bool& inValue) override
    {
        ImGui::Checkbox(inName.c_str(),&inValue);
    }

    void propertyFloat(std::string inName, float& inValue) override
    {
        float temp = inValue;
        ImGui::InputFloat(inName.c_str(),&temp);
        if(temp != inValue)
            inValue = temp;
    }

    void propertyInt(std::string inName, int& inValue) override
    {
        int temp = inValue;
        ImGui::InputInt(inName.c_str(),&temp);
        if(temp != inValue)
            inValue = temp;
    }

    void propertyEnum(std::string inName, std::vector<std::string> options, std::string& selected) override
    {
        // std::vector<const char*> items;
        // int i = 0;
        // int sel = 0;
        // for (auto& it : options)
        // {
        //     if (selected == it) sel = i;
        //     items.push_back(it.c_str());
        //     i++;
        // }
        //
        // ImGui::Combo(inName.c_str(), &sel, items.data(), items.size());
        // selected = options[sel];

        static char filter[64] = "";
        static int selected_idx = -1;
        std::vector<const char*> items;
        for (auto& it : options)
        {
            items.push_back(it.c_str());
        }

        if (ImGui::BeginCombo("Searchable Combo", selected_idx == -1 ? "Select..." : items[selected_idx])) {
            // Pole tekstowe do filtrowania
            ImGui::InputText("##Filter", filter, IM_ARRAYSIZE(filter));
            ImGui::Separator();

            for (int i = 0; i < items.size(); i++) {
                // Logika filtrowania (case-insensitive wymagałoby dodatkowej funkcji)
                if (strlen(filter) > 0 && strstr(items[i], filter) == nullptr) {
                    continue ;
                }

                const bool is_selected = (selected_idx == i);
                if (ImGui::Selectable(items[i], is_selected)) {
                    selected_idx = i;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    void propertyColor(std::string inName, float color[4]) override
    {
        ImGui::ColorPicker4(inName.c_str(), color);
    }
};