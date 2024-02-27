#include "Input.h"
#include <vector>
#include <iostream>
#include <Windows.h>

template<typename Key, typename Value>
std::ostream& operator << (std::ostream& os, std::map<Key, Value>& map)
{
	os << "{\n";
	for (const auto& pair:map)
	{
		os << "\t{" << pair.first << " => " << pair.second << "}\n";
	}
	os << "}";

	return os;
}

Input::Input()
{}

bool Input::Init()
{
	std::vector<int> GodDamnLongInputIdList = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'M', 'N', 'L', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		// Left, right and middle buttons. Have a gamer mouse with 14 buttons? Too bad!
		VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
		VK_RETURN, VK_SPACE, VK_DELETE,
		VK_CONTROL, VK_SHIFT, VK_MENU, VK_TAB
	};

	for (int i = 0; i < GodDamnLongInputIdList.size(); i++)
	{
		InputList.insert({ GodDamnLongInputIdList[i], 0 });
	}
	std::cout << InputList << std::endl;
	return true;
}

void Input::Update()
{
	for (auto& pair:InputList)
	{
		if (GetAsyncKeyState(pair.first) & 0x01)
		{
			if (pair.first>0x2E)
				std::cout << "Key " << pair.first << " pressed: " << static_cast<char>(pair.first) << ". Current value: " << pair.second << std::endl;
			else
				std::cout << "Special key " << pair.first << " pressed! Current value:" << pair.second << std::endl;

			if (pair.second < 2)
				pair.second++;
		}
		else if (pair.second > 0) {
			if (pair.first > 0x2E)
				std::cout << "Key " << pair.first << " released: " << static_cast<char>(pair.first) << std::endl;
			else
				std::cout << "Special key " << pair.first << " released" << std::endl;

			pair.second = 0;
		}
	}
	std::cout << "-------------------" << std::endl;
	std::cout << InputList << std::endl;
}