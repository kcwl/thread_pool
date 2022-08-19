#pragma once
#include <future>
#include "basic_task.hpp"

namespace thread_pool
{
	namespace detail
	{
		template<typename _Task>
		struct use_task
		{
			template<typename _Schedule, typename _Func, typename... _Args>
			auto operator()(_Schedule& schedule, _Func&& func, _Args&&... args) -> std::future<std::invoke_result_t<_Func, _Args...>>
			{
				using return_type = std::invoke_result_t<_Func, _Args...>;

				auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<_Func>(func), std::forward<_Args>(args)...));

				auto future = task_ptr->get_future();

				_Task task([tk = std::move(task_ptr)]{ (*tk)(); });

				schedule.push(std::move(task));

				return future;
			}
		};

		template<typename _Func1>
		struct use_task<basic_priority_task<_Func1>>
		{
			template<typename _Schedule,typename _Func, typename... _Args>
			auto operator()(_Schedule& schedule, _Func&& func, _Args&&... args, std::size_t&& priority) -> std::future<std::invoke_result_t<_Func, _Args...>>
			{
				using return_type = std::invoke_result_t<_Func, _Args...>;

				auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<_Func>(func), std::forward<_Args>(args)...));

				auto future = task_ptr->get_future();

				basic_priority_task<_Func1> task([tk = std::move(task_ptr)]{ (*tk)(); }, std::forward<std::size_t>(priority));

				schedule.push(std::move(task));

				return future;
			}
		};

		template<typename _Tuple, std::size_t... I>
		constexpr auto expand(_Tuple&& t, std::index_sequence<I...>)
		{
			return (std::get<I>(t), ...);
		}

		template<typename _Tuple, typename _Indices = std::make_index_sequence<std::tuple_size_v<_Tuple>>>
		constexpr auto expand(_Tuple&& t)
		{
			return expand(std::forward<_Tuple>(t), _Indices{});
		}

		template<typename _Task, typename _Func, typename... _Args>
		struct task_result
		{
			using type = std::invoke_result_t<_Func, _Args...>;
		};

		template<typename _TaskFunc, typename _Func, typename... _Args>
		struct task_result<basic_priority_task<_TaskFunc>, _Func, _Args...>
		{
			template<typename... _Args1>
			constexpr static auto task_type(_Func&&, _Args1&&..., std::size_t) ->std::invoke_result_t<_Func, _Args1...>
			{
				return {};
			}

			using type = decltype(task_type(std::declval<_Func>(), expand(std::tuple<_Args...>())));
		};

		template<typename _Task, typename _Func, typename... _Args>
		using task_result_t = task_result<_Task, _Func, _Args...>::type;
	}

}