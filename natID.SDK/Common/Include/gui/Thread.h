// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

using MainThreadFunction = std::function<void()>;
using MainThreadFunction1 = std::function<void(td::Variant)>;

using MainThreadSharedFunction = std::shared_ptr<MainThreadFunction>;
using MainThreadSharedFunction1 = std::shared_ptr<MainThreadFunction1>;

// low-level calls
NATGUI_API void asyncExecInMainThread(const MainThreadSharedFunction& fn);
NATGUI_API void asyncExecInMainThread(const MainThreadSharedFunction1& fn, td::Variant param);


// Templates for convenience
template <typename Fn>
requires (!std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction> &&
          !std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction1>)
void asyncExecInMainThread(Fn&& fn)
{
    auto sharedFn = std::make_shared<MainThreadFunction>(std::forward<Fn>(fn));
    asyncExecInMainThread(sharedFn);
}

template <typename Fn>
requires (!std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction> &&
          !std::is_same_v<std::decay_t<Fn>, MainThreadSharedFunction1>)
void asyncExecInMainThread(Fn&& fn, const td::Variant& param)
{
    auto sharedFn = std::make_shared<MainThreadFunction1>(std::forward<Fn>(fn));
    asyncExecInMainThread(sharedFn, param);
}

} // namespace thread
} // namespace gui


