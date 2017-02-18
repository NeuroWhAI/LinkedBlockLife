#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__


#include <queue>
#include <vector>
#include <thread>
#include <future>
#include <condition_variable>
#include <type_traits>
#include <functional>
#include <mutex>
#include <memory>




class ThreadPool
{
public:
	ThreadPool();
	explicit ThreadPool(std::size_t count);
	~ThreadPool();


private:
	std::mutex m_jobMutex;
	std::condition_variable m_cond;
	std::queue<std::function<void()>> m_job;
	std::vector<std::thread> m_pool;
	bool m_running;


private:
	void threadJob();


public:
	template <typename FUNC, typename... ARGS>
	auto reserve(FUNC&& func, ARGS&&... args)
		->std::future<std::result_of_t<FUNC(ARGS...)>>;
};


#include "ThreadPool.inl"


#endif