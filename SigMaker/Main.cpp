#include "Includes.h"
#include "Misc.h"
#include <string>
#include <sstream> 
#include <iostream>
#include <vector>
 
void ShowOptions( void )
{
    ushort selectionType, keepUnsafeData, logLevel;
    char szBuffer[MAXSTR] = { 0 };

    selectionType = (ushort)Settings.iSelectionType;
    keepUnsafeData = (ushort)Settings.iKeepUnsafeData;
    logLevel = (ushort)Settings.iLogLevel;
    _itoa_s( Settings.iMaxRefCount, szBuffer, MAXSTR, 10 );

    int iResult = ask_form( 
        "Options\n"
        "<#Choose the best sig from total length:R>\n" // 0
        "<#Choose the best sig from the amount of opcodes:R>\n" // 1
        "<#Choose the best sig by the smallest amount of wildcards:R>>\n" // 2
        "<Maximum refs for auto generation:A:20:10::>\n"
        "<#Add only relilable data to sigs(choose if unsure):R>\n" // 0
        "<#Include unsafe data in sigs(may produce better results):R>>\n" // 1
        "<#Disable logging:R>\n" // 0
        "<#Log results:R>\n" // 1
        "<#Log errors and results:R>\n" // 2
        "<#Log errors, results and interim steps of all proceedures:R>>\n" // 3
        , &selectionType, szBuffer, &keepUnsafeData, &logLevel );

    if (iResult > 0)
    {
        Settings.iSelectionType = selectionType;
        Settings.iKeepUnsafeData = keepUnsafeData;
        Settings.iLogLevel = logLevel;
        qsscanf( szBuffer, "%i", &Settings.iMaxRefCount );      
        Settings.Save( "sigmaker.ini" );
    }
}

void GetBatch(void) {
	qstring BatchString;
	ea_t ea_ptr;
	bool Response = ask_text(&BatchString, MAXSTR, "0x7777777", "Enter Offsets");
	if (Response) {
		Settings.iLogLevel = 0;
		std::stringstream BatchStream((const char*)(BatchString.c_str()));
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(BatchStream, line, '\n')) {
			//msg("Read Address: %s\n", line);
			lines.push_back(line);
			char* end;
			unsigned long long result;
			errno = 0;
			result = strtoull(line.c_str(), &end, 16);
			if (result == 0 && end == line.c_str()) {
				/* str was not a number */
			}
			else if (result == ULLONG_MAX && errno) {
				/* the value of str does not fit in unsigned long long */
			}
			else if (*end) {
				/* str began with a number but has junk left over at the end */
			}
			else {
				GenerateBatch(result); //THIS WORKS
			}
		}
	}
}

/*
void GetBatch(void) {
	qstring BatchString;
	size_t vcount;
	bool Response = ask_text(&BatchString, MAXSTR, "0x7777777", "Enter Offsets");
	if (Response) {
		std::stringstream BatchStream((const char*)(BatchString.c_str()));
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(BatchStream, line, '\n')) {
			msg("Read Address: %s\n", line);
			lines.push_back(line);
		}
		vcount = lines.sizeof();
		for (int i = 0; i < vcount; i++) {
			line = lines[i];
			ea_t ea_ptr = reinterpret_cast<ea_t>(line);
			GenerateBatch(ea_ptr);
			}

	}
	//GenerateBatch(0x141610EA0); //THIS WORKS
}
*/

/*
void GetBatch(void) {
	qstring BatchString;
	bool Response = ask_text(&BatchString, MAXSTR, "0x7777777", "Enter Offsets");
	if (Response) {
		qvector<qstring> lines;
		FILE* oFile = qfopen("C:\\Users\\lelan\\OneDrive\\desktop\\text.txt", "wb");
		qfwrite(oFile, &BatchString, sizeof(BatchString));
		qfclose(oFile);
		FILE* iFile = qfopen("C:\\Users\\lelan\\OneDrive\\desktop\\text.txt", "rb");
		for (qstring line; qgetline(&line, iFile);) {
		msg("Read Address: \n0x%X\n", line);
		lines.push_back(line);
		qfclose(iFile);
		}
	}
	//GenerateBatch(0x141610EA0); //THIS WORKS
}
*/

//for (qstring line; qgetline(&line, &BatchString); /**/) {
//msg("Read Address: \n0x%X\n", line);
//lines.push_back(line);

/*
void GetBatch(void) {
	qstring BatchString;
	qBatchVector BatchVector;
	int vcount = 0;
	bool Response = ask_text(&BatchString, MAXSTR, "0x7777777", "Enter Offsets");
	if (Response) {
		msg("Input into box: %ls\n",BatchString);
		tmpfile
		BatchVector = StringToLines(BatchString, &vcount);
		for (int i = 1; i <= vcount; i++) {
			qstring TempString = BatchVector[i];
			const char* cstr = TempString.c_str();
			msg("Read Address: %s\n", cstr);
			//if (str2ea(&ea_ptr, cstr, 0)) GenerateBatch(ea_ptr);
		}
	}
	//GenerateBatch(0x141610EA0); //THIS WORKS
}
*/

bool idaapi run( size_t /*arg*/ )
{
    int iAction = 0;

    int iResult = ask_form(
        "What do you want to do?\n"
        "<#Auto create ida pattern:R>\n" // 0
        "<#Auto create code pattern:R>\n" // 1
		"<#Batch mode:R>\n" // 2
        //"<#Auto create crc32 pattern:R>\n" // 2
        "<#Create ida pattern from selection:R>\n" // 3
        "<#Create code pattern from selection:R>\n" // 4
        "<#Create crc32 pattern from selection:R>\n" // 5
        "<#Test ida pattern:R>\n" // 6
        "<#Test code pattern:R>\n" // 7
        "<#Convert a sig:R>\n" // 8
        "<#Configure the plugin:R>>\n" // 9
		//"<#Batch mode:R>\n" // 10
        , &iAction );

    if (iResult > 0)
    {
        switch (iAction)
        {
        case 0:
            GenerateSig( SIG_IDA );
            break;
        case 1:
            GenerateSig( SIG_CODE );
            break;
        case 2:
			GetBatch();
            //GenerateSig( SIG_CRC );
            break;
        case 3: 
            CreateSig( SIG_IDA );
            break;
        case 4: 
            CreateSig( SIG_CODE );
            break;
        case 5: 
            CreateSig( SIG_CRC );
            break;
        case 6: 
            ShowSearchWindow( );
            break;
        case 7: 
            ShowSearchDialog( );
            break;
        case 8: 
            ShowSigConverter( );
            break;
        case 9: 
            ShowOptions( );
            break;
	//	case 10:
	//		GetBatch();
	//		break;
        }
    }

    return true;
}

int idaapi init( void )
{
    Settings.Init( );
    Settings.Load( "sigmaker.ini" );

    return PLUGIN_OK;
}

plugin_t PLUGIN = {
    IDP_INTERFACE_VERSION,
    PLUGIN_KEEP,
    init,
    NULL,
    run,
    "Creates a unique signature",
    "SigMaker plugin",
    "SigMaker",
    "Ctrl-Alt-S"
};