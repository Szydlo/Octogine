#include "Input.h"

using Octo::Input;

float Input::getDirection(const int key, const int secondKey)
{
	if (getKeyDown(key) && getKeyDown(secondKey)) return 0.0f;
	if (getKeyDown(key)) return 1.0f;
	if (getKeyDown(secondKey)) return -1.0f;

	return 0.0f;
}