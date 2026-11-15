#include "literals_generator.h"

#include <fstream>

// Excecute from project root:
// cmake --preset windows-tests-debug
// cmake --build build/windows-tests-debug --target generate_physics_literals

int main()
{
    std::ofstream output{ "physics/literals.generated.h" };
    physics::literals_generator::generate(output);
}
