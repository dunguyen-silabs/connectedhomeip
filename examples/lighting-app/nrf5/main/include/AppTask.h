/*
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "AppEvent.h"
#include "LightingManager.h"

class AppTask
{
public:
    int StartAppTask();
    static void AppTaskMain(void * pvParameter);

    void PostLightingActionRequest(LightingManager::Action_t aAction);
    void PostEvent(const AppEvent * event);

private:
    friend AppTask & GetAppTask(void);

    int Init();

    static void ActionInitiated(LightingManager::Action_t aAction);
    static void ActionCompleted(LightingManager::Action_t aAction);

    void CancelTimer(void);

    void DispatchEvent(AppEvent * event);
    void UpdateClusterState(void);

    static void FunctionTimerEventHandler(AppEvent * aEvent);
    static void FunctionHandler(AppEvent * aEvent);
    static void LightingActionEventHandler(AppEvent * aEvent);

    static void ButtonEventHandler(uint8_t pin_no, uint8_t button_action);
    static void TimerEventHandler(void * p_context);

    void StartTimer(uint32_t aTimeoutInMs);

    enum Function_t
    {
        kFunction_NoneSelected   = 0,
        kFunction_SoftwareUpdate = 0,
        kFunction_FactoryReset,

        kFunction_Invalid
    } Function;

    Function_t mFunction;
    bool mFunctionTimerActive;

    static AppTask sAppTask;
};

inline AppTask & GetAppTask(void)
{
    return AppTask::sAppTask;
}
