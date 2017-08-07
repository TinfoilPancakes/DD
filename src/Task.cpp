/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppatil <ppatil@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 09:49:52 by ppatil            #+#    #+#             */
/*   Updated: 2017/08/05 09:49:58 by ppatil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"

using namespace DD;

Task::Task(const TaskInit init, const TaskRun run, const TaskDestroy destroy)
    : to_init(init), to_run(run), to_destroy(destroy) {
	this->run_init = true;
	this->iter_count = 0;
};
