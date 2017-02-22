#include "ThreadPool.h"

#include <cassert>




template <typename FUNC, typename... ARGS>
auto ThreadPool::reserve(FUNC&& func, ARGS&&... args)
->std::future<std::result_of_t<FUNC(ARGS...)>>
{
	using ReturnType = std::result_of_t<FUNC(ARGS...)>;


	assert(m_running);


	auto job = std::make_shared<std::packaged_task<ReturnType()>>(
		std::bind(std::forward<FUNC>(func), std::forward<ARGS>(args)...)
		);

	auto fut = job->get_future();

	{
		std::unique_lock<std::mutex> lock{ m_jobMutex };

		m_job.emplace([job] { (*job)(); });
	}

	m_cond.notify_one();


	return fut;
}

