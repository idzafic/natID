// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Thread.h
    @brief Utilities for scheduling functions to execute on the GUI main thread from worker threads. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/natGUI.h>
#include <functional>
#include <memory>
#include <td/Variant.h>
#include <thread>

namespace gui {
namespace thread {

using MainThreadFunction = std::function<void()>; ///< Callable with no parameters for main-thread dispatch.
using MainThreadFunction1 = std::function<void(td::Variant)>; ///< Callable that receives a Variant by value.
using MainThreadFunction2 = std::function<void(td::Variant&)>; ///< Callable that receives a Variant by reference (input/output).

using MainThreadSharedFunction = std::shared_ptr<MainThreadFunction>; ///< Shared pointer to a MainThreadFunction.
using MainThreadSharedFunction1 = std::shared_ptr<MainThreadFunction1>; ///< Shared pointer to a MainThreadFunction1.
using MainThreadSharedFunction2 = std::shared_ptr<MainThreadFunction2>; ///< Shared pointer to a MainThreadFunction2.

// low-level calls
/// @brief Schedules a function for asynchronous execution on the main thread.
/// @param fn Shared pointer to the function to execute.
NATGUI_API void asyncExecInMainThread(const MainThreadSharedFunction& fn);

/// @brief Schedules a function that receives a Variant for asynchronous execution on the main thread.
/// @param fn Shared pointer to the function to execute.
/// @param param Variant parameter passed to the function.
NATGUI_API void asyncExecInMainThread(const MainThreadSharedFunction1& fn, td::Variant param);

/// @brief Executes a function synchronously on the main thread and blocks until it completes.
/// @param fn Shared pointer to the function to execute.
NATGUI_API void syncExecInMainThread(const MainThreadSharedFunction& fn);

/// @brief Executes a function synchronously on the main thread, passing a Variant by reference.
/// @param fn Shared pointer to the function to execute.
/// @param param Variant that may serve as both input and output.
NATGUI_API void syncExecInMainThread(const MainThreadSharedFunction2& fn, td::Variant& param); //in this case, param can be input/output


// Templates for convenience
/// @brief Convenience template that asynchronously dispatches any callable to the main thread.
/// @tparam Fn Callable type (must not already be a MainThreadSharedFunction or MainThreadSharedFunction1).
/// @param fn Callable to dispatch.
template <typename Fn>
requires (!std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction> &&
          !std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction1>)
void asyncExecInMainThread(Fn&& fn)
{
    auto sharedFn = std::make_shared<MainThreadFunction>(std::forward<Fn>(fn));
    asyncExecInMainThread(sharedFn);
}

/// @brief Convenience template that asynchronously dispatches a callable with a Variant parameter to the main thread.
/// @tparam Fn Callable type (must not already be a MainThreadSharedFunction or MainThreadSharedFunction1).
/// @param fn Callable to dispatch.
/// @param param Variant parameter passed to the callable.
template <typename Fn>
requires (!std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction> &&
          !std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction1>)
void asyncExecInMainThread(Fn&& fn, const td::Variant& param)
{
    auto sharedFn = std::make_shared<MainThreadFunction1>(std::forward<Fn>(fn));
    asyncExecInMainThread(sharedFn, param);
}

//and for synchronous execution
/// @brief Convenience template that synchronously executes any callable on the main thread.
/// @tparam Fn Callable type (must not already be a MainThreadSharedFunction or MainThreadSharedFunction2).
/// @param fn Callable to execute.
template <typename Fn>
requires (!std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction> &&
          !std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction2>)
void syncExecInMainThread(Fn&& fn)
{
    auto sharedFn = std::make_shared<MainThreadFunction>(std::forward<Fn>(fn));
    syncExecInMainThread(sharedFn);
}

/// @brief Convenience template that synchronously executes a callable with a Variant reference on the main thread.
/// @tparam Fn Callable type (must not already be a MainThreadSharedFunction or MainThreadSharedFunction2).
/// @param fn Callable to execute.
/// @param param Variant that may serve as both input and output.
template <typename Fn>
requires (!std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction> &&
          !std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction2>)
void syncExecInMainThread(Fn&& fn, td::Variant& param)
{
    auto sharedFn = std::make_shared<MainThreadFunction2>(std::forward<Fn>(fn));
    syncExecInMainThread(sharedFn, param);
}

} // namespace thread
} // namespace gui


