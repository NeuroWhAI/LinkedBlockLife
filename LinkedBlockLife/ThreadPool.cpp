#include "ThreadPool.h"




ThreadPool::ThreadPool()
	: ThreadPool(std::thread::hardware_concurrency())
{

}


ThreadPool::ThreadPool(std::size_t count)
	: m_running(true)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		m_pool.emplace_back(&ThreadPool::threadJob, this);
	}
}


ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock{ m_jobMutex };

		m_running = false;
	}

	m_cond.notify_all();

	for (auto& task : m_pool)
		task.join();
}

//#################################################################################################

void ThreadPool::threadJob()
{
	while (m_running)
	{
		std::function<void()> job;

		{
			std::unique_lock<std::mutex> lock{ m_jobMutex };

			m_cond.wait(lock, [this] {
				return !(m_running && m_job.empty());
			});

			if (m_running == false && m_job.empty())
				break;

			job = std::move(m_job.front());
			m_job.pop();
		}

		job();
	}
}

