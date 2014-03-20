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
#include "Singleton.h"

namespace CR
{
	namespace Core
	{
		template<typename Created, typename Key, typename... Args>
		class ClassFactory 
		{
			friend class Singleton<ClassFactory>;
			typedef std::function<Created(Args...)> Creater;
		public:
			bool RegisterCreater(Key _key,Creater _creater)
			{
				m_creaters.insert(std::make_pair(_key,_creater));
				return true;
			}
			Created Create(Key _key, Args... params)
			{
				auto iterator = m_creaters.find(_key);
				if(iterator == m_creaters.end())
					throw std::exception("no registered creater for requested key");
				else
					return (iterator->second)(params...);
			}

		private:
			ClassFactory() {}
			ClassFactory(ClassFactory &_other);
			virtual ~ClassFactory() {}
			std::unordered_map<Key,Creater> m_creaters;
		};
	}
}