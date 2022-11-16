// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "TableEntry.h"

/* 
Represents a single table of N columns & variable columns
- 1x N-sized vector strings for col. names
- 1x N-sized vector of EntryType for col. types --> COULD ALSO BE CHARS
- 1x N * (# rows) 2D vector of TableEntries for actual data
- 1x string denoting table name
- 1x uint32_t denoting the first UNFILLED row index ("past-the-end")
*/

// ---------------------- FUNCTORS ----------------------
// FUNCTOR COMPARE LESS
class CompLess {
private:
    std::vector<TableEntry> val; // size 1
    uint32_t col;
    uint32_t &count;
    
public:
    // PARAMETERS: STRING INPUT, COL. DATATYPE, COL. IDX, COUNTER UINT32_T
    CompLess(std::string& str_in, EntryType coltype, uint32_t col_in, uint32_t& cnt_in);
    // Unary Predicate functor operator()
    bool operator() (std::vector<TableEntry>& row);
};

// FUNCTOR COMPARE GREATER
class CompGreater {
private:
    std::vector<TableEntry> val; // size 1
    uint32_t col;
    uint32_t& count;
    
public:
    // PARAMETERS: STRING INPUT, COL. DATATYPE, COL. IDX, COUNTER UINT32_T
    CompGreater(std::string& str_in, EntryType coltype, uint32_t col_in, uint32_t& cnt_in);
    // Unary Predicate functor operator()
    bool operator() (std::vector<TableEntry>& row);
};

// FUNCTOR COMPARE EQUAL
class CompEqual {
private:
    std::vector<TableEntry> val; // size 1
    uint32_t col;
    uint32_t &count;
    
public:
    // PARAMETERS: STRING INPUT, COL. DATATYPE, COL. IDX, COUNTER UINT32_T
    CompEqual(std::string& str_in, EntryType coltype, uint32_t col_in, uint32_t& cnt_in);
    // Unary Predicate functor operator()
    bool operator() (std::vector<TableEntry>& row);
};
// -------------------- END FUNCTORS --------------------

class Table {
public:
    uint32_t end_idx = 0;
    std::string table_name;

    std::unordered_map<std::string, uint32_t> names; // maps column name to indices
    std::vector<EntryType> types; // {double, int, bool, string}
    std::vector<std::vector<TableEntry> > entries;

    // BST & HASH METADATA
    std::map<TableEntry, std::vector<uint32_t> > bst;
    std::unordered_map<TableEntry, std::vector<uint32_t> > hash;
    std::string bst_col_name, hash_col_name;
    uint32_t bst_col_idx, hash_col_idx;
    bool bst_exists = false, hash_exists = false;

    // CREATE
    void create_local(std::string& name, uint32_t n);
    // INSERT
    void insert_local(uint32_t m);
    // DELETE FROM
    void delete_local(std::string& name, std::string colname, char op);
    // GENERATE (LOCAL TO TABLE)
    void generate_local(std::string& colname, char mode);
    // PRINT ALL
    void print_all_local(std::string& name, uint32_t ncols, 
            std::vector<std::string>& printed, bool is_quiet);
    // PRINT WHERE
    void print_local(std::string& name, std::string& colname, char op, uint32_t ncols, 
            std::vector<std::string>& printed, bool is_quiet);
};

/*
Handles program input, only ADT is the unordered-map of Table objects
- 1x unordered_map of tables  
*/

struct PrintPair {
    std::string colname; // column name
    uint32_t table; // table num
};

class Language {
public:
    bool quiet_mode = false;
    std::unordered_map<std::string, Table> dataset;
    std::string input;


    // CREATE
    void create_table(std::string name, uint32_t n);
    // INSERT
    void insert_rows(std::string name, uint32_t m);
    // DELETE FROM
    void delete_from(std::string name, std::string colname, char op);
    // GENERATE INDEX
    void generate_index(std::string name, std::string colname, char mode);
    // PRINT ALL
    void print_all(std::string name, uint32_t ncols, std::vector<std::string>& printed);
    // PRINT WHERE
    void print_where(std::string name, std::string colname, char op, 
            uint32_t ncols, std::vector<std::string>& printed);

    // JOIN --> KEEP ALL IN LANGUAGE
    void join_tables(std::string name1, std::string name2, std::string colname1, 
            std::string colname2, uint32_t ncols, std::vector<PrintPair>& pairs);
    // REMOVE TABLE --> KEEP ALL IN LANGUAGE
    void remove_table(std::string name);
    // GET OPTIONS
    void get_options(int argc, char** argv);
    // DRIVER PROGRAM
    void run();
};