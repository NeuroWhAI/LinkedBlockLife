#include "LinkerPort.h"

#include "Linker.h"




LinkerPort::LinkerPort(const Block& owner)
	: m_owner(owner)
{

}

//#################################################################################################

bool LinkerPort::contains(Linker const* linker) const
{
	if (&linker->getFirst() == &m_owner)
	{
		return contains(&linker->getSecond());
	}


	return contains(&linker->getFirst());
}


bool LinkerPort::contains(Block const* target) const
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


void LinkerPort::disconnect(Linker const* linker)
{
	const auto size = m_linkers.size();

	for (std::size_t index = 0; index < size; ++index)
	{
		if (m_linkers[index] == linker)
		{
			m_linkers.erase(m_linkers.begin() + index);
			m_targets.erase(m_targets.begin() + index);
			break;
		}
	}
}


void LinkerPort::disconnect(Block const* target)
{
	const auto size = m_targets.size();

	for (std::size_t index = 0; index < size; ++index)
	{
		if (m_targets[index] == target)
		{
			m_targets.erase(m_targets.begin() + index);
			m_linkers.erase(m_linkers.begin() + index);
			break;
		}
	}
}

//#################################################################################################

const std::vector<Linker*>& LinkerPort::getLinkerList() const
{
	return m_linkers;
}


const std::vector<Block*>& LinkerPort::getTargetList() const
{
	return m_targets;
}

