#pragma once

#include <string>
#include <unordered_map>
#include <vector>

template<typename T>
const T FindCommonElement(const std::vector<T>& a, const std::vector<T>& b, T valueIfNone)
{
	for (const T& ta : a)
	{
		for (const T& tb : b)
		{
			if (ta == tb)
			{
				return ta;
			}
		}
	}
	return valueIfNone;
}

template<typename KeyType, typename ValueType>
ValueType* MapFind(std::unordered_map<KeyType,ValueType> map, KeyType key)
{
	auto itr = map.find(key);
	if (itr != map.end())
	{
		return &(itr->second);
	}
	else
	{
		return nullptr;
	}
}

void LowercaseInPlace(std::string& s);
std::string LowercaseCopy(const std::string& s);
