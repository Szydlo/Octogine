#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Octo
{
    class Entity
    {
        public:
            Entity(std::string name = "", std::shared_ptr<Entity> parent = nullptr);

            virtual void update(double deltaTime);
            void setParent(std::shared_ptr<Entity> parent);
            
            void addChild(Entity child) 
            { 
                m_Children.push_back(child); 
                m_Children[m_Children.size()-1].setParent(std::make_shared<Entity>(this)); 
            }

            Entity& createChild(Entity child) 
            { 
                m_Children.emplace_back(child); 
                m_Children[m_Children.size()-1].setParent(std::make_shared<Entity>(this)); 

                return m_Children[m_Children.size()-1]; 
            }

            Entity& getChildByName(std::string name)
            {
                for (auto& child : m_Children)
                {
                    if (child.getName() == name)
                    {
                        return child;
                    }
                }
            }


            bool hasChildren() 
            {
                if (m_Children.size() > 0)
                    return true;
                return false;
            }

            std::string& getName() { return m_Name; }
            //std::vector<Entity>& getChildren() { return m_Children;}*/
        protected:
            std::shared_ptr<Entity> m_Parent;
            std::string m_Name;
            std::vector<Entity> m_Children;
    };
};