/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file pixie16app_common.h
 * @brief Function prototypes shared among multiple files.
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16APP_COMMON_H
#define XIA_PIXIE16APP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------
	Functions that are only used locally
  -------------------------------------*/

int Pixie_Start_Run(unsigned short mode,  // mode = NEW_RUN or RESUME_RUN
                    unsigned short run_task,  // run task number
                    unsigned short control_task,  // control task number
                    unsigned short ModNum);

int Pixie_End_Run(unsigned short ModNum);

int Pixie_Check_Run_Status(unsigned short ModNum);

int Pixie_Control_Task_Run(
    unsigned short ModNum,  // Pixie module number
    unsigned short ControlTask,  // Control task number
    unsigned int Max_Poll);  // Timeout control in unit of ms for control task run

int Pixie_Broadcast(const char* str,  // variable name whose value is to be broadcasted
                    unsigned short SourceModule);  // the source module number

int Pixie_ComputeFIFO(unsigned int TraceDelay,  // current trace dealy value
                      unsigned short ModNum,  // Pixie module number
                      unsigned short ChanNum);  // Pixie channel number

int Pixie_CopyDSPParameters(unsigned short BitMask,  // copy/extract bit mask pattern
                            unsigned short SourceModule,  // source module number
                            unsigned short SourceChannel,  // source Pixie channel
                            unsigned short DestinationModule,  // destination module number
                            unsigned short DestinationChannel);  // destination channel number

int Pixie_Init_DSPVarAddress(const char* DSPVarFile, unsigned short ModNum);

int Pixie_Copy_DSPVarAddress(unsigned short SourceModNum, unsigned short DestinationModNum);

#ifdef __cplusplus
}
#endif

#endif
