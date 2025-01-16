#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Octo
{
    class Entity
    {
        public:
            Entity() = default;
            Entity(std::string name = "");

            virtual void update(double deltaTime);

            void addChild(Entity* entity)
            {
                m_Children.push_back(entity);
                entity->setParent(this);
            }

            bool hasChildren() 
            {
                if (m_Children.size() > 0)
                    return true;
                return false;
            }

            template<typename T>
            bool isA()
            {
                return (dynamic_cast<T*>(this) != NULL);
            }

            template<typename T> T as()
            {
                return dynamic_cast<T>(this);
            }

            void setParent(Entity* entity)
            {
                m_Parent = entity;
            }

            Entity* getParent() { return m_Parent; }

            std::string& getName() { return m_Name; }
            std::vector<Entity*>& getChildren() { return m_Children;}
        protected:
            Entity* m_Parent = nullptr;
            std::string m_Name;
            std::vector<Entity*> m_Children;
    };
};