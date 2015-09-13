/*
===========================================================================

Copyright (c) 2010-2015 Darkstar Dev Teams

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

This file is part of DarkStar-server source code.

===========================================================================
*/

#ifndef _AIBASE_H
#define _AIBASE_H

#include <memory>
#include <stack>

#include "ai_common.h"
#include "controllers/controller.h"
#include "helpers/action_queue.h"
#include "helpers/pathfind.h"
#include "helpers/event_handler.h"

class CBaseEntity;
class CState;

class CAIBase
{
public:
    CAIBase(CBaseEntity*);
    CAIBase(CBaseEntity*, std::unique_ptr<CPathFind>&&, std::unique_ptr<CController>&&);

    //no copy construct/assign (only move)
    CAIBase(const CAIBase&) = delete;
    CAIBase& operator=(const CAIBase&) = delete;

    void Tick(time_point _tick);
    CState* GetCurrentState();
    bool IsStateStackEmpty();
    //whether AI is currently able to change state from external means
    virtual bool CanChangeState();

    time_point getTick();
    time_point getPrevTick();

    virtual void queueAction(queueAction_t&&);

    // stores all events and their associated lua callbacks
    CAIEventHandler EventHandler;

protected:
    // pathfinder, not guaranteed to be implemented
    std::unique_ptr<CPathFind> pathfind;
    // input controller
    std::unique_ptr<CController> Controller;
    // current synchronized server time (before AI loop execution)
    time_point m_Tick;
    time_point m_PrevTick;
    //entity who holds this AI
    CBaseEntity* PEntity;

    template<typename T, typename... Args>
    void ChangeState(Args&&... args) { if (CanChangeState()) { m_stateStack.emplace(std::make_unique<T>(std::forward<Args>(args)...)); } }

private:
    std::stack<std::unique_ptr<CState>> m_stateStack;
    CAIActionQueue ActionQueue;
};

#endif