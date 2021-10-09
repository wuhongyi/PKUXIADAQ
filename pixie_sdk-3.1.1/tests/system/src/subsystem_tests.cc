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

/** @file subsystem_tests.cc
 * @brief Part of the QA test suite to verify analog front end functionality.
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
//#include <unistd.h>
#include <math.h>
#include <windows.h>

using namespace std;

int test_analogfrontend(unsigned short nummodules);
double get_average(unsigned short* data, unsigned long numpnts);
double get_deviation(unsigned short* data, unsigned long numpnts, double avg);

int main(int argc, char* argv[]) {
    unsigned short NumModules;
    unsigned short* PXISlotMap;
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char DSPCodeFile[80];
    char DSPParFile[80];
    char DSPVarFile[80];
    char EEPROMFile[80];
    char DummyFile[80];
    char ErrMSG[256];
    const char config[100] = "Configuration/.cfgPixie";
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

    cout << endl << "Firmware files: \n";
    input >> ComFPGAConfigFile;
    input.getline(temp, 80);
    cout << "ComFPGAConfigFile:  " << ComFPGAConfigFile << endl;
    input >> SPFPGAConfigFile;
    input.getline(temp, 80);
    cout << "SPFPGAConfigFile:   " << SPFPGAConfigFile << endl;
    //input >> TrigFPGAConfigFile;
    //input.getline (temp, 80);
    //cout << "TrigFPGAConfigFile: " << TrigFPGAConfigFile << endl;
    input >> DSPCodeFile;
    input.getline(temp, 80);
    cout << "DSPCodeFile:        " << DSPCodeFile << endl;

    input >> DSPParFile;
    input.getline(temp, 80);
    cout << "DSPParFile:         " << DSPParFile << endl;

    input >> DummyFile;
    input.getline(temp, 80);
    cout << "DummyFile:         " << DummyFile << endl;

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

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, "", DSPCodeFile, DSPParFile,
                               DSPVarFile, NumModules, 0x7F);
    if (retval < 0) {
        printf("*ERROR* Pixie16BootModule failed, retval = %d", retval);
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return (-2);
    } else {
        cout << "Boot OK " << retval << endl;
    }

    //Pixie_Init_DSPVarAddress(DSPVarFile, 0);

    //==== Test analog front end ====//
    cout << "Testing analog front end " << endl;
    retval = test_analogfrontend(NumModules);

    return (1);
}

int test_analogfrontend(unsigned short nummodules) {
    unsigned short m, n, i;
    unsigned short ADCTrace[MAX_ADC_TRACE_LEN];
    unsigned short ModSerNum[PRESET_MAX_MODULES];
    unsigned short SN = 0;
    unsigned long k, offsetdac;
    double offsetvolts, avg, dev;
    FILE* datfil[PRESET_MAX_MODULES];
    char IOBytes[PRESET_MAX_MODULES][8192];
    char ErrMSG[256], filnam[256];
    double fftTrace[MAX_ADC_TRACE_LEN * 2];
    double power[MAX_ADC_TRACE_LEN];
    int retval;

    // Check if nummodules is valid
    if (nummodules > PRESET_MAX_MODULES) {
        sprintf(
            ErrMSG,
            "*ERROR* (test_analogfrontend): nummodules %d exceeds maximum allowed number of modules %d",
            nummodules, PRESET_MAX_MODULES);
        Pixie_Print_MSG(ErrMSG);
        return (-1);
    }

    // Read I2C-EEPROM of each module
    for (k = 0; k < nummodules; k++) {
        retval = I2CM24C64_Sequential_Read((unsigned short) k, 0, 8192, IOBytes[k]);
        if (retval < 0) {
            sprintf(
                ErrMSG,
                "*ERROR* (test_analogfrontend): Could not read I2C-EEPROM in Module=%ld; retval=%d",
                k, retval);
            Pixie_Print_MSG(ErrMSG);
            return (-2);
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

    // Make output data files for all modules
    for (k = 0; k < nummodules; k++) {
        sprintf(filnam, "analogfrontend_test_mod#%hu.dat", ModSerNum[k]);
        datfil[k] = fopen(filnam, "w");
        if (datfil[k] == NULL) {
            sprintf(ErrMSG, "*ERROR* (test_analogfrontend): can't open data file %s", filnam);
            Pixie_Print_MSG(ErrMSG);

            // Close files that were already opened
            for (m = 0; m < k; m++) {
                fclose(datfil[m]);
            }
            return (-3);
        }
    }

    // Write module information to file
    for (k = 0; k < nummodules; k++) {
        fprintf(datfil[k], "Module serial number: %hu\n\n", ModSerNum[k]);
        fprintf(datfil[k], "Module I2C-EEPROM information:\n");
        for (m = 0; m < 8192; m++) {
            fprintf(datfil[k], "%d\n", (unsigned char) IOBytes[k][m]);
        }
    }

    // Going through each of the 65536 steps of the 16-bit DAC
    for (k = 0; k < nummodules; k++) {
        fprintf(datfil[k], "\nADC values vs. OffsetDAC steps\n\n");
    }
    for (k = 0; k < 65535; k++) {
        offsetvolts = (double) k / 65536.0 * DAC_VOLTAGE_RANGE - DAC_VOLTAGE_RANGE / 2.0;

        for (m = 0; m < nummodules; m++) {
            for (n = 0; n < 16; n++) {
                retval = Pixie16WriteSglChanPar("VOFFSET", offsetvolts, m, n);
                if (retval < 0) {
                    sprintf(
                        ErrMSG,
                        "*ERROR* (test_analogfrontend): Pixie16WriteSglChanPar failed in mod %d, chan %d, retval=%d",
                        m, n, retval);
                    Pixie_Print_MSG(ErrMSG);

                    // Close all data files
                    for (i = 0; i < nummodules; i++) {
                        fclose(datfil[i]);
                    }
                    return (-4);
                }
            }
        }

        // Wait until the DACs are settled down
        Sleep(15);

        for (m = 0; m < nummodules; m++) {
            retval = Pixie16AcquireADCTrace(m);
            if (retval < 0) {
                sprintf(
                    ErrMSG,
                    "*ERROR* (test_analogfrontend): Pixie16AcquireADCTrace failed in mod %d, retval=%d",
                    m, retval);
                Pixie_Print_MSG(ErrMSG);
                // Close all data files
                for (i = 0; i < nummodules; i++) {
                    fclose(datfil[i]);
                }
                return (-5);
            }

            for (n = 0; n < 16; n++) {
                retval = Pixie16ReadSglChanADCTrace(ADCTrace, MAX_ADC_TRACE_LEN, m, n);
                if (retval < 0) {
                    sprintf(
                        ErrMSG,
                        "*ERROR* (test_analogfrontend): Pixie16ReadSglChanADCTrace failed in mod %d, chan %d, retval=%d",
                        m, n, retval);
                    Pixie_Print_MSG(ErrMSG);
                    // Close all data files
                    for (i = 0; i < nummodules; i++) {
                        fclose(datfil[i]);
                    }
                    return (-6);
                }

                retval = Pixie16IMbufferIO((unsigned int*) &offsetdac, 1,
                                           (unsigned long) (OffsetDAC_Address[m] + n), MOD_READ, m);
                if (retval < 0) {
                    sprintf(
                        ErrMSG,
                        "*ERROR* (test_analogfrontend): Pixie16IMbufferIO failed in mod %d, chan %d, retval=%d",
                        m, n, retval);
                    Pixie_Print_MSG(ErrMSG);
                    // Close all data files
                    for (i = 0; i < nummodules; i++) {
                        fclose(datfil[i]);
                    }
                    return (-7);
                }

                avg = get_average(ADCTrace, MAX_ADC_TRACE_LEN);
                dev = get_deviation(ADCTrace, MAX_ADC_TRACE_LEN, avg);
                fprintf(datfil[m], "%d\t%d\t%ld\t%f\t%f\n", m, n, offsetdac, avg, dev);
            }
        }
        if (fmod((double) k, 1000.0) == 0.0) {
            printf("DAC Steps %d DONE\n", (int) k);
            sprintf(ErrMSG, "DAC Steps %d DONE", (int) k);
            Pixie_Print_MSG(ErrMSG);
        }
    }

    // Check FFT of ADC traces
    for (k = 0; k < nummodules; k++) {
        fprintf(datfil[k], "\nFFT of ADC Traces @ OffsetDAC = 32768\n\n");
    }
    // First set OffsetDAC to 32768
    offsetvolts = 32768.0 / 65536.0 * DAC_VOLTAGE_RANGE - DAC_VOLTAGE_RANGE / 2.0;
    for (m = 0; m < nummodules; m++) {
        for (n = 0; n < 16; n++) {
            retval = Pixie16WriteSglChanPar("VOFFSET", offsetvolts, m, n);
            if (retval < 0) {
                sprintf(
                    ErrMSG,
                    "*ERROR* (test_analogfrontend): Pixie16WriteSglChanPar failed in mod %d, chan %d, retval=%d",
                    m, n, retval);
                Pixie_Print_MSG(ErrMSG);

                // Close all data files
                for (i = 0; i < nummodules; i++) {
                    fclose(datfil[i]);
                }
                return (-8);
            }
        }
    }
    // Wait until the DACs are settled down
    Sleep(5);

    for (m = 0; m < nummodules; m++) {
        retval = Pixie16AcquireADCTrace(m);
        if (retval < 0) {
            sprintf(
                ErrMSG,
                "*ERROR* (test_analogfrontend): Pixie16AcquireADCTrace failed in mod %d, retval=%d",
                m, retval);
            Pixie_Print_MSG(ErrMSG);
            // Close all data files
            for (i = 0; i < nummodules; i++) {
                fclose(datfil[i]);
            }
            return (-9);
        }

        for (n = 0; n < 16; n++) {
            retval = Pixie16ReadSglChanADCTrace(ADCTrace, MAX_ADC_TRACE_LEN, m, n);
            if (retval < 0) {
                sprintf(
                    ErrMSG,
                    "*ERROR* (test_analogfrontend): Pixie16ReadSglChanADCTrace failed in mod %d, chan %d, retval=%d",
                    m, n, retval);
                Pixie_Print_MSG(ErrMSG);
                // Close all data files
                for (i = 0; i < nummodules; i++) {
                    fclose(datfil[i]);
                }
                return (-10);
            }

            // Compute FFT of the acquired ADC trace
            for (i = 0; i < MAX_ADC_TRACE_LEN; i++) {
                fftTrace[i * 2] = (double) ADCTrace[i];
                fftTrace[i * 2 + 1] = 0.0;
            }
            // Compute complex FFT
            retval = Pixie16complexFFT(fftTrace, MAX_ADC_TRACE_LEN);
            // Compute power
            for (i = 0; i < MAX_ADC_TRACE_LEN; i++) {
                power[i] = sqrt(pow(fftTrace[i * 2], 2.0) + pow(fftTrace[i * 2 + 1], 2.0)) /
                           (double) MAX_ADC_TRACE_LEN;
            }
            // clear out the DC offset
            power[0] = 0.0;
            // Write to the output file
            for (i = 0; i < MAX_ADC_TRACE_LEN; i++) {
                fprintf(datfil[m], "%d\t%d\t%f\n", m, n, power[i]);
            }
        }
    }

    // Close all data files
    for (k = 0; k < nummodules; k++) {
        fclose(datfil[k]);
    }

    return (0);
}


double get_average(unsigned short* data, unsigned long numpnts) {

    double avg;
    unsigned long k;

    avg = 0.0;
    for (k = 0; k < numpnts; k++) {
        avg += data[k];
    }
    avg /= numpnts;

    return avg;
}

double get_deviation(unsigned short* data, unsigned long numpnts, double avg) {

    double dev;
    unsigned long k;

    dev = 0.0;
    for (k = 0; k < numpnts; k++) {
        dev += ((double) data[k] - avg) * ((double) data[k] - avg);
    }
    dev /= numpnts;

    return dev;
}
