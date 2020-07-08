/* ==================================================================
** Copyright 2019 - BSD Robotics (Microelectronic Systems Pty Ltd)
** ==================================================================
** ==================================================================
**		Authors		:	Theophilus Ah-Hing & Lachlan Stewart
**		Filename    :	dataHandling.h
**		File Type	:	Header file (.h)
**		Project     :	DBS Validator
**		Company		:	BSD Robotics (Microelectronic Systems Pty Ltd)
**		Compiler    :	GNU Compiler Collection (GCC)
**		Date/Time   :	09/02/2020
**		Abstract    :	This file contains the declaration of the data-
**						Handling class which was constructed as a
**						'Singleton' class.
**		Features	:	~ Reading from a text file.
**						~ Overriding content within a text file.
**						~ Appending to a text file.
**						~ Reading from a barcode scanner which formats
**						  the information.
**						~ Searching for particular information.
** ================================================================== */

//-------------------- i - INITIALIZATION --------------------//
//---------------------- HEADER GUARDS -----------------------//
#ifndef _DATAHANDLING_H_
#define _DATAHANDLING_H_

//---------------------- USER INCLUDES -----------------------//
#include "commonIncludes.h"

//----------------  1 - DATA HANDLING CLASS  -----------------//

class DataHandling
{
//-----------------  1.1 - PRIVATE MEMBERS  ------------------//
private:
	/*
		Marking default constructor as private so that class can
		NOT be instantiated outside of this class.
	*/
	DataHandling(void) = default;
	/*
		Opens up a text file and reads from the text file. At
		the moment, it's set to reading entry by entry, but this
		can be changed by changing the while(!fileStream.eof())
		to while(std::getline(fileStream, string variable)).

		~ Parameters:
			+ filepath: The filepath of the text file.
	*/
	void readFileInternal(const std::string&);
	/*
		Opens a text file and re-writes the entire textfile;
		i.e. All content that was previously in this text file
		will be deleted.

		~ Parameters:
			+ filepath: The filepath of the text file.
	*/
	void overrideFileInternal(const std::string&);
	/*
		Opens a text file in append mode. This is used to append to
		the text file instead of re-writting the entire text
		file.

		~ Parameters:
			+ filepath: The filepath of the text file.
	*/
	void appendFileInternal(const std::string&);
	/*
		This will extract the information from the barcode scanner and
		format it.

		~ Parameters: UNKNOWN
	*/
	void readBarcodeInternal(void);
	/*
		This will search through certain pieces of information and return
		the desired information.

		~ Parameters: UNKNOWN
	*/
	void searchEngineInternal(void);

//------------------  1.2 - PUBLIC MEMBERS  ------------------//
public:
	/*
		Returns an instance of this class.
	*/
	static DataHandling& get(void);
	/*
		Deleting copy constructor as this class is a singleton. Also, it prevents the occurance of memory leakage or
		segmentation faults.
	*/
	DataHandling(const DataHandling&) = delete;
	/*
		The following functions are defined to avoid having to write get() each time.
	*/
	static void readFile(const std::string&);

	static void overrideFile(const std::string&);

	static void appendFile(const std::string&);

	static void readBarcode(void);

	static void searchEngine(void);
	/*
		Destructor for debugging purposes (keep track of objects created).
		It's defined as virtual so that any subclasses that inherit from
		this class will call it's own destructor.
	*/
	~DataHandling(void);
};
#endif