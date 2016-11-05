#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "def21160.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <signal.h>
#include <cstring>
#include <string>

/////////////timing stuff//////////////

#include <sys/time.h>

//////////////////////////////////////

using namespace std;

int main(int argc, char *argv[])
{
	unsigned short NumModules;
	unsigned short *PXISlotMap;
	char ComFPGAConfigFile[80];
	char SPFPGAConfigFile[80];
	char TrigFPGAConfigFile[80];
	char DSPCodeFile[80];
	char DSPParFile[80];
	char DSPVarFile[80];
	char ErrMSG[256];
	int retval=0;


	////////read cfg file (cfgPixie16.txt)///////////////////////////////////
	const char config[20]="cfgPixie16.txt";

	ifstream input;
	char *temp = new char[80];
	input.open (config, ios::in);

	if (input.fail ())
	{
		cout << "can't open the config file ! " << config << endl << flush;
		return false;
	}

	input >> NumModules;
	cout << "\n\n" << NumModules << " modules, in slots:";
	input.getline (temp, 80);
	PXISlotMap = new unsigned short[NumModules+1];
	for (int i = 0; i < NumModules; i++)
	{
		input >> PXISlotMap[i];
		input.getline (temp, 80);
		cout << PXISlotMap[i] << " ";
	}

	//==== This code is necessary if modules are installed in two crates ====//
	//input >> PXISlotMap[NumModules];
	//input.getline (temp, 80);
	//cout << PXISlotMap[NumModules] << " ";

	cout << endl << "Firmware files: \n";
	input >> ComFPGAConfigFile;
	input.getline (temp, 80);
	cout << "ComFPGAConfigFile:  " << ComFPGAConfigFile << endl;
	input >> SPFPGAConfigFile;
	input.getline (temp, 80);
	cout << "SPFPGAConfigFile:   " << SPFPGAConfigFile << endl;
	input >> TrigFPGAConfigFile;
	input.getline (temp, 80);
	cout << "TrigFPGAConfigFile: " << TrigFPGAConfigFile << endl;
	input >> DSPCodeFile;
	input.getline (temp, 80);
	cout << "DSPCodeFile:        " << DSPCodeFile << endl;
	input >> DSPParFile;
	input.getline (temp, 80);
	cout << "DSPParFile:         " << DSPParFile << endl;
	input >> DSPVarFile;
	input.getline (temp, 80);
	cout << "DSPVarFile:         " << DSPVarFile << endl;
	input.close();
	input.clear();

	////////////////////////////////////////////////////////////////////
	cout<<"-----------------------------------------\n";
	cout<<"Booting....\n";

	retval = Pixie16InitSystem (NumModules, PXISlotMap, 0);
	if (retval < 0)
	{
		sprintf (ErrMSG, "*ERROR* Pixie16InitSystem failed, retval = %d",
			retval);
		Pixie_Print_MSG (ErrMSG);
		return -1;
	}
	else
	{
		cout<<"Init OK "<<retval<<endl;
	}

	retval = Pixie16BootModule (ComFPGAConfigFile, SPFPGAConfigFile,
		TrigFPGAConfigFile, DSPCodeFile, DSPParFile,
		DSPVarFile, NumModules, 0x7F);
	if (retval < 0)
	{
		sprintf (ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
		Pixie_Print_MSG (ErrMSG);
		return -2;
	}
	
	sprintf (ErrMSG, "Pixie16BootModule succeeded, retval = %d", retval);
	Pixie_Print_MSG (ErrMSG);
	cout<<"Boot OK "<<retval<<endl;

	return 1;
}
