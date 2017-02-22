#ifndef __LINKER_PORT_H__
#define __LINKER_PORT_H__


#include <vector>




class Linker;
class Block;


class LinkerPort
{
public:
	explicit LinkerPort(const Block& owner);


private:
	const Block& m_owner;


private:
	std::vector<Linker*> m_linkers;
	std::vector<Block*> m_targets;


public:
	bool contains(Linker const* linker) const;
	bool contains(Block const* target) const;
	void connect(Linker* linker);
	void connect(Linker* linker, Block* target);
	void disconnectAll();
	void disconnect(Linker const* linker);
	void disconnect(Block const* target);


public:
	const std::vector<Linker*>& getLinkerList() const;
	const std::vector<Block*>& getTargetList() const;
};


#endif