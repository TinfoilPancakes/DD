/* ************************************************************************** */
/*                                                                            */
/*                                                   __                       */
/*   TaskManager.cpp                               <(o )___                   */
/*                                                  ( ._> /   - Weh.          */
/*   By: prp <tfm357@gmail.com>                    --`---'-------------       */
/*                                                 54 69 6E 66 6F 69 6C       */
/*   Created: 2017/08/05 17:11:13 by prp              2E 54 65 63 68          */
/*   Updated: 2017/08/06 17:27:57 by prp              50 2E 52 2E 50          */
/*                                                                            */
/* ************************************************************************** */

#include "TaskManager.hpp"

#include <iostream>

using namespace DD;

TaskManager::TaskThread::TaskThread() { this->to_continue = true; }

TaskManager::TaskThread::TaskThread(TaskThread&& other) {
	this->to_continue = other.to_continue.load();
	this->thread_handle = std::move(other.thread_handle);
}

TaskManager::TaskThread::~TaskThread() {
	this->to_continue = false;
	if (this->thread_handle.joinable())
		this->thread_handle.join();
}

void TaskManager::TaskThread::run_loop(TaskManager* const manager,
                                       TaskThread* const  thread) {
	while (thread->to_continue) {
		std::unique_lock<std::mutex> locker(manager->sync);
		manager->signal.wait(locker, [manager, thread] {
			return !manager->task_queue.empty() || !thread->to_continue;
		});
		locker.unlock();

		if (!thread->to_continue)
			break;

		Task work = manager->pop_task();

		if (work.get_should_init()) {
			work.init();
			work.set_should_init(false);
		}

		auto result = work.run();
		work.increment_iter_count();

		if (result == Task::Result::FinishedNoDestroy)
			continue;
		if (result == Task::Result::Finished)
			work.destroy();
		else {
			if (result == Task::Result::ContinueReInit)
				work.set_should_init(true);
			manager->add_task(work);
		}
	}
}

void TaskManager::TaskThread::launch(TaskManager* const manager) {
	this->thread_handle = std::thread(TaskThread::run_loop, manager, this);
}

void TaskManager::TaskThread::signal_stop() { this->to_continue = false; }

void TaskManager::TaskThread::join() {
	if (this->thread_handle.joinable())
		this->thread_handle.join();
}

TaskManager::TaskManager(int pool_size) {
	std::lock_guard<std::mutex> guard(this->sync);
	for (int i = 0; i < pool_size; ++i) {
		this->thread_pool.push_back(TaskThread());
	}
	for (auto& worker : this->thread_pool)
		worker.launch(this);
}

TaskManager::~TaskManager() {
	for (auto& thread : this->thread_pool) {
		thread.signal_stop();
	}

	this->signal.notify_all();

	for (auto& thread : this->thread_pool) {
		thread.join();
	}
}

void TaskManager::add_task(DD::Task task) {
	std::unique_lock<std::mutex> lock(this->sync);

	this->task_queue.push(task);
	lock.unlock();
	this->signal.notify_one();
}

Task TaskManager::pop_task() {
	std::lock_guard<std::mutex> lock(this->sync);

	auto task = this->task_queue.front();
	this->task_queue.pop();
	return task;
}

bool TaskManager::is_empty() {
	std::lock_guard<std::mutex> lock(this->sync);

	return this->task_queue.empty();
}

size_t TaskManager::tasks_remaining() {
	std::lock_guard<std::mutex> lock(this->sync);

	return this->task_queue.size();
}
