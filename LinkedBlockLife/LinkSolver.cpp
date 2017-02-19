#include "LinkSolver.h"

#include "Block.h"
#include "Linker.h"




using std::size_t;

//#################################################################################################

LinkSolver::LinkSolver()
{

}

//#################################################################################################

void LinkSolver::updateLinker(Linker& linker)
{
	linker.calculateDir();
}


void LinkSolver::updateBlock(Block& block)
{
	auto& linkerPort = block.getLinkerPort();

	auto& linkers = linkerPort.getLinkerList();

	for (auto* linker : linkers)
	{
		auto force = linker->calculateElasticity();

		if (&linker->getSecond() == &block)
		{
			force *= -1.0f;
		}

		block.addForce(force);
	}
}

