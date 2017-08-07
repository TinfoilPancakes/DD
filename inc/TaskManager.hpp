/* ************************************************************************** */
/*                                                                            */
/*                                                   __                       */
/*   TaskManager.hpp                               <(o )___                   */
/*                                                  ( ._> /   - Weh.          */
/*   By: prp <tfm357@gmail.com>                    --`---'-------------       */
/*                                                 54 69 6E 66 6F 69 6C       */
/*   Created: 2017/08/05 12:41:20 by prp              2E 54 65 63 68          */
/*   Updated: 2017/08/06 17:27:07 by prp              50 2E 52 2E 50          */
/*                                                                            */
/* ************************************************************************** */

#ifndef DD_TASK_MANAGER_HPP
#define DD_TASK_MANAGER_HPP

#include "Task.hpp"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace DD {
class TaskManager {
	class TaskThread {
		std::atomic_bool to_continue;
		std::thread      thread_handle;

		static void run_loop(TaskManager* const manager,
		                     TaskThread* const  thread);

	public:
		TaskThread();
		TaskThread(TaskThread&& other);
		~TaskThread();
		void launch(TaskManager* const manager);
		void signal_stop();
		void join();
	};

	mutable std::mutex              sync;
	mutable std::condition_variable signal;

	std::queue<Task>        task_queue;
	std::vector<TaskThread> thread_pool;

public:
	TaskManager(int pool_size = std::thread::hardware_concurrency());

	~TaskManager();

	void add_task(Task task);

	Task pop_task();

	bool is_empty();

	size_t tasks_remaining();
};
}

#endif