
static int callback (void *NotUsed, int argc, char **argv, char **azColName)
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
    
  sqlite3_stmt * _all_crew;

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

    /*
    char *zErrMsg = 0;
    rc = sqlite3_exec (_db, query, callback, 0, &zErrMsg);
    if(rc != SQLITE_OK)
    {
      std::cout << "SQL error: " << sqlite3_errmsg(_db) << std::endl;
      sqlite3_free (zErrMsg);
    }
    */

    const char * query = "select * from crew";
    rc = sqlite3_prepare_v2(_db, query, -1, &_all_crew, 0);
    /*
    // if you need to inservert vars into the query 
    if (rc == SQLITE_OK)
    { sqlite3_bind_int (res, 1, 3);
    }
    else
    { fprintf (stderr, "Failed to execute statement: %s\n", sqlite3_errmsg (_db));
    }
    */ 
  }

  void message(const char * input, char * output)
  {
    std::cout << "input: " << input << std::endl;
    rapidjson::Document doc;
    doc . SetObject ();
    rapidjson::Document::AllocatorType &alloc = doc . GetAllocator ();

    if (strcmp ("show_crew", input) == 0)
    {
      char data[100];
      strcpy (data, "Crew: ");
      int step;
      do
      { 
        step = sqlite3_step (_all_crew);
        if (step == SQLITE_ROW)
        { printf("%s: ", sqlite3_column_text (_all_crew, 0));
          printf("%s\n", sqlite3_column_text (_all_crew, 1));
          strcat (data, const_cast<char *>(reinterpret_cast<const char*> (sqlite3_column_text (_all_crew, 0)))); 
        }
      }
      while (step != SQLITE_DONE);
      sqlite3_reset (_all_crew);

      rapidjson::Document btn0;
      btn0 . SetObject ();
      btn0 . AddMember ("txt", "Back", alloc) . AddMember ("key", "back", alloc);
      
      rapidjson::Value buttons (rapidjson::kArrayType);
      buttons . PushBack (btn0, alloc); 
     
      std::string s(data); 
      doc . AddMember ("prompt", rapidjson::StringRef (data), alloc);
      doc . AddMember ("buttons", buttons, alloc);
    }
    else if (strcmp ("add_crew", input) == 0)
    {
      sqlite3_stmt * insert_crew;
      const char * s = "insert into crew (Name, current_job, id) values ('Glenn', 'Pilot', 4)";
      sqlite3_prepare_v2(_db, s, -1, &insert_crew, 0);
      sqlite3_step (insert_crew);
      sqlite3_finalize (insert_crew);
      
      rapidjson::Document btn0;
      btn0 . SetObject ();
      btn0 . AddMember ("txt", "Back", alloc);
      btn0 . AddMember ("key", "back", alloc);
      
      rapidjson::Value buttons (rapidjson::kArrayType);
      buttons . PushBack (btn0, alloc); 
      
      doc . AddMember ("prompt", "Crew increased", alloc);
      doc . AddMember ("buttons", buttons, alloc);
    }
    else
    {
      rapidjson::Document btn0;
      btn0 . SetObject ();
      btn0 . AddMember ("txt", "Show your crew", alloc) . AddMember ("key", "show_crew", alloc);
      rapidjson::Document btn1;
      btn1 . SetObject ();
      btn1 . AddMember ("txt", "Add to crew", alloc) . AddMember ("key", "add_crew", alloc);
      
      rapidjson::Value buttons (rapidjson::kArrayType);
      buttons . PushBack (btn0, alloc) . PushBack (btn1, alloc); 
      
      doc . AddMember ("prompt", "You're hanging at the hideout.", alloc);
      doc . AddMember ("buttons", buttons, alloc);
    }

    rapidjson::StringBuffer buf;
    rapidjson::Writer <rapidjson::StringBuffer> writer(buf);
    doc . Accept (writer);

    std::cout << buf . GetString () << std::endl;
    strcat (output, buf . GetString ());
  }

  void CleanUp ()
  {
    sqlite3_finalize (_all_crew);
    sqlite3_close (_db); 
  }
};
