#pragma once

#include <string>
#include <vector>

namespace Octo
{
    class Entity
    {
        public:
            Entity() = default;
            explicit Entity(const std::string &name = "");
            virtual ~Entity() = default;

            virtual void onStart();
            virtual void onUpdate(double deltaTime);

            void addChild(Entity* entity)
            {
                m_Children.push_back(entity);
                entity->setParent(this);
            }

            [[nodiscard]] bool hasChildren() const
            {
                if (!m_Children.empty())
                    return true;
                return false;
            }

            template<typename T>
            bool isA()
            {
                return (dynamic_cast<T*>(this) != nullptr);
            }

            template<typename T> T as()
            {
                return dynamic_cast<T>(this);
            }

            void setParent(Entity* entity)
            {
                m_Parent = entity;
            }

            [[nodiscard]] Entity* getParent() const { return m_Parent; }

            std::string& getName() { return m_Name; }
            std::vector<Entity*>& getChildren() { return m_Children;}
        protected:
            Entity* m_Parent = nullptr;
            std::string m_Name;
            std::vector<Entity*> m_Children;
    };
};