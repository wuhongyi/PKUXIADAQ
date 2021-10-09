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

/** @file DSPWrHostRd_ExtFifoTest.cc
 * @brief Executes Control Task 11 to test the external FIFO.
 */

#include "pixie16app_common.h"
#include "pixie16app_export.h"
#include "pixie16app_globals.h"
#include "pixie16sys_export.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <string>
//#include <unistd.h>
#include <math.h>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[]) {
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char DSPCodeFile[80];
    char DSPParFile[80];
    char DSPVarFile[80];
    char EEPROMFile[80];
    char DummyFile[80];
    char ErrMSG[256];
    const char config[100] = "Configuration/.cfgPixie";
    unsigned short NumModules = 0;
    unsigned short ModNum = 0;
    unsigned short* PXISlotMap;
    unsigned short ByteCounter = 0;
    unsigned short Bytes = 0;
    unsigned int NumWords, TotalReadWords;
    int retval = 0;

    ifstream input;
    char* temp = new char[80];
    input.open(config, ios::in);

    if (input.fail()) {
        cout << "can't open the config file ! " << config << endl << flush;
        return false;
    }

    input >> NumModules;
    cout << "\n\n" << NumModules << " modules, in slots:";
    input.getline(temp, 80);
    PXISlotMap = new unsigned short[NumModules + 1];
    for (int i = 0; i < NumModules; i++) {
        input >> PXISlotMap[i];
        input.getline(temp, 80);
        cout << PXISlotMap[i] << " ";
    }

    //==== This code is necessary if modules are installed in two crates ====//
    //input >> PXISlotMap[NumModules];
    //input.getline (temp, 80);
    //cout << PXISlotMap[NumModules] << " ";

    input >> ComFPGAConfigFile;
    input.getline(temp, 80);

    input >> SPFPGAConfigFile;
    input.getline(temp, 80);

    input >> DSPCodeFile;
    input.getline(temp, 80);

    input >> DSPParFile;
    input.getline(temp, 80);

    input >> DummyFile;
    input.getline(temp, 80);

    input >> EEPROMFile;
    input.getline(temp, 80);

    input >> DSPVarFile;
    input.getline(temp, 80);

    input.close();
    input.clear();

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    cout << "Booting....\n";

    retval = Pixie16InitSystem(NumModules, PXISlotMap, 0);
    if (retval < 0) {
        printf("*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        sprintf(ErrMSG, "*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return (-1);
    } else {
        cout << "Init OK " << retval << endl;
    }

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, "", DSPCodeFile, DSPParFile,
                               DSPVarFile, NumModules, 0x00);
    if (retval < 0) {
        printf("*ERROR* Pixie16BootModule failed, retval = %d", retval);
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return (-2);
    } else {
        cout << "Boot OK " << retval << endl;
    }

    //==== Writes a block of data by the DSP to the external FIFO ====//
    cout << "DSP writes data to external FIFO... " << endl;
    retval = Pixie_Start_Run(NEW_RUN, 0, 11, 0);
    cout << "retval=" << retval << endl;

    //==== Read data from external FIFO ====//

    TotalReadWords = 0;

    do {
        cout << "Host reads data from external FIFO... " << endl;
        retval = Pixie16SaveExternalFIFODataToFile("lmdata0.bin", &NumWords, 0, 0);
        cout << "retval=" << retval << endl;
        cout << "Number of words read=" << NumWords << endl;

        TotalReadWords += NumWords;
    } while (TotalReadWords < 0x7A12000);

    return (1);
}
