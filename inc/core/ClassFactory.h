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
	const bool registered = shapeFactor::Instance().RegisterCreater(Shapes::Circle, [](float arg){return new Circle(arg);});
	}

	//then in other code to use
	IShape* shape = chapeFactory.Create(Shapes::Circle, 1.0f);
	*/
#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <exception>
#include "core/Singleton.h"
#include "core/Concepts.h"

namespace CR::Core {
	template<SemiRegular Created, Regular Key, typename... ArgTypes>
	class ClassFactory : public Singleton<ClassFactory<Created, Key, ArgTypes...>> {
		friend class Singleton<ClassFactory<Created, Key, ArgTypes...>>;
		using Creator = std::function<Created(ArgTypes...)>;
	public:
		bool RegisterCreator(Key a_key, Creator a_creator) {
			m_creators.insert(std::make_pair(a_key, a_creator));
			return true;
		}
		template<SemiRegular... ArgTypesF>
		Created Create(Key a_key, ArgTypesF... a_args) {
			auto iterator = m_creators.find(a_key);
			if(iterator == m_creators.end())
				throw std::exception("no registered creater for requested key");
			else
				return (iterator->second)(std::forward<ArgTypesF>(a_args)...);
		}

	private:
		ClassFactory() = default;
		ClassFactory(const ClassFactory&) = delete;
		ClassFactory(ClassFactory&&) = delete;
		virtual ~ClassFactory() = default;
		ClassFactory& operator=(const ClassFactory&) = delete;
		ClassFactory& operator=(ClassFactory&&) = delete;
		std::unordered_map<Key, Creator> m_creators;
	};
}