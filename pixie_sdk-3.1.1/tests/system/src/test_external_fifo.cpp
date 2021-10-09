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

/** @file test_external_fifo.cpp
 * @brief Tests the external FIFO functionality on a module.
 */

#include "pixie16app_common.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <math.h>
#include <signal.h>
#include <sstream>
#include <string>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

int main(int argc, char* argv[]) {
    unsigned short NumModules;
    unsigned short* PXISlotMap;
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char TrigFPGAConfigFile[80];
    char DSPCodeFile[80];
    char DSPParFile[80];
    char DSPVarFile[80];
    char ErrMSG[256];
    int retval = 0;
    unsigned int nFIFOWords[24];
    unsigned int* lmdata;
    const unsigned long readthresh = 16384;
    const unsigned int eventlen = 2516;
    unsigned int readevents;
    unsigned int totalerrors;

    ////////read cfg file (cfgPixie16.txt)///////////////////////////////////
    const char config[20] = "cfgPixie16.txt";

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

    cout << endl << "Firmware files: \n";
    input >> ComFPGAConfigFile;
    input.getline(temp, 80);
    cout << "ComFPGAConfigFile:  " << ComFPGAConfigFile << endl;
    input >> SPFPGAConfigFile;
    input.getline(temp, 80);
    cout << "SPFPGAConfigFile:   " << SPFPGAConfigFile << endl;
    input >> TrigFPGAConfigFile;
    input.getline(temp, 80);
    cout << "TrigFPGAConfigFile: " << TrigFPGAConfigFile << endl;
    input >> DSPCodeFile;
    input.getline(temp, 80);
    cout << "DSPCodeFile:        " << DSPCodeFile << endl;
    input >> DSPParFile;
    input.getline(temp, 80);
    cout << "DSPParFile:         " << DSPParFile << endl;
    input >> DSPVarFile;
    input.getline(temp, 80);
    cout << "DSPVarFile:         " << DSPVarFile << endl;
    input.close();
    input.clear();

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    cout << "Booting...\n";

    retval = Pixie16InitSystem(NumModules, PXISlotMap, 0);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -1;
    } else {
        cout << "Init OK " << retval << endl;
    }

    ///////////////////////////////////////////////////
    // Here we use Boot Pattern 0x70, i.e. only loads the
    // DSP parameters, Program FIPPIS and Set DACs, etc. We assume
    // the Pixie-16 modules have been booted using either nscope or
    // other programs. The truth is we don't need to reboot the
    // Pixie-16 modules each time when we try to acquire data.

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, TrigFPGAConfigFile, DSPCodeFile,
                               DSPParFile, DSPVarFile, NumModules, 0x70);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -2;
    } else {
        cout << "Boot OK " << retval << endl;
    }

    // Adjust DC-Offsets
    for (int k = 0; k < NumModules; k++) {
        retval = Pixie16AdjustOffsets(k);
        if (retval < 0) {
            sprintf(ErrMSG, "*ERROR* Pixie16AdjustOffsets in module %d failed, retval = %d", k,
                    retval);
            Pixie_Print_MSG(ErrMSG);
            return -6;
        }
    }

    ///////////////////////////////////////////////////
    // Synchronize modules
    int modnum = 0;
    retval = Pixie16WriteSglModPar("SYNCH_WAIT", 1, modnum);
    if (retval < 0)
        cout << "Synch Wait problem " << retval << endl;
    else
        cout << "Synch Wait OK " << retval << endl;

    retval = Pixie16WriteSglModPar("IN_SYNCH", 0, modnum);
    if (retval < 0)
        cout << "In Sync problem " << retval << endl;
    else
        cout << "In Synch OK " << retval << endl;
    //////////////////////////////

    retval = Pixie16StartListModeRun(NumModules, 0x100, NEW_RUN);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16StartListModeRun failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -3;
    } else
        cout << "List Mode started OK " << retval << endl << flush;

    usleep(100000);  //delay for the DSP run start

    if ((lmdata = (unsigned int*) malloc(sizeof(unsigned int) * 131072)) == NULL) {
        printf("failed to allocate memory block lmdata\n");
        return -4;
    }

    // Clear counters to 0 (counters keep track of how many words each module has read)
    for (int k = 0; k < NumModules; k++) {
        nFIFOWords[k] = 0;
    }

    readevents = 0;
    totalerrors = 0;

    //////////////////////////////////////////////
    // Acquiring data
    while (1) {
        for (int k = 0; k < NumModules; k++) {
            retval = Pixie_Read_ExtFIFOStatus(&nFIFOWords[k], k);
            if (retval < 0) {
                sprintf(ErrMSG, "*ERROR* read Pixie status failed, retval = %d", retval);
                Pixie_Print_MSG(ErrMSG);
                return -5;
            }
            if (nFIFOWords[k] > readthresh) {
                retval = Pixie_ExtFIFO_Read(lmdata, eventlen, k);
                if (retval < 0) {
                    sprintf(ErrMSG, "*ERROR* Pixie_ExtFIFO_Read failed in module %d, retval = %d",
                            k, retval);
                    Pixie_Print_MSG(ErrMSG);
                    retval = Pixie16EndRun(0);
                    if (retval < 0) {
                        free(lmdata);
                        printf("end run #1 failed, retval=%d\n", retval);
                        return -6;
                    }
                    free(lmdata);
                    return -7;
                }

                // Data check
                if (((lmdata[0] & 0xF0) >> 4) != (unsigned int) (k + 2)) {
                    totalerrors++;
                    printf("read data error: slot number %d, total # of errors=%d\n",
                           (lmdata[0] & 0xF0) / 16, totalerrors);
                }
                if (((lmdata[0] & 0xF00) >> 8) != 0) {
                    totalerrors++;
                    printf("read data error: crate number %d, total # of errors=%d\n",
                           (lmdata[0] & 0xF0) / 16, totalerrors);
                }

                readevents++;

                if (fmod(readevents, 1000) == 0) {
                    printf("read %d events, total # of errors=%d\n", readevents, totalerrors);
                }
            }
        }
        if (readevents >= 4294967294)
            break;
    }

    cout << "Run ended \n" << flush;
    exit(1);
}
