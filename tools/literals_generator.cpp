#include "literals_generator.h"

#include <fstream>

// Excecute from project root:
// cmake --preset tests-debug
// cmake --build build/tests-debug --target generate_physics_literals

int main()
{
    std::ofstream output{ "physics/literals.generated.h" };
    physics::literals_generator::generate(output);
}
