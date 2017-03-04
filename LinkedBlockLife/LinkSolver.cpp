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
	linker.calculateElasticity();
}


void LinkSolver::updateBlock(Block& block)
{
	auto& linkerPort = block.getLinkerPort();
	auto& linkers = linkerPort.getLinkerList();

	for (auto* linker : linkers)
	{
		auto& force = linker->getElasticity();

		if (&linker->getSecond() == &block)
		{
			block.addForce(-force);
		}
		else
		{
			block.addForce(force);
		}
	}
}

