/* ************************************************************************** */
/*                                                                            */
/*                                                   __                       */
/*   main.cpp                                      <(o )___                   */
/*                                                  ( ._> /   - Weh.          */
/*   By: prp <tfm357@gmail.com>                    --`---'-------------       */
/*                                                 54 69 6E 66 6F 69 6C       */
/*   Created: 2017/08/05 19:45:10 by prp              2E 54 65 63 68          */
/*   Updated: 2017/08/06 12:30:02 by prp              50 2E 52 2E 50          */
/*                                                                            */
/* ************************************************************************** */

#include "TaskManager.hpp"

#include <chrono>
#include <iostream>
#include <random>

// std::mutex cout_lock;

int main(int argc, char const* argv[]) {
	DD::TaskManager task_manager;
	for (int i = 0; i < argc; ++i) {
		// cout_lock.lock();
		std::cout << "Adding: Task #" << i << '\n';
		// cout_lock.unlock();
		const auto init = [i] {
			// cout_lock.lock();
			std::cout << "Initializing: Task #" << i << '\n';
			// cout_lock.unlock();
		};
		const auto run = [i, argv]() -> DD::Task::Result {
			std::random_device rd;
			// cout_lock.lock();
			std::cout << "Executing: Task #" << i << " arv[" << i
			          << "]: " << argv[i] << '\n';
			// cout_lock.unlock();
			const auto ms = std::chrono::milliseconds(rd() % 100);
			std::this_thread::sleep_for(ms);
			if ((int)rd() < 0)
				return DD::Task::Result::Finished;
			return DD::Task::Result::Continue;
		};
		const auto destroy = [i] {
			// cout_lock.lock();
			std::cout << "Destroying: Task #" << i << '\n';
			// cout_lock.unlock();
		};
		task_manager.add_task(DD::Task(init, run, destroy));
	}
	// cout_lock.lock();
	std::cout << "Press `Enter` to exit program.\n";
	// cout_lock.unlock();
	std::string line;
	std::getline(std::cin, line);
	return 0;
}
