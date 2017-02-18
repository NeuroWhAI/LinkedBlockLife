#include "LinkerPort.h"

#include "Linker.h"




LinkerPort::LinkerPort(const Block& owner)
	: m_owner(owner)
{

}

//#################################################################################################

bool LinkerPort::checkDuplicate(Linker* linker) const
{
	if (&linker->getFirst() == &m_owner)
	{
		return checkDuplicate(&linker->getSecond());
	}


	return checkDuplicate(&linker->getFirst());
}


bool LinkerPort::checkDuplicate(Block* target) const
{
	for (auto* block : m_targets)
	{
		if (block == target)
		{
			return true;
		}
	}


	return false;
}


void LinkerPort::connect(Linker* linker)
{
	connect(linker, (&linker->getFirst() == &m_owner) ? &linker->getSecond() : &linker->getFirst());
}


void LinkerPort::connect(Linker* linker, Block* target)
{
	m_linkers.emplace_back(linker);
	m_targets.emplace_back(target);
}


void LinkerPort::disconnectAll()
{
	m_linkers.clear();
	m_targets.clear();
}

//#################################################################################################

std::vector<Linker*>& LinkerPort::getLinkerList()
{
	return m_linkers;
}


std::vector<Block*>& LinkerPort::getTargetList()
{
	return m_targets;
}

