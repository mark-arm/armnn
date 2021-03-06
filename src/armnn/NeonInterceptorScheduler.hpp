//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#pragma once

#include "NeonTimer.hpp"
#include "WallClockTimer.hpp"

#include <arm_compute/runtime/IScheduler.h>
#include <arm_compute/runtime/Scheduler.h>
#include <arm_compute/core/CPP/ICPPKernel.h>

namespace armnn
{

class NeonInterceptorScheduler : public arm_compute::IScheduler
{
public:
    NeonInterceptorScheduler(NeonTimer::KernelMeasurements &kernels, arm_compute::IScheduler &realScheduler);
    ~NeonInterceptorScheduler() = default;

    void set_num_threads(unsigned int numThreads) override;

    unsigned int num_threads() const override;

    void schedule(arm_compute::ICPPKernel *kernel, const Hints &hints) override;

    void run_workloads(std::vector<Workload> &workloads) override;

private:
    NeonTimer::KernelMeasurements& m_Kernels;
    arm_compute::IScheduler& m_RealScheduler;
    WallClockTimer m_Timer;
};

} // namespace armnn
