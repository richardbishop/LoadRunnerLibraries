/*========================================================================================
lib1.h Function Summary
=======================
Data management routines
------------------------
Interface	char* HEX_decode(char* HEX_orig)
Author		Dave Brown, Acuma
Function	Returns string with %hex characters replaced

Interface	char* genderchar(char* strIN)
Author		Dave Brown, Acuma
Function	Returns null-terminated gender or logs error

Interface	char* dateto2F(char* strIN)
Author		Dave Brown, Acuma
Function	Returns date, replacing '/' with %2F in valid date for URL parameters

Interface	char* spaceto20 (char* strIN)
Author		Dave Brown, Acuma
Function	Replaces spaces with %20 for URL parameters

Interface	void CaptureAddress()
Author		Mark Dowd, Acuma
Function	web_create_html_param for addresses, externalised for script clarity


Interface	void DecodeAddress ()
Author		Mark Dowd, Acuma, Dave Brown, Acuma
Function	Decodes addresses. Externalised to eliminate "syntax error" with ">"

Interface	int RandInt( int iMIN, int iMAX )
Author		Mark Dowd, Acuma and others
Function	Returns a random integer betwee iMIN and iMAX inclusive

Interface	void SetSeed()
Author		Mark Dowd, Acuma and others
Function	Initialises the random seed using the back end of the {Timestamptime}
		variable. Must be called once per VUser.

Interface	char *strrev(char *string)
Author		Mark Dowd, Acuma and others
Function	Reverses the order of characters in a string.
		Note that parameter is passed as a pointer, and so the result
		need not be assigned.

Logging and auditing functions
------------------------------
Interface	void CaptureSession()
Author		Dave Brown, Acuma
Function	Sets the delimiters for capturing NMSPSessID and ServerKey

Interface	void CaptureApplicantIDs()
Author		Dave Brown, Acuma
Function	Sets the delimiters for capturing ApplicantID1, ApplicantID2,
		ApplicantName1 and ApplicantName2

Interface	void CaptureScreen()
		void VerifyScreen (char* Expected, char* TransName)
Author		Mark Dowd, Acuma
Function	CaptureScreen monitors the next server interaction (NMSP or CF)
		for identity information. VerifyScreen compares the data with Expected
		and logs a loadrunner error, noting TransName (Transaction Name) prior
		to ending the iteration by calling an invalid URL

	14-Mar	Modified by Mark Dowd to reset all variables before capture and after verify.
		This is to get around a problem of mis-reporting the returned ID where none
		is returned.

Interface	void AppendLog (char* Action, char* ASPName, char* TransName)
Author		Mark Dowd, Acuma
Function	Appends transaction details into a local comma-delimited file
		Action should be "Send" or "Receive"
		** Requires the following parameters to be defined **
		{GroupName}	Group Name - %s
		{VuserID}	VUser ID - %03s
		{Timestampdate}	Date/Time - %Y-%m-%d
		{Timestamptime}	Date/Time - %H:%M:%S.000
		{UserName}	File - NMSP UserID (also contains password)
		{Iteration}	Iteration number - %d

Interface	void LogDownLoadSize (char* ASPName)
Author		Mark Dowd, Acuma
Function	Appends bytes returned from a web request into a local comma-delimited file
		** See AppendLog for list of required parameters **

Interface	void CaptureHTML()
Author		James Beverley, Accenture
Function	Captures upto 5 sets of HTML returned in the succeeding data returned from the server.


Interface	void LogHTMLFileName (char* FileName)
Author		James Beverley, Accenture
Function	Logs FileName to a text file with suffix .filenames

Interface  	void WriteHTMLFile (char* Req, char* Txn, char* HTML)
Author		James Beverley, Accenture
Function	Creates file containing HTML passed in

Interface	void LogHTML (char* RequestName, char* Transaction)
Author		James Beverley, Accenture
Function	Logs all HTML captured by CaptureHTML() to a text file by calling WriteHTMLFile


Interface	void nmsp_init(char* ScriptName)
Author		Mark Dowd, Acuma
Function	Externalises most of the vuser_init.c activity to ensure consistency
		across all scripts.

========================================================================================*/
char* HEX_decode(char* HEX_orig) {
	char* HEX_new;
	int i;
	char code;
 
	if (!HEX_orig) /* don't try to deref NULL ptrs */
		return NULL;

	/* allocate temporary memory */ 
	if( ( HEX_new = (char*)malloc(strlen(HEX_orig)*sizeof(char)) ) == NULL ) {
		lr_output_message("HEX_decode():  malloc():  Couldn't allocate memory.");
		return NULL;
	}

	/* walk the source string */
	for (i = 0; *HEX_orig; HEX_orig++, i++) {
		if ( *HEX_orig == '\\' ) {
		/* hex-encoded (ASCII) chars restored */
			if (sscanf(HEX_orig+1, "x%X", &code) != 1)
				code = '?'; /* failed to scan, make it something */
			HEX_new[i] = code;
			HEX_orig +=3; /* hop over the scan chars */
		}
		else
			HEX_new[i] = *HEX_orig;
	}

	HEX_new[i] = '\0'; /* make sure new string is terminated */

	/* release temporary memory */
	free(HEX_new);

	return HEX_new;
}
char buffer[125];

char* genderchar(char* strIN)
{ 
	if (!strIN) {
		lr_log_message("Error in Gender: %s", strIN);
		return NULL;
	}

	buffer[0] = *strIN;
	buffer[1] = '\0';

	return (char*)buffer;
}

char* dateto2F(char* strIN)
{ 
	if (!strIN) {
		lr_log_message("DOB is blank. DOB set to 01/01/1965");
		buffer[0] = '0';
		buffer[1] = '1';
		buffer[2] = '%';
		buffer[3] = '2';
		buffer[4] = 'F';
		buffer[5] = '0';
		buffer[6] = '1';
		buffer[7] = '%';
		buffer[8] = '2';
		buffer[9] = 'F';
		buffer[10] = '1';
		buffer[11] = '9';
		buffer[12] = '6';
		buffer[13] = '5';
		buffer[14] = '\0';
		return (char*)buffer;
	}

	if (strlen(strIN) != 10) {
		lr_log_message("Error in Date: %s. DOB set to 01/01/1965", strIN);
		buffer[0] = '0';
		buffer[1] = '1';
		buffer[2] = '%';
		buffer[3] = '2';
		buffer[4] = 'F';
		buffer[5] = '0';
		buffer[6] = '1';
		buffer[7] = '%';
		buffer[8] = '2';
		buffer[9] = 'F';
		buffer[10] = '1';
		buffer[11] = '9';
		buffer[12] = '6';
		buffer[13] = '5';
		buffer[14] = '\0';
		return (char*)buffer; 
	}

	buffer[0] = *strIN;strIN++;
	buffer[1] = *strIN;strIN+=2;
	buffer[2] = '%';
	buffer[3] = '2';
	buffer[4] = 'F';
	buffer[5] = *strIN;strIN++;
	buffer[6] = *strIN;strIN+=2;
	buffer[7] = '%';
	buffer[8] = '2';
	buffer[9] = 'F';
	buffer[10] = *strIN;strIN++;
	buffer[11] = *strIN;strIN++;
	buffer[12] = *strIN;strIN++;
	buffer[13] = *strIN;
	buffer[14] = '\0';

	return (char*)buffer;
}

char* spaceto20 (char* strIN)
{
	int strleng;
	int pos = 0;
	int i;

	if (!strIN) /* don't try to deref NULL ptrs */
		return NULL;

	strleng = strlen(strIN);

	for (i = 0; *strIN; strIN++, i++) {
		if (*strIN == ' ') {
			buffer[pos] = '%';pos++;
			buffer[pos] = '2';pos++;
			buffer[pos] = '0';pos++;
		} else {
			buffer[pos] = *strIN;pos++;
		}
	}

	buffer[pos] = '\0';

	return (char*)buffer;
}

void CaptureSession()
{
	web_create_html_param("NMSPSessID",
		"NMSPSessID=",
		"&");
	
	web_create_html_param("ServerKey",
		"ServerKey=",
		"&");
}

void CaptureApplicantIDs()
{
/*	web_create_html_param("ApplicantID1",
		"txthidAddApp1InvolvedPartyXX' VALUE=\"",
		"\"");

	web_create_html_param("ApplicantID2",
		"txthidAddApp2InvolvedPartyXX' VALUE=\"",
		"\"");
*/
	web_create_html_param_ex("ApplicantID1",
		"<OPTION value = \"",
		"\">",
		"1");

	web_create_html_param_ex("ApplicantID2",
		"<OPTION value = \"",
		"\">",
		"2");

	web_create_html_param("ApplicantName1",
		"txthidMainApplicantNameXX' VALUE=\"",
		"\"");

	web_create_html_param("ApplicantName2",
		"txthidSecondApplicantNameXX' VALUE=\"",
		"\"");
}

void CaptureAddress()
{
	web_create_html_param_ex("addr_line1",
		"AddressLine1:'",
		"'",
		"2");
	web_create_html_param_ex("addr_line2",
		"AddressLine2:'",
		"'",
		"2");
	web_create_html_param_ex("addr_line3",
		"AddressLine3:'",
		"'",
		"2");
	web_create_html_param_ex("addr_line4",
		"AddressLine4:'",
		"'",
		"2");
}

void DecodeAddress ()
{
	char address[255];

	if (strlen(lr_eval_string("{addr_line1}")) > 3)
		lr_save_string(HEX_decode(lr_eval_string("{addr_line1}")),"addr_line1");
	if (strlen(lr_eval_string("{addr_line2}")) > 3)
		lr_save_string(HEX_decode(lr_eval_string("{addr_line2}")),"addr_line2");
	if (strlen(lr_eval_string("{addr_line3}")) > 3)
		lr_save_string(HEX_decode(lr_eval_string("{addr_line3}")),"addr_line3");
	if (strlen(lr_eval_string("{addr_line4}")) > 3)
		lr_save_string(HEX_decode(lr_eval_string("{addr_line4}")),"addr_line4");

	// Build up complete address using non null address lines

	if (strlen(lr_eval_string("{addr_line4}")) > 0)
		sprintf(address,"%s\r\n%s\r\n%s\r\n%s",
			lr_eval_string("{addr_line1}"),
			lr_eval_string("{addr_line2}"),
			lr_eval_string("{addr_line3}"),
			lr_eval_string("{addr_line4}"));
	else
		if (strlen(lr_eval_string("{addr_line3}")) > 0)
			sprintf(address,"%s\r\n%s\r\n%s",
				lr_eval_string("{addr_line1}"),
				lr_eval_string("{addr_line2}"),
				lr_eval_string("{addr_line3}"));
		else
			if (strlen(lr_eval_string("{addr_line2}")) > 0)
				sprintf(address,"%s\r\n%s",
					lr_eval_string("{addr_line1}"),
					lr_eval_string("{addr_line2}"));
			else
				sprintf(address,"%s",
					lr_eval_string("{addr_line1}"));

	lr_save_string(address,"addr_lines");
}

void CaptureScreen()
{
	// Reset parameters (this is NOT done by LoadRunner)
	lr_save_string("","NMSPScreenID");
	lr_save_string("","CFScreenID1");
	lr_save_string("","CFScreenID2");

	//Add parameter to capture time out message
	lr_save_string("","TimeOutScreenID");
	lr_save_string("","NMSPScreenID1");
	lr_save_string("","NMSPScreenID2");

	web_create_html_param("TimeOutScreenID",
		"<TD>10004</TD></TR><TR><TD>Description</TD><TD>",
		"</TD></TR><TR><TD>Source</TD><TD>");

	web_create_html_param("NMSPScreenID1",
		"<title>",
		"</title>");

	web_create_html_param("NMSPScreenID2",
		"<SCRIPT ",
		"location.href");

	web_create_html_param("NMSPScreenID",
		"<!-- :SCREENIDENT:",
		": -->");
	web_create_html_param("CFScreenID1",
		"<div class=\"screen\" id=\"",
		"\">");
	web_create_html_param("CFScreenID2",
		"<DIV class=\"screen\" id=\"",
		"\">");
}

void VerifyScreen (char* Expected, char* TransName)
{
	char Actual[2000];

	// Capture the only screen value returned
	strcpy(Actual, lr_eval_string("{NMSPScreenID}"));

	if(Actual[0] == 0x00)
	{
		strcpy(Actual, lr_eval_string("{CFScreenID1}"));
		if(Actual[0] == 0x00)
			strcpy(Actual, lr_eval_string("{CFScreenID2}"));
			if(Actual[0] == 0x00)
				strcpy(Actual, lr_eval_string("{TimeOutScreenID}"));
					if(Actual[0] == 0x00)
						strcpy(Actual, lr_eval_string("{NMSPScreenID1}"));
											if(Actual[0] == 0x00)
												strcpy(Actual, lr_eval_string("{NMSPScreenID2}"));
	}


	// Compare with the expected value
	if(strcmp(Expected,Actual))
	{
		lr_error_message("User,%s,row,%s,expected,%s,but received,%s,for transaction,%s",
			lr_eval_string("{CurrentUser}"),
			lr_eval_string("{DataRow}"),
			Expected,
			Actual,
			TransName);
		web_url("Terminate Iteration","URL=http://",LAST);
	}

	// Reset parameters (this is NOT done by LoadRunner)
	lr_save_string("","NMSPScreenID");
	lr_save_string("","CFScreenID1");
	lr_save_string("","CFScreenID2");
	lr_save_string("","TimeOutScreenID");
	lr_save_string("","NMSPScreenID1");
	lr_save_string("","NMSPScreenID2");

}

void AppendLog (char* Action, char* ASPName, char* TransName)
{
	long outstream;

	char OutputFile[23];

	sprintf(OutputFile,"C:\\%s-%s-%s-%s.loadrunner",
		lr_eval_string("{ScriptName}"),
		lr_eval_string("{HostName}"),
		lr_eval_string("{GroupName}"),
		lr_eval_string("{VuserID}"));

	outstream = fopen (OutputFile,"a+");

	if (outstream == NULL)
		lr_output_message ("Could not open output file %s",OutputFile);
	else {
		fprintf (outstream, "%s,%s,%s,%s,%s,%s,%s,%s\n",
			lr_eval_string ("{Timestampdate}"),
			lr_eval_string ("{Timestamptime}"),
			lr_eval_string ("{CurrentUser}"),
			ASPName,
			Action,
			TransName,
			lr_eval_string ("{Iteration}"),
			lr_eval_string ("{PerformanceInfo}"));
		fclose (outstream);
	}

	// Blank out the PerformanceInfo parameter
	lr_save_string("", "PerformanceInfo");
}

char* spacetoPlus (char* strIN)
{
	int strleng;
	int pos = 0;
	int i;

	if (!strIN) /* don't try to deref NULL ptrs */
		return NULL;

	strleng = strlen(strIN);

	for (i = 0; *strIN; strIN++, i++) {
		if (*strIN == ' ') {
			buffer[pos] = '+';pos++;
		} else {
			buffer[pos] = *strIN;pos++;
		}
	}

	buffer[pos] = '\0';

	return (char*)buffer;
}

int RandInt( int iMIN, int iMAX ) 
{ 
	// If both are the same, return iMIN
	if( iMIN == iMAX ) 
	return iMIN; 

	// Ensure that iMAX > iMIN
	else if( iMAX < iMIN ) 
	{ 
		int temp = iMIN; 
		iMIN = iMAX; 
		iMAX = temp; 
	} 

	// Return a random integer between iMIN and iMAX inclusive
	// 32768 is the observed maximum value of rand()
	return (iMIN + (int) ((iMAX-iMIN+1) * (((float)rand())/(float)32768))); 
}

void LogDownLoadSize (char* ASPName)
{
	long outstream;
	long DownLoadSize;

	char OutputFile[23];

	sprintf(OutputFile,"C:\\%s-%s-%s-%s.returnedbytes",
		lr_eval_string("{ScriptName}"),
		lr_eval_string("{HostName}"),
		lr_eval_string("{GroupName}"),
		lr_eval_string("{VuserID}"));

	outstream = fopen (OutputFile,"a+");

	if (outstream == NULL)
		lr_output_message ("Could not open output file %s",OutputFile);
	else {

		DownLoadSize=web_get_int_property(HTTP_INFO_DOWNLOAD_SIZE);

		fprintf (outstream, "%s,%d\n",
			ASPName,
			DownLoadSize);
		fclose (outstream);
	}
}

int w3c_counter = 1;

void CaptureHTML()
{
	// Reset parameters (this is NOT done by LoadRunner)
	lr_save_string("","HTML01");
	lr_save_string("","HTML02");
	lr_save_string("","HTML03");
	lr_save_string("","HTML04");
	lr_save_string("","HTML05");

	web_create_html_param_ex("HTML01","<html>","</html>","1");
	web_create_html_param_ex("HTML02","<html>","</html>","2");
	web_create_html_param_ex("HTML03","<html>","</html>","3");
	web_create_html_param_ex("HTML04","<html>","</html>","4");
	web_create_html_param_ex("HTML05","<html>","</html>","5");
}

void LogHTMLFileName (char* FileName)
{
	long outstream;

	char OutputFile[100];

	sprintf(OutputFile,"C:\\%s.filenames",
		lr_eval_string("{ScriptName}"));

	outstream = fopen (OutputFile,"a+");

	if (outstream == NULL)
		lr_output_message ("Could not open output file %s",OutputFile);
	else {
		fprintf (outstream, "%s\n",FileName);			
		fclose (outstream);
	}
}

void WriteHTMLFile (char* Req, char* Txn, char* HTML)
{
	long outstream;

	char OutputFile[100];

	sprintf(OutputFile,"C:\\%s-%s-%s-%02d.html",
		lr_eval_string("{ScriptName}"),
		Txn,
		Req,
		w3c_counter);

	outstream = fopen (OutputFile,"a+");

	if (outstream == NULL)
		lr_output_message ("Could not open output file %s",OutputFile);
	else {
		fprintf (outstream, "%s%s%s\n","<html>",HTML,"</html>"),
		fclose (outstream);
		LogHTMLFileName (OutputFile);
	}
}

void LogHTML (char* RequestName, char* Transaction)
{
	w3c_counter = 1;

	if (lr_eval_string("{HTML01}")[0] != 0x00) {
		WriteHTMLFile(RequestName, Transaction, (char*) lr_eval_string("{HTML01}"));
		w3c_counter++;
	}
	if ((char*)lr_eval_string("{HTML02}")[0] != 0x00) {
		WriteHTMLFile(RequestName, Transaction, (char*)lr_eval_string("{HTML02}"));
		w3c_counter++;
	}
	if ((char*)lr_eval_string("{HTML03}")[0] != 0x00) {
		WriteHTMLFile(RequestName, Transaction, (char*) lr_eval_string("{HTML03}"));
		w3c_counter++;
	}
	if ((char*)lr_eval_string("{HTML04}")[0] != 0x00) {
		WriteHTMLFile(RequestName, Transaction, (char*) lr_eval_string("{HTML04}"));
		w3c_counter++;
	}
	if ((char*)lr_eval_string("{HTML05}")[0] != 0x00) {
		WriteHTMLFile(RequestName, Transaction, (char*) lr_eval_string("{HTML05}"));
		w3c_counter++;
	}
}

char *strrev(char *string)
{
  char temp;
  size_t slen;
  char *p1, *p2;
  slen = strlen(string);
  if(slen > 1)
  {
    p1 = string;
    p2 = string + (slen - 1);
    while (p1 < p2)
    {
      temp = *p1;
      *p1 = *p2;
      *p2 = temp;
      p1++;
      p2--;
    }
  }
  return(string);
}

void SetSeed()
{
	// Declare local variables
	int seed;
	char* time;
	char chopped_time[128];

	// Get the current millisecond time (format Hh;Mm;Ss.000)
	time = lr_eval_string("{Timestamptime}");

	// Reverse it in-situ (format 000.sS:mM:hH)
	strrev(time);

	// Extract the first three characters
	strncpy((char*)chopped_time,time,3);

	// Convert to integer
	seed = atoi(chopped_time);

	// Feed this number into the random number seed
	srand(seed);
}

void nmsp_init(char* Script)
{
	web_cleanup_cookies();
	lr_save_string(lr_eval_string ("{UserName}"),"CurrentUser");
	lr_save_string(Script,"ScriptName");

	// Simulate local cache
	//=====================
	// Derive server date & time
	web_create_html_param("If-Modified-Since","Date:","\r\n");

	web_url("blank.htm",
		"URL=http://hww.mortgagesales.hx-online.hxgroup.com/blank.htm",
		"TargetFrame=",
		"Resource=0",
		"RecContentType=text/html",
		"SupportFrames=0",
		"Snapshot=t11.inf",
		LAST);

	// Set auto-header (no "old" furniture)
	web_add_auto_header("If-Modified-Since",lr_eval_string("{If-Modified-Since}"));

	// Initialise parameters
	lr_save_string("", "PerformanceInfo");
	lr_save_string("", "NMSPScreenID");
	lr_save_string("", "CFScreenID1");
	lr_save_string("", "CFScreenID2");
	lr_save_string("", "TimeOutScreenID");
	lr_save_string("","NMSPScreenID1");
	lr_save_string("","NMSPScreenID2");

	// Seed random number generator
	SetSeed();
}


CaptureAgmntNo()
{
	lr_save_string("","AgmntNo");

	web_create_html_param("AgmntNo","&AgmntNo=","&OutputChannel");
}

RandomDate(char *string)
{
int day;
int month;
int year;
char *const slash = "/";
char myDate[10];
	 
	day = rand() % 27 + 1;
	//lr_output_message("%d", day);
	month = rand() % 11 + 1;
	//lr_output_message("%d", month);
	year = 2014;
	//lr_output_message("%d", year);
	
	//lr_output_message("%d%s%d%s%d", day, slash, month, slash, year);
	
	sprintf(myDate, "%d/%d/%d", day, month, year);
	lr_output_message("Date:[%s]", myDate);
	
    return myDate;
}