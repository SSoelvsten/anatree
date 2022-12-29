// Import Unit Testing framework, Bandit
#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

// Import Anatree
#include <anatree.h>

int main(int argc, char* argv[])
{
#ifdef NDEBUG
  std::cerr << "Warning: Internal assertions are not enabled!"
            << std::endl << std::endl;
#endif
  return bandit::run(argc, argv);
}
