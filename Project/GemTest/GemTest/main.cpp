#include <stdlib.h>
#include <utility>
#include <memory>
#include "../../Common/Engine.h"

using namespace Gem;

int main(int argc, char ** argv)
{
	Gem::Memory::MemoryManager::Initialize();
	Gem::Reflection::Runtime::Compile(nullptr);

	Gem::Vector2 vec1;
	vec1.Set(72.0f, 16.0f);
	Gem::Vector2 vec2 = vec1.Normalized();

	Gem::Debugging::Debug::LogFormat("Math", "Vec1 = %s", vec1.ToString().c_str());
	Gem::Debugging::Debug::LogFormat("Math", "Vec2 = %s", vec2.ToString().c_str());

	Gem::Reflection::Runtime::Terminate();
	Gem::Memory::MemoryManager::Terminate();
	system("pause");

	return EXIT_SUCCESS;
}
