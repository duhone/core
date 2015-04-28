/*
 *  Property.h
 *  BoB
 *
 *  Created by Eric Duhon on 1/27/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "Concepts.h"
#include "TypeTraits.h"

namespace CR
{
	namespace Core
	{
		/** ValueType must be at least SemiRegular, however Property will also work with all
		other types, TotalyOrdered, Arithmetic, ect. And work with the operators those types work
		with*/
		template<SemiRegular ValueType, typename ThisType>
		struct Property
		{
			using GetterType = ValueType& (ThisType::*)();
			using SetterType = void (ThisType::*)(const ValueType&);

			Property(ThisType* a_this, GetterType a_getter, SetterType a_setter)
			{
				m_this = a_this;
				m_getter = a_getter;
				m_setter = a_setter;
			}
			operator ValueType()
			{
				return Get();
			}
			void operator=(const ValueType &a_value)
			{
				Set(a_value);
			}
			void operator=(const Property& a_value)
			{
				Set(a_value.Get());
			}
			friend bool operator==(const Property& a_arg1, const Property& a_arg2)
			{
				return a_arg1.Get() == a_arg2.Get();
			}
			friend bool operator==(const ValueType& a_arg1, const Property& a_arg2)
			{
				return a_arg1 == a_arg2.Get();
			}
			friend bool operator==(const Property& a_arg1, const ValueType& a_arg2)
			{
				return a_arg1.Get() == a_arg2;
			}
			/*
			template<typename T, typename U>
			friend auto operator!=(const T& a_arg1, const U& a_arg2) -> std::enable_if_t<
				is_one_of<T, Property, ValueType>::value && is_one_of<U, Property, ValueType>::value
				,bool>
			{
				return !(a_arg1 == a_arg2);
			}
			*/
			friend bool operator<(const Property& a_arg1, const Property& a_arg2)
			{
				return a_arg1.Get() < a_arg2.Get();
			}
			friend bool operator<(const Property& a_arg1, const ValueType& a_arg2)
			{
				return a_arg1.Get() < a_arg2;
			}
			friend bool operator<(const ValueType& a_arg1, const Property& a_arg2)
			{
				return a_arg1 < a_arg2.Get();
			}
			/*template<typename T, typename U>
			friend bool operator>(const T& a_arg1, const U& a_arg2)
			{
				return (a_arg2 < a_arg1);
			}
			ValueType operator+=(const Property& a_other)
			{
				(*this) = (m_this->*m_getter)() + (a_other.m_this->*a_other.m_getter)()
			}*/
			//Your class must implement ==, <, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
		private:
			ValueType Get() const { return (m_this->*m_getter)(); }
			void Set(const ValueType& a_other) { (m_this->*m_setter)(a_other); }

			ThisType *m_this;
			GetterType m_getter;
			SetterType m_setter;
		};
	}
}

#define CRProperty(ValueType, ThisType, Getter, Setter) CR::Core::Property<ValueType, ThisType> Data{this, &ThisType::Getter, &ThisType::Setter}