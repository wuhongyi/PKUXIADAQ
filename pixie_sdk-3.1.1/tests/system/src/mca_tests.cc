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

/** @file mca_tests.cc
 * @brief Executes tests against the MCA for QA purposes.
 */

#include <math.h>
#include <signal.h>
#include <windows.h>

#include "pixie16app_export.h"
#include "pixie16app_globals.h"
#include "pixie16sys_export.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//////////////////////////////////////

using namespace std;

//const int FALSE = 0, TRUE = 1;

int test_analogfrontend(unsigned short nummodules);
double get_average(unsigned short* data, unsigned long numpnts);
double get_deviation(unsigned short* data, unsigned long numpnts, double avg);

int main(int argc, char* argv[]) {
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char DSPCodeFile[80];
    char DSPParFile_G09[80];
    char DSPParFile_G40[80];
    char DSPVarFile[80];
    char EEPROMFile[80];
    char ErrMSG[256];
    char filnam[256];
    char IOBytes[PRESET_MAX_MODULES][8192];
    char command[80];
    char setfilenam[256];
    unsigned short ModSerNum[PRESET_MAX_MODULES];
    unsigned short NumModules;
    unsigned short* PXISlotMap;
    unsigned short SN = 0;
    int retval = 0;
    int k = 0;
    long runtime_IEEE;
    double runtime;

    ////////read cfg file (.cfgPixie)///////////////////////////////////
    const char config[100] = "configuration/.cfgPixie";

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

    input >> DSPCodeFile;
    input.getline(temp, 80);
    cout << "DSPCodeFile:        " << DSPCodeFile << endl;

    input >> DSPParFile_G09;
    input.getline(temp, 80);
    cout << "DSPParFile_G09:         " << DSPParFile_G09 << endl;

    input >> DSPParFile_G40;
    input.getline(temp, 80);
    cout << "DSPParFile_G40:         " << DSPParFile_G40 << endl;

    input >> EEPROMFile;
    input.getline(temp, 80);
    cout << "EEPROMFile:         " << EEPROMFile << endl;

    input >> DSPVarFile;
    input.getline(temp, 80);
    cout << "DSPVarFile:         " << DSPVarFile << endl;

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

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, "", DSPCodeFile, DSPParFile_G09,
                               DSPVarFile, NumModules, 0x7F);
    if (retval < 0) {
        printf("*ERROR* Pixie16BootModule @ gain = 0.9 failed, retval = %d", retval);
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule @ gain = 0.9 failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return (-2);
    } else {
        cout << "Boot all modules @ gain = 0.9 ok " << retval << endl;
    }

    //==== Testing mca ====//
    cout << "Testing mca ... " << endl;

    // Read I2C-EEPROM of each module
    for (k = 0; k < NumModules; k++) {
        retval = I2CM24C64_Sequential_Read(k, 0, 8192, IOBytes[k]);
        if (retval < 0) {
            printf("*ERROR* Could not read I2C-EEPROM in Module=%d; retval=%d", k, retval);
            sprintf(ErrMSG, "*ERROR* Could not read I2C-EEPROM in Module=%d; retval=%d", k, retval);
            Pixie_Print_MSG(ErrMSG);
            return (-3);
        }

        SN = (unsigned short) (unsigned char) IOBytes[k][0] +
             256 * (unsigned short) (unsigned char) IOBytes[k][1];

        if (IOBytes[k][2] >= 11) {
            ModSerNum[k] = SN;
        } else {
            ModSerNum[k] = (unsigned short) (unsigned char) IOBytes[k][0];
        }
        printf("Module S/N: %hu\n", ModSerNum[k]);
    }

    // Acquire MCA histograms at gain 0.9 and 4.0
    for (k = 0; k < NumModules; k++) {
        // Wait for system ready - user needs to connect the 16 input signals
        printf("\nPlease enter 'S' to start acquiring mca histograms in module %d when ready ", k);
        do {
            scanf("%s", command);
        } while (strcmp(command, "S") != 0);

        // Adjust offsets for all channels
        retval = Pixie16AdjustOffsets(k);
        if (retval < 0) {
            printf("Error adjust offsets in module %d at gain 0.9\n", k);
        }

        cout << "MCA run @ gain 0.9 in progress, please wait ..." << endl;

        // MCA run
        runtime = 120.;  // in seconds
        runtime_IEEE =
            Decimal2IEEEFloating(runtime);  // convert to IEEE 32-bit floating point format
        retval = Pixie16WriteSglModPar("HOST_RT_PRESET", runtime_IEEE, k);
        if (retval < 0) {
            printf("Error writing host preset run time\n");
        }

        ///////////////////////////////////////////////////
        // Disable synchronizing modules
        retval = Pixie16WriteSglModPar("SYNCH_WAIT", 0, k);
        if (retval < 0)
            cout << "Synch Wait problem " << retval << endl;

        retval = Pixie16WriteSglModPar("IN_SYNCH", 1, k);
        if (retval < 0)
            cout << "In Sync problem " << retval << endl;
        //////////////////////////////

        // Start mca run
        retval = Pixie16StartHistogramRun(k, NEW_RUN);
        // Wait for mca run to stop
        Sleep((unsigned int) (runtime * 1000.0));
        // Stop the run
        retval = Pixie16EndRun(k);
        // Read out the mca histograms and save them to a file
        sprintf(filnam, "mcahistogram_mod#%hu_g09.bin", ModSerNum[k]);
        retval = Pixie16SaveHistogramToFile(filnam, k);

        // Save DSP settings to file
        sprintf(setfilenam, "mcatest_g0.9_mod%d.set", k);
        retval = Pixie16SaveDSPParametersToFile(setfilenam);

        // Set gain to 4.0
        retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, "", DSPCodeFile,
                                   DSPParFile_G40, DSPVarFile, k, 0x70);
        if (retval < 0) {
            sprintf(ErrMSG, "*ERROR* Pixie16BootModule @ gain = 4.0 failed, retval = %d", retval);
            Pixie_Print_MSG(ErrMSG);
            return (-4);
        } else {
            cout << "Boot @ gain = 4.0 OK " << k << endl;
        }

        // Adjust offsets for all channels
        retval = Pixie16AdjustOffsets(k);
        if (retval < 0) {
            printf("Error adjust offsets in module %d at gain 4.0\n", k);
        }

        // Write runtime to DSP
        runtime_IEEE =
            Decimal2IEEEFloating(runtime);  // convert to IEEE 32-bit floating point format
        retval = Pixie16WriteSglModPar("HOST_RT_PRESET", runtime_IEEE, k);
        if (retval < 0) {
            printf("Error writing host preset run time\n");
        }

        ///////////////////////////////////////////////////
        // Disable synchronizing modules
        retval = Pixie16WriteSglModPar("SYNCH_WAIT", 0, k);
        if (retval < 0)
            cout << "Synch Wait problem " << retval << endl;

        retval = Pixie16WriteSglModPar("IN_SYNCH", 1, k);
        if (retval < 0)
            cout << "In Sync problem " << retval << endl;
        //////////////////////////////

        // Start mca run
        retval = Pixie16StartHistogramRun(k, NEW_RUN);

        cout << "MCA run @ gain 4.0 in progress, please wait ..." << endl;

        // Wait for mca run to stop
        Sleep((unsigned int) (runtime * 1000.0));
        // Stop the run
        retval = Pixie16EndRun(k);
        // Read out the mca histograms and save them to a file
        sprintf(filnam, "mcahistogram_mod#%hu_g40.bin", ModSerNum[k]);
        retval = Pixie16SaveHistogramToFile(filnam, k);

        // Save DSP settings to file
        sprintf(setfilenam, "mcatest_g4.0_mod%d.set", k);
        retval = Pixie16SaveDSPParametersToFile(setfilenam);
    }
    return (1);
}
