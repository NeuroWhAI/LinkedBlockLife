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
	bool checkDuplicate(Linker* linker) const;
	bool checkDuplicate(Block* target) const;
	void connect(Linker* linker);
	void connect(Linker* linker, Block* target);
	void disconnectAll();


public:
	std::vector<Linker*>& getLinkerList();
	std::vector<Block*>& getTargetList();
};


#endif