#pragma once

#include "Entity.h"

namespace Hazel
{
    class ScriptableEntity
    {
    public:
        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(TimeStep ts) {}
    private:
        Entity m_Entity;
        friend class Scene;
    };
}