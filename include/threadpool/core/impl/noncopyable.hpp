#pragma once

namespace threadpool
{
	namespace impl
	{
		class noncopyable
		{
		protected:
			noncopyable() = default;
			~noncopyable() = default;

		private:
			noncopyable(const noncopyable&) = delete;
			noncopyable& operator=(const noncopyable&) = delete;
		};
	} // namespace impl
} // namespace threadpool