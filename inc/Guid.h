#pragma once
#include<string>
#include<functional>
#include <cassert>

namespace CR
{
	namespace Core
	{
		class Guid
		{
		public:
			Guid(unsigned int _data1, unsigned int _data2,
				unsigned int _data3, unsigned int _data4)
			{
				data[0] = _data1;
				data[1] = _data2;
				data[2] = _data3;
				data[3] = _data4;
			}
			Guid(const Guid &_other)
			{
				(*this) = _other;
			}
			template<typename T>
			Guid(const T& a_data)
			{
				Set(a_data);
			}
			Guid& operator=(const Guid &_other)
			{
				data[0] = _other.data[0];
				data[1] = _other.data[1];
				data[2] = _other.data[2];
				data[3] = _other.data[3];
				return *this;
			}
			Guid()
			{
				data[0] = 0;
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
			}
			bool operator==(const Guid &_other) const
			{
				if(data[0] == _other.data[0] && data[1] == _other.data[1] &&
					data[2] == _other.data[2] && data[3] == _other.data[3])
					return true;
				else
					return false;
			}
			static Guid Null()
			{
				static Guid null = Guid(0,0,0,0);
				return null;
			}
			bool IsNull() const
			{
				if(data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0)
					return true;
				else 
					return false;
			}
			//!works with strings, wstrings, char arrays, wchar_t arrays
			template<typename T>
			void Set(const T& a_data)
			{
				data[0] = data[1] = data[2] = data[3] = 0;
				int count = 0;
				auto iterator = std::begin(a_data);
				while (iterator != std::end(a_data))
				{
					int index = (count&0x0ff8)>>3;
					if((*iterator) >= 48 && (*iterator) <= 57)
					{
						data[index] = (data[index] << 4)+((*iterator)-48);
						count++;
					}
					if((*iterator) >= 65 && (*iterator) <= 71)
					{
						data[index] = (data[index] << 4)+((*iterator)-55);
						count++;
					}
					if((*iterator) >= 97 && (*iterator) <= 102)
					{
						data[index] = (data[index] << 4)+((*iterator)-87);
						count++;
					}
					iterator++;
				}
				if (count != 32)
					throw std::invalid_argument("guids must have 32 chars that are 0-9, a-f, or A-F, other chars are allowed but ignored");
			}
			unsigned int Data1() const {return data[0];}
			unsigned int Data2() const { return data[1]; }
			unsigned int Data3() const { return data[2]; }
			unsigned int Data4() const { return data[3]; }
			unsigned int operator[](int a_index) const
			{
				assert(a_index >= 0 && a_index < 4);
				return data[a_index];
			}
		bool operator<(const Guid &_other) const
		{
			for(int i = 0;i < 4;++i)
			{
				if(data[i]<_other[i])
					return true;
				if(data[i]>_other[i])
					return false;
			}
			return false;
		}

		operator std::string() const
		{
			static const char translation[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
			return Convert<std::string::value_type, std::string::traits_type>(translation, '{', '-', '}');
		}

		operator std::wstring() const
		{
			static const wchar_t translation[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };
			return Convert<std::wstring::value_type, std::wstring::traits_type>(translation, L'{', L'-', L'}');
		}

		private:
			template<typename CharT, typename Traits>
			std::basic_string<CharT, Traits> Convert(const CharT* a_translation, CharT a_firstChar, CharT a_seperatorChar, CharT a_lastChar) const
			{
				std::basic_string<CharT, Traits> result;
				auto conversion = [&](unsigned short a_data){
					result += a_translation[(a_data >> 12) & 0xf];
					result += a_translation[(a_data >> 8) & 0xf];
					result += a_translation[(a_data >> 4) & 0xf];
					result += a_translation[a_data & 0xf];
				};

				auto cdata = reinterpret_cast<const unsigned short*>(data);
				int index = 0;
				//little endian
				result += a_firstChar;
				conversion(cdata[1]);
				conversion(cdata[0]);
				result += a_seperatorChar;
				conversion(cdata[3]);
				result += a_seperatorChar;
				conversion(cdata[2]);
				result += a_seperatorChar;
				conversion(cdata[5]);
				result += a_seperatorChar;
				conversion(cdata[4]);
				conversion(cdata[7]);
				conversion(cdata[6]);
				result += a_lastChar;
				return result;
			}

			unsigned int data[4];
		};
	}
}

template<typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& a_strm, const CR::Core::Guid& a_guid)
{
	a_strm << static_cast<std::basic_string<CharT, Traits>>(a_guid);
	return a_strm;
}

namespace std {
	template <>
	class hash<CR::Core::Guid>{
	public:
		size_t operator()(const CR::Core::Guid& a_guid) const
		{
			return hash<unsigned int>()(a_guid.Data1()) ^ hash<unsigned int>()(a_guid.Data2()) ^
				hash<unsigned int>()(a_guid.Data3()) ^ hash<unsigned int>()(a_guid.Data4());
		}
	};
};