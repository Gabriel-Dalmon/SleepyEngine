#pragma once

#include <map>
#include <string>

class Input
{
	public:
		Input();
		bool Init();

		void Update();

		bool IsPressed(std::string key, bool allowHold);
		bool IsHeld(std::string key);
		bool IsReleased(std::string key);

		void OnKeyPressed(std::string key);
		void OnKeyReleased(std::string key);

	private:
		std::map<int, int> InputList;
};