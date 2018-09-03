
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

  void populate_menu_show_crew (rapidjson::Document &doc, rapidjson::Document::AllocatorType &alloc)
  {
    rapidjson::Value crew (rapidjson::kArrayType);

    rapidjson::Value header (rapidjson::kArrayType);
    header . PushBack ("Name", alloc); 
    header . PushBack ("Intimidation", alloc); 
    
    crew . PushBack (header, alloc);
    
    int step;
    do
    { 
      step = sqlite3_step (_all_crew);
      if (step == SQLITE_ROW)
      { printf("%s: ", sqlite3_column_text (_all_crew, 0));
        printf("%s\n", sqlite3_column_text (_all_crew, 1));
        rapidjson::Value row (rapidjson::kArrayType);
        const unsigned char * raw_field = sqlite3_column_text (_all_crew, 1);
        char * field = const_cast<char *> (reinterpret_cast<const char*> (raw_field));
        size_t len = sqlite3_column_bytes (_all_crew, 1);
        row . PushBack (rapidjson::Value{}.SetString(field, len, alloc), alloc); 
        
        int intimidation = sqlite3_column_int (_all_crew, 2);
        row . PushBack (intimidation, alloc); 
        
        crew . PushBack (row, alloc);
      }
    }
    while (step != SQLITE_DONE);
    sqlite3_reset (_all_crew);

    rapidjson::Value tables (rapidjson::kArrayType);
    tables . PushBack (crew, alloc); 
    
    // buttons 
    rapidjson::Document btn0;
    btn0 . SetObject ();
    btn0 . AddMember ("txt", "Back", alloc) . AddMember ("key", "back", alloc);
    
    rapidjson::Value buttons (rapidjson::kArrayType);
    buttons . PushBack (btn0, alloc); 

    doc . AddMember ("prompt", "Crew:", alloc);
    doc . AddMember ("buttons", buttons, alloc);
    doc . AddMember ("tables", tables, alloc);
  }

  void message(const char * input, char * output)
  {
    std::cout << "input: " << input << std::endl;
    rapidjson::Document doc;
    doc . SetObject ();
    rapidjson::Document::AllocatorType &alloc = doc . GetAllocator ();

    if (strcmp ("show_crew", input) == 0)
    { populate_menu_show_crew (doc, alloc);
    }
    else if (strcmp ("add_crew", input) == 0)
    {
      sqlite3_stmt * insert_crew;
      const char * s = "insert into crew (id, name, intimidation, planning, computers) values (4, 'Matt', 0, 0, 0)";
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
      if (strcmp ("next", input) == 0)
      { HandleNextTurn ();
      }
      rapidjson::Document btn0;
      btn0 . SetObject ();
      btn0 . AddMember ("txt", "Next Turn", alloc) . AddMember ("key", "next", alloc);
      rapidjson::Document btn1;
      btn1 . SetObject ();
      btn1 . AddMember ("txt", "Add to crew", alloc) . AddMember ("key", "add_crew", alloc);
      rapidjson::Document btn2;
      btn2 . SetObject ();
      btn2 . AddMember ("txt", "Show your crew", alloc) . AddMember ("key", "show_crew", alloc);
      
      rapidjson::Value buttons (rapidjson::kArrayType);
      buttons . PushBack (btn0, alloc) . PushBack (btn1, alloc) . PushBack (btn2, alloc); 
      
      doc . AddMember ("prompt", "You're hanging at the hideout. What do you want to do?", alloc);
      doc . AddMember ("buttons", buttons, alloc);
    }

    rapidjson::StringBuffer buf;
    rapidjson::Writer <rapidjson::StringBuffer> writer(buf);
    doc . Accept (writer);

    std::cout << buf . GetString () << std::endl;
    strcat (output, buf . GetString ());
  }

  void HandleNextTurn ()
  {
    _turn_count++;
  }

  void CleanUp ()
  {
    sqlite3_finalize (_all_crew);
    sqlite3_close (_db); 
  }
};
