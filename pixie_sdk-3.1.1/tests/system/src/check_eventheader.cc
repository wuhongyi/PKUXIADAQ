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

/** @file check_eventheader.cc
 * @brief Checks event headers in a list-mode data file.
 */

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

#define Debug_Print 1
#define MAX_HEADERLEN 18

using namespace std;

int main(int argc, char* argv[]) {
    unsigned short NumModules;
    unsigned short* PXISlotMap;
    unsigned short OfflineMode;
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char TrigFPGAConfigFile[80] = "";
    char DSPCodeFile[80];
    char DSPParFile_G09[80];
    char DSPParFile_G40[80];
    char DSPParFile_LMTest[80];
    char EEPROMFile[80];
    char DSPVarFile[80];
    char ListModeDataFile[80];
    char ErrMSG[256];
    int retval = 0;

    unsigned long HeaderData[MAX_HEADERLEN];
    unsigned long TraceLength;
    unsigned short RcdHeaderLen, Trace_Data[8192];
    unsigned long TotalSkippedWords, TotalWords;

    unsigned short channum, slotnum, cratenum;

    int i;

    FILE* ListModeFile = NULL;

    ////////read configuration file (cfgPixie16.txt)///////////////////////////////////
    const char inffile[200] = "Configuration/.cfgPixie";

    ifstream input;
    char* temp = new char[80];
    input.open(inffile, ios::in);

    if (input.fail()) {
        cout << "can't open the configuration file" << inffile << endl << flush;
        return 0;
    }

    input >> NumModules;
    cout << "\n\n" << NumModules << " modules, in slots:";
    input.getline(temp, 80);
    PXISlotMap = new unsigned short[NumModules + 1];
    for (i = 0; i < NumModules; i++) {
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

    input >> DSPParFile_LMTest;
    input.getline(temp, 80);
    cout << "DSPParFile_LMTest:         " << DSPParFile_LMTest << endl;

    input >> ListModeDataFile;
    input.getline(temp, 80);
    input.clear();

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    cout << "Booting into Offline mode....\n";

    OfflineMode = 1;
    retval = Pixie16InitSystem(NumModules, PXISlotMap, OfflineMode);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -1;
    } else {
        cout << "Init OK " << retval << endl;
    }

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, TrigFPGAConfigFile, DSPCodeFile,
                               DSPParFile_LMTest, DSPVarFile, NumModules, 0x7F);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -2;
    } else {
        cout << "Boot OK " << retval << endl;
    }

    i = 0;
LoopModules:

    sprintf(ListModeDataFile, "lmdata_mod%d.bin", i);

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    cout << "Parsing list mode data file....\n";

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    cout << "Checking event headers in module " << i << "....\n";

    ListModeFile = fopen(ListModeDataFile, "rb");
    if (ListModeFile != NULL) {
        // Get file length
        fseek(ListModeFile, 0, SEEK_END);
        TotalWords = (ftell(ListModeFile) + 1) / 4;
        // Point ListModeFile to the beginning of file
        fseek(ListModeFile, 0, SEEK_SET);

        // Initialize indicator and counter
        TotalSkippedWords = 0;

        do {
            // Read the header
            fread(HeaderData, 4, 1, ListModeFile);

            channum = HeaderData[0] & 0xF;
            slotnum = (HeaderData[0] & 0xF0) >> 4;
            cratenum = (HeaderData[0] & 0xF00) >> 8;
            RcdHeaderLen = (HeaderData[0] & 0x1F000) >> 12;

            if (cratenum > 0) {
                printf("*ERROR* wrong crate number %d\n", cratenum);
                printf("*Prior trace length =  %d\n", TraceLength);
                printf("*Total skipped words  =  %d\n", TotalSkippedWords);
                fclose(ListModeFile);
                goto ContinueNextMod;
            }
            if ((slotnum < 2) || (slotnum > 14)) {
                printf("*ERROR* wrong slot number %d\n", slotnum);
                printf("*Prior trace length =  %d\n", TraceLength);
                printf("*Total skipped words  =  %d\n", TotalSkippedWords);
                fclose(ListModeFile);
                goto ContinueNextMod;
            }
            fread(HeaderData, 4, (RcdHeaderLen - 1), ListModeFile);
            TotalSkippedWords += RcdHeaderLen;

            TraceLength = (HeaderData[2] & 0x7FFF0000) >> 16;

            // Read trace data
            fread(Trace_Data, 2, TraceLength, ListModeFile);
            TotalSkippedWords += TraceLength / 2;
        } while (TotalSkippedWords < TotalWords);

        fclose(ListModeFile);
    } else {
        printf("*ERROR* can't open list mode data file %s\n", ListModeDataFile);
        return (-2);
    }

ContinueNextMod:
    i++;
    if (i < NumModules)
        goto LoopModules;

    return 1;
}
