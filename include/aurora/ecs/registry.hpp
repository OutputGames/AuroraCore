#if !defined(REGISTRY_HPP)
#define REGISTRY_HPP

#include "aurora/utils/utils.hpp"
#include "component.hpp"

struct AURORA_API ComponentRegistry
{
    template<typename T> static Ref<Component> createInstance() { return Ref<Component>(std::make_shared<T>()); }

    //typedef std::map<std::string, Component* (*)()> map_type;
    static std::map<std::string, Ref<Component>(*)()> cmp_map;


    template <typename... C>
    struct ComponentRegister
    {

    };

    template <typename... Comp>
    static void RegisterComponent();

    template <typename... Comp>
    static void RegisterComponent(ComponentRegistry::ComponentRegister<Comp...>);

};

template <typename ... Comp>
void ComponentRegistry::RegisterComponent()
{
    ([]()
        {
	        std::string rawname = typeid(Comp).name();

	        std::string sub = "class ";

            std::string::size_type i = rawname.find(sub);

            if (i != std::string::npos)
                rawname.erase(i, sub.length());


            ComponentRegistry::cmp_map[rawname] = &createInstance<Comp>;
            
        }(), ...);
}

template <typename ... Comp>
void ComponentRegistry::RegisterComponent(ComponentRegistry::ComponentRegister<Comp...>)
{
    RegisterComponent<Comp ...>();
}

#endif // REGISTRY_HPP
