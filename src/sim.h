
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
    std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
  std::cout << "\n";
	return 0;
}

struct Sim
{

  int _turn_count;
  sqlite3 * _db; 

  Sim ()
  {
    _turn_count = 0;

    int rc = sqlite3_open("assets/data/gang.db", &_db);
    if (rc)
    {
      std::cout << "db failed" << std::endl;
    }
    else
    {
      std::cout << "db opened successfully" << std::endl;
    }

    /*
    const char * create_table = "create table crew (Name varchar(30), Job varchar(30))";

    char *zErrMsg = 0;
    rc = sqlite3_exec (db, create_table, callback, 0, &zErrMsg);
    if(rc != SQLITE_OK)
    {
      std::cout << "SQL error: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_free (zErrMsg);
    }

    const char * insert = "insert into crew (Name, Job) values ('Glenn', 'Pilot')";
    rc = sqlite3_exec (db, insert, callback, 0, &zErrMsg);
    if(rc != SQLITE_OK)
    {
      std::cout << "SQL error: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_free (zErrMsg);
    }
    */

    char *zErrMsg = 0;
    const char * query = "select * from crew";
    rc = sqlite3_exec (_db, query, callback, 0, &zErrMsg);
    if(rc != SQLITE_OK)
    {
      std::cout << "SQL error: " << sqlite3_errmsg(_db) << std::endl;
      sqlite3_free (zErrMsg);
    }

  }

  void message(const char * input, char * output)
  {
    std::cout << "input: " << input << std::endl;

    strcat (output, "{");
    if (strcmp ("open_door", input) == 0)
    {
      strcat (output, "prompt: 'The door reveals a huge monster!',");
      strcat (output, "buttons: [");
      strcat (output, "{txt: 'Run away!', key: 'run_away'},");
      strcat (output, "{txt: 'Fight!', key: 'fight'}");
      strcat (output, "]");
    }
    else if (strcmp ("light", input) == 0)
    {
      strcat (output, "prompt: 'The light reveals a dusty painting.',");
      strcat (output, "buttons: [");
      strcat (output, "{txt: 'Inspect it.', key: 'inspect'}");
      strcat (output, "]");
    }
    else
    {
      strcat (output, "prompt: 'There is dark hallway leading to a closed door.',");
      strcat (output, "buttons: [");
      strcat (output, "{txt: 'Open the door', key: 'open_door'},");
      strcat (output, "{txt: 'Turn on light', key: 'light'}");
      strcat (output, "]");
    }
    strcat (output, "}");
  }

  void CleanUp ()
  {
    sqlite3_close (_db); 
  }
};
