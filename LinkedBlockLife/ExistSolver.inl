#include "ExistSolver.h"




template <typename ListType, typename WD, typename... WD_ARGS>
void ExistSolver::removeTarget(ListType& list, IndexBoard& indexBoard,
	WD&& removeFromWorld, WD_ARGS&&... args)
{
	const auto coreCount = indexBoard.size();

	const auto itemCount = list.size();

	m_currentCore = ((coreCount >= 2) ? 1 : 0);
	m_currentItr = indexBoard[m_currentCore].cbegin();
	m_currentEnd = indexBoard[m_currentCore].cend();

	/*
	* �Ʒ� �ڵ�� m_target###[1], m_target###[2], ..., m_target###[3], m_target###[0]
	* �������� �� ���Ҹ� ���ʷ� ��ȸ�Ͽ����� ���� ������������ �Ǿ���������
	* ���������� �����Ѵ�.
	* ������ Solver���� check###�� ȣ���ϴ� ������ �������� ������.
	*/

	updateIterator(indexBoard);

	auto pivot = list.begin() + *m_currentItr;

	for (std::size_t index = *m_currentItr; index < itemCount; ++index)
	{
		// ������ ��ǥ�� ���� �����ְ�
		// ������ ��ǥ�� ã������
		if ((m_currentCore != 0 || m_currentItr != m_currentEnd)
			&& *m_currentItr == index)
		{
			// ���� ���忡�� ����.
			(this->*std::forward<WD>(removeFromWorld))(*list[index], std::forward<WD_ARGS>(args)...);

			// ���� ��Ͽ� �������� �ʰ� ���� ��ǥ�� �̵�.
			++m_currentItr;
		}
		else
		{
			// ������ ��ǥ�� �ƴϸ�

			// �״�� ���� ��Ͽ� ����.
			*pivot = std::move(list[index]);

			++pivot;
		}

		updateIterator(indexBoard);
	}


	// �Ϻ��� ����.
	list.erase(pivot, list.end());


	// ��ǥ �ʱ�ȭ.
	for (auto& ls : indexBoard)
	{
		ls.clear();
	}
}

