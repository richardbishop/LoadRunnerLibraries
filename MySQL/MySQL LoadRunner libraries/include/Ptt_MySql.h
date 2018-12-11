#include <mysql.h>
#include <mysql_com.h>
#include <mysql_time.h>
#include <mysql_version.h>
#include <typelib.h>
#include <my_list.h>
#include <my_alloc.h>

struct field
{
	char cell[100];
};
struct field row[50][50];

char single_cell[100];
int row_count, col_count;

char *cols[50];
char *vals[50];

MYSQL *lr_mysql_connect(char *server, char *user, char *password, char *database, int port)
{
	MYSQL *Mconn;
	char sql_query[256], sql_sub_query[50];

	int i=0, res=0, conn_fail=0, conn_iter=0;
	char response[50];// to go

    /*------------------------------------------------------------------------*/
	/* Initialise MySQL */
	if(!(Mconn = mysql_init(NULL)))
    {
		lr_message("Error -1: Cannot initialize MySQL - %s", mysql_error(Mconn));
		//return -1;
    }
    /*------------------------------------------------------------------------*/

	do
	{

		/* Connect to database */
		if (!mysql_real_connect(Mconn, server, user, password, database, port, NULL, 0))
		{
			conn_fail = -2;
			conn_iter++;
			sleep(100);
		}
		else
			conn_fail = 0;
	}
	while(conn_fail < 0 && conn_iter<10);

	if (conn_fail < 0)
	{
		lr_message("Error -2: %s", mysql_error(Mconn));
		//mysql_close(Mconn);
		//return -2;
	}
	else
	{
		//lr_message("MySql - Good Connection");
		//mysql_close(Mconn);
	}
		return Mconn;
}


int lr_mysql_query(MYSQL *Mconn, char *query)
{

	MYSQL_RES *Mres;
	MYSQL_ROW Mrow;
	unsigned int NumFields, NumRows, iRow, iCol;

	if (mysql_query(Mconn, query))
	{
	  lr_message("Error -3: row not found '%s' - %s", query, mysql_error(Mconn));
	  mysql_close(Mconn);
	  return -3;
	}

	Mres = mysql_store_result(Mconn);
	iCol = 0;
    if (Mres)  // there are rows
    {
        NumFields = mysql_num_fields(Mres);
        //lr_message("Cols = %d", NumFields);
        // retrieve rows
        while ((Mrow = mysql_fetch_row(Mres)))
		{
		   unsigned long *lengths;
		   lengths = mysql_fetch_lengths(Mres);
		   for(iRow = 0; iRow < NumFields; iRow++)
		   {
		       //lr_message("[%.*s] ", (int) lengths[i], Mrow[i] ? Mrow[i] : "NULL");
		       sprintf(row[iRow][iCol].cell,"%.*s",  (int) lengths[iRow], Mrow[iRow] ? Mrow[iRow] : "NULL");
		       //lr_message("row[%d][%d].cell = [%s]", iRow, iCol, row[iRow][iCol].cell);
		   }
		   iCol++;
		}
		// rows retrieved...........
        mysql_free_result(Mres);
		row_count = iCol;
		col_count = iRow;
    }
    else  // mysql_store_result() returned nothing; should it have?
    {
        if (mysql_errno(Mconn))
        {
           lr_error_message("Error: %s\n", mysql_error(Mconn));
        }
        else if (mysql_field_count(Mconn) == 0)
        {
            // query does not return data
            // (it was not a SELECT)
            NumRows = mysql_affected_rows(Mconn);
            //lr_message("NumRows Affected = %d", NumRows);
        }
    }

}


int lr_mysql_disconnect(MYSQL *Mconn)
{
	mysql_close(Mconn);
	return 1;
}
