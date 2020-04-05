/*
    example use
    enum class Shapes
    {
    Circle,
    Box
    };
    typedef Singleton<ClassFactory<IShape,Shapes, float> > shapeFactory;
    //use anonomous namespace so this part isnt accesable. and use const bool trick to register your creater
    namespace
    {
    const bool registered = shapeFactor::Instance().RegisterCreater(Shapes::Circle, [](float arg){return new
   Circle(arg);});
    }

    //then in other code to use
    IShape* shape = chapeFactory.Create(Shapes::Circle, 1.0f);
    */
#pragma once

#include "core/Log.h"

#include "core/Concepts.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace CR::Core {
	template<SemiRegular Created, Regular Key, typename... ArgTypes>
	class ClassFactory {
		using Creator = std::function<Created(ArgTypes...)>;

	  public:
		ClassFactory()                    = default;
		ClassFactory(const ClassFactory&) = delete;
		ClassFactory(ClassFactory&&)      = delete;
		~ClassFactory()                   = default;
		ClassFactory& operator=(const ClassFactory&) = delete;
		ClassFactory& operator=(ClassFactory&&) = delete;

		bool RegisterCreator(Key a_key, Creator a_creator) {
			m_creators.insert(std::make_pair(a_key, a_creator));
			return true;
		}
		template<SemiRegular... ArgTypesF>
		Created Create(Key a_key, ArgTypesF... a_args) {
			auto iterator = m_creators.find(a_key);
			Log::Error(iterator != m_creators.end(), "no registered creater for requested key {}", a_key);

			return (iterator->second)(std::forward<ArgTypesF>(a_args)...);
		}

	  private:
		std::unordered_map<Key, Creator> m_creators;
	};
}    // namespace CR::Core
