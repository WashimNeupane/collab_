/* ==================================================================
** Copyright 2019 - BSD Robotics (Microelectronic Systems Pty Ltd)
** ==================================================================
** ==================================================================
**		Authors		:	Theophilus Ah-Hing & Lachlan Stewart.
**		Filename    :	dataHandling.cpp
**		File Type	:	Source file (.cpp)
**		Project     :	DBS Validator
**		Company		:	BSD Robotics (Microelectronic Systems Pty Ltd)
**		Compiler    :	GNU Compiler Collection (GCC)
**		Date/Time   :	09/02/2020
**		Abstract    :	This file contains the definition for all
**						members of the 'DataHandling' class. For
**						function documentation, please refer to the
**						dataHandling.h file.
** ================================================================== */

//-------------------- i - USER INCLUDES ---------------------//
#include "dataHandling.h"

//--------------- 1 - PRIVATE ACCESS FUNCTIONS ---------------//
//------------------ 1.1 - READ TEXT FILE --------------------//
void DataHandling::readFileInternal(const std::string& filepath) {

	bool isConfig = (filepath.find("config") != std::string::npos);
	std::ifstream fileStream(filepath);

	if (fileStream.is_open()) {
		if (isConfig) {
			while (!fileStream.eof()) {
				/*
					Open a textfile containing the systems previous configuration.

					~ To use:
						fileStream >> data; where data can be of any data type.
				*/
			}
		}
		else {

			while (!fileStream.eof()) {
				/*
					Open a textfile containing samples.
				*/
			}
		}
	}
	else {
		std::cout << "[ERROR]: Can not open file!" << std::endl;
	}
	fileStream.close();
}

//---------------- 1.2 - OVERRIDE TEXT FILE ------------------//
void DataHandling::overrideFileInternal(const std::string& filepath) {

	bool isConfig = (filepath.find("config") != std::string::npos);
	std::ofstream fileStream(filepath);

	if (fileStream.is_open()) {
		if (isConfig) {
			/*
				If configurations are changed, override textfile.

				~ To use:
					fileStream << data1 << delimiter << data2...; where data can be of any type and delimiter
					is how to split the text file data; i.e. " " to split with a space.
			*/
		}
		else {
			/*
				Override other files.
			*/
		}
		fileStream.seekp(0, std::ios::end); // Mark the end
	}
	else
		std::cout << "[ERROR]: Can not open file." << std::endl;

	fileStream.close();
}

//----------------- 1.3 - APPEND TEXT FILE -------------------//
void DataHandling::appendFileInternal(const std::string& filepath) {

	bool isConfigChanges = (filepath.find("configChanges") != std::string::npos);
	std::ofstream fileStream(filepath, std::ios_base::app);

	if (fileStream.is_open()) {
		if (isConfigChanges) {
			/*
				Append changes made to config.

				~ To use:
					fileStream << data; where data can be of any type.
			*/
			fileStream << "Hello";
		}
		else {
			/*
				Append to other file.
			*/
		}
	}
	else
		std::cout << "[ERROR]: Can not open file." << std::endl;

	fileStream.close();
}

//------------------- 1.4 - READ BARCODE ---------------------//
void DataHandling::readBarcodeInternal(void) {
	// Can not implement this function
}

//------------------ 1.5 - SEARCH ENGINE ---------------------//
void DataHandling::searchEngineInternal(void) {
	/*
		given text (what to look for) and is given an index
		read word by word until index is found.

		Future Implementation
	*/
}

//--------------- 2 - PUBLIC MEMBER FUNCTIONS ----------------//
//------------------- 2.1 - GET INSTANCE ---------------------//
DataHandling& DataHandling::get(void) {
	static DataHandling dataHndlInstance;
	return dataHndlInstance;
}

//----------- 2.2 - RETURNING INTERNAL FUNCTIONS -------------//
//------- 2.2.1 - RETURN READ FILE INTERNAL FUNCTION ---------//
void DataHandling::readFile(const std::string& fp) {
	return get().readFileInternal(fp);
}

//----- 2.2.2 - RETURN OVERRIDE FILE INTERNAL FUNCTION -------//
void DataHandling::overrideFile(const std::string& fp) {
	return get().overrideFileInternal(fp);
}

//------ 2.2.3 - RETURN APPEND FILE INTERNAL FUNCTION --------//
void DataHandling::appendFile(const std::string& fp) {
	return get().appendFileInternal(fp);
}

//------ 2.2.4 - RETURN READ BARCODE INTERNAL FUNCTION -------//
void DataHandling::readBarcode(void) {
	return get().readBarcodeInternal();
}

//----- 2.2.5 - RETURN SEARCH ENGINE INTERNAL FUNCTION -------//
void DataHandling::searchEngine(void) {
	return get().searchEngineInternal();
}

//---------- 2.3 - DATAHANDLING CLASS DESTRUCTOR -------------//
DataHandling::~DataHandling(void) {
	std::cout << "Database object destroyed!" << std::endl;
}