/* ************************************************************************** */
/*                                                                            */
/*                                                   __                       */
/*   Task.hpp                                      <(o )___                   */
/*                                                  ( ._> /   - Weh.          */
/*   By: prp <tfm357@gmail.com>                    --`---'-------------       */
/*                                                 54 69 6E 66 6F 69 6C       */
/*   Created: 2017/08/05 09:50:04 by prp              2E 54 65 63 68          */
/*   Updated: 2017/08/05 20:09:20 by prp              50 2E 52 2E 50          */
/*                                                                            */
/* ************************************************************************** */

#ifndef DD_TASK_HPP
#define DD_TASK_HPP

#include <atomic>
#include <functional>

namespace DD {
class Task {
public:
	enum Result { Finished, FinishedNoDestroy, Continue, ContinueReInit };

private:
	using TaskInit = std::function<void()>;
	using TaskRun = std::function<Result()>;
	using TaskDestroy = std::function<void()>;

	const TaskInit to_init;

	const TaskRun to_run;

	const TaskDestroy to_destroy;

	size_t iter_count;
	bool run_init;

public:
	Task(const TaskInit    to_init,
	     const TaskRun     to_run,
	     const TaskDestroy to_destroy);

	inline Result run() const {
		return this->to_run != nullptr ? this->to_run() : Result::Finished;
	}

	inline void destroy() const {
		if (this->to_destroy != nullptr)
			this->to_destroy();
	}

	inline void init() const {
		if (this->to_init != nullptr)
			this->to_init();
	}

	inline const TaskRun& get_run() const { return this->to_run; }

	inline const TaskInit& get_init() const { return this->to_init; }

	inline const TaskDestroy& get_destroy() const { return this->to_destroy; }

	inline bool get_should_init() const {
		return this->run_init;
	}

	inline void set_should_init(bool should_init) {
		this->run_init = should_init;
	}

	inline void increment_iter_count() {
		this->iter_count++;
	}

	inline size_t get_iter_count() const {
		return this->iter_count;
	}

	inline void clear_iter_count() {
		this->iter_count = 0;
	}
};
}

#endif