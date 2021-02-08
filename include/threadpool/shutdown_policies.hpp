#pragma once
#include <memory>


namespace thread_pool
{
	template<class Pool>
	struct wait_for_all_task
	{
		static void shutdown(std::shared_ptr<Pool> pool)
		{
			pool->wait();
			pool->shutdown();
		}
	};

	template<class Pool>
	struct wait_for_active_task
	{
		static void shutdown(std::shared_ptr<Pool> pool)
		{
			pool->clear();
			pool->wait();
			pool->shutdown();
		}
	};
}
