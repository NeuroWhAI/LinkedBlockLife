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
	* 아래 코드는 m_target###[1], m_target###[2], ..., m_target###[3], m_target###[0]
	* 순서에서 각 원소를 차례로 순회하였을때 값이 오름차순으로 되어있을때만
	* 정상적으로 동작한다.
	* 때문에 Solver에서 check###을 호출하는 로직에 의존성을 가진다.
	*/

	updateIterator(indexBoard);

	auto pivot = list.begin() + *m_currentItr;

	for (std::size_t index = *m_currentItr; index < itemCount; ++index)
	{
		// 삭제할 목표가 아직 남아있고
		// 삭제할 목표를 찾았으면
		if ((m_currentCore != 0 || m_currentItr != m_currentEnd)
			&& *m_currentItr == index)
		{
			// 먼저 월드에서 제거.
			(this->*std::forward<WD>(removeFromWorld))(*list[index], std::forward<WD_ARGS>(args)...);

			// 기존 목록에 저장하지 않고 다음 목표로 이동.
			++m_currentItr;
		}
		else
		{
			// 삭제할 목표가 아니면

			// 그대로 기존 목록에 저장.
			*pivot = std::move(list[index]);

			++pivot;
		}

		updateIterator(indexBoard);
	}


	// 완벽히 제거.
	list.erase(pivot, list.end());


	// 목표 초기화.
	for (auto& ls : indexBoard)
	{
		ls.clear();
	}
}

