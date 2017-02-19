#ifndef __LINK_SOLVER_H__
#define __LINK_SOLVER_H__


#include <memory>
#include <vector>




class Block;
class Linker;


class LinkSolver
{
public:
	LinkSolver();


public:
	void updateLinker(Linker& linker);
	void updateBlock(Block& block);
};


#endif