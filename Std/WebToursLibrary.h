char * RandomDate()
{
int day;
int month;
int year;
char *const slash = "/";
char myDate[20];
	 
	day = rand() % 27 + 1;
	month = rand() % 11 + 1;
	year = 2015;
	
	//lr_output_message("%d%s%d%s%d", day, slash, month, slash, year);
	
	sprintf(myDate, "%d/%d/%d", day, month, year);
	lr_output_message("Date:[%s]", myDate);
	
    return myDate;
}