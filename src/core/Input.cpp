#include "Input.h"

using Octo::Input;

float Input::getDirection(int key, int secondKey)
{
	if (getKeyDown(key) && getKeyDown(secondKey)) return 0.0f;
	else if (getKeyDown(key)) return 1.0f;
	else if (getKeyDown(secondKey)) return -1.0f;

	return 0.0f;
}