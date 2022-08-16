#pragma once
#include <memory>
#include <thread>

namespace thread_pool
{
	namespace detail
	{
		template<typename Pool>
		class work_thread : public std::enable_shared_from_this<work_thread<Pool>>
		{
		public:
			work_thread(std::shared_ptr<Pool> pool_ptr)
				: pool_ptr_(pool_ptr)
			{
				thread_ptr_.reset(new std::thread([&] { run(); }));
			}

		public:
			void run()
			{
				while (pool_ptr_->execute());
			}

			void join()
			{
				return thread_ptr_->join();
			}

		private:
			std::shared_ptr<Pool> pool_ptr_;

			std::shared_ptr<std::thread> thread_ptr_;
		};
	}
}