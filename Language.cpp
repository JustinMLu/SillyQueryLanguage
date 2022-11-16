// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <climits>
#include <numeric>
#include <string>
#include <vector>
#include <getopt.h>
#include "Language.h"
#include "TableEntry.h"

// ---------------------- FUNCTORS ----------------------
// CTOR PARAMETERS: <CIN STRING ENTRY>, <COL. DATATYPE>, <COL. IDX>, <COUNTER VAR>
CompLess::CompLess(std::string& str_in, EntryType coltype, uint32_t col_in, uint32_t& cnt_in) 
: col(col_in), count(cnt_in) {
    switch (coltype) {
        case EntryType::String:
            val.emplace_back(str_in);   
            break;
            
        case EntryType::Double:
            val.emplace_back(stod(str_in));
            break;

        case EntryType::Int:
            val.emplace_back(stoi(str_in));
            break;

        case EntryType::Bool:
            if (str_in[0] == 't') {
                val.emplace_back(true);
            }
            else {
                val.emplace_back(false);
            }
            break;
    }
}

bool CompLess::operator() (std::vector<TableEntry>& row) {
    if (row[col] < val[0]) {
        count++;
    }
    return row[col] < val[0];
}

// CTOR PARAMETERS: <CIN STRING ENTRY>, <COL. DATATYPE>, <COL. IDX>, <COUNTER VAR>
CompGreater::CompGreater(std::string& str_in, EntryType coltype, uint32_t col_in, uint32_t& cnt_in) 
: col(col_in), count(cnt_in) {
    switch (coltype) {
        case EntryType::String:
            val.emplace_back(str_in);   
            break;
            
        case EntryType::Double:
            val.emplace_back(stod(str_in));
            break;

        case EntryType::Int:
            val.emplace_back(stoi(str_in));
            break;

        case EntryType::Bool:
            if (str_in[0] == 't') {
                val.emplace_back(true);
            }
            else {
                val.emplace_back(false);
            }
            break;
    }
}

bool CompGreater::operator() (std::vector<TableEntry>& row) {
    if (row[col] > val[0]) {
        count++;
    }
    return row[col] > val[0];
}

// CTOR PARAMETERS: <CIN STRING ENTRY>, <COL. DATATYPE>, <COL. IDX>, <COUNTER VAR>
CompEqual::CompEqual(std::string& str_in, EntryType coltype, uint32_t col_in, uint32_t& cnt_in) 
: col(col_in), count(cnt_in) {
    switch (coltype) {
        case EntryType::String:
            val.emplace_back(str_in);   
            break;
            
        case EntryType::Double:
            val.emplace_back(stod(str_in));
            break;

        case EntryType::Int:
            val.emplace_back(stoi(str_in));
            break;

        case EntryType::Bool:
            if (str_in[0] == 't') {
                val.emplace_back(true);
            }
            else {
                val.emplace_back(false);
            }
            break;
    }
}

bool CompEqual::operator() (std::vector<TableEntry>& row) {
    if (row[col] == val[0]) {
        count++;
    }
    return row[col] == val[0];
}
// -------------------- END FUNCTORS --------------------

// ---------------------- TABLE CLASS ----------------------
// TABLE CREATE
void Table::create_local(std::string& name, uint32_t n) {
    table_name = name;
    types.resize(n);
    end_idx = 0;
    std::string input; // used for both col. type & col. name

    // Reads in & pushes back column types
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> input; 

        if (input[0] == 'd') {
            types[i] = EntryType::Double;
        }
        else if (input[0] == 'i') {
            types[i] = EntryType::Int;
        }
        else if (input[0] == 'b') {
            types[i] = EntryType::Bool;
        }
        else {
            types[i] = EntryType::String;
        }
    }
    // Reads in & maps colnames to indices, also prints spec output
    std::cout << "New table " << table_name << " with column(s) ";
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> input;
        names[input] = i;
        std::cout << input << ' ';
    }
    std::cout << "created\n";
}

// TABLE INSERT
void Table::insert_local(uint32_t m) {
    size_t num_cols = types.size();
    std::string str_in;

    for (uint32_t i = end_idx; i < end_idx + m; i++) {
        // Create a temp vector to later assign as a row on 'entries'
        std::vector<TableEntry> temp;
        temp.reserve(num_cols);

        for (uint32_t j = 0; j < num_cols; j++) {
            std::cin >> str_in;
            // If column is type string, we must use the string input var
            if (types[j] == EntryType::String) {
                temp.emplace_back(str_in);
            }
            // If column is type double, use stod() to convert string to double
            else if (types[j] == EntryType::Double) {
                temp.emplace_back(stod(str_in));
            }
            // If column is type int, just cin as an int lol
            else if (types[j] == EntryType::Int) {
                temp.emplace_back(stoi(str_in));
            }
            // If column is a bool, then evaluate the 'true'/'false' string
            else { // BOOLEAN
                if (str_in[0] == 't') {
                    temp.emplace_back(true);
                }
                else {
                    temp.emplace_back(false);
                }
            }
            // If existing hash or bst found, add indices to hash/bst --> EMPTY BST/HASH STILL HAS TO BE INSERTED INTO
            if (hash_exists && j == hash_col_idx) {
                hash[temp.back()].push_back(i); //TEMP.BACK() NOT GUARANTEED TO BE TYPE OF HASH
                
            }
            else if (bst_exists && j == bst_col_idx) {
                bst[temp.back()].push_back(i); //TEMP.BACK() NOT GUARANTEED TO BE TYPE OF BST 
            }
        }
        // Now push temp into 'entries' as a row
        entries.push_back(temp);
    }
    // Print spec output (1/2)
    std::cout << "Added " << m << " rows to " << table_name 
            << " from position " << end_idx;
    // Update last unfilled row idx variable
    end_idx += m;
    // Print spec output (2/2)
    if (m != 0) {
        std::cout << " to " << end_idx - 1 << '\n';
    }
    else {
        std::cout << " to " << end_idx << '\n';
    }
}

// TABLE DELETE
void Table::delete_local(std::string& name, std::string colname, char op) {
    uint32_t rows_deleted = 0;
    uint32_t col_idx = names[colname];
    EntryType coltype = types[col_idx];
        
    // Input to TableEntry is handled inside functors
    std::string input;
    std::cin >> input;
    
    // Move all 'removed' elements to back of array using std::remove_if
    if (op == '>') {
        CompGreater pred(input, coltype, col_idx, rows_deleted);
        std::remove_if(entries.begin(), entries.end(), pred);
        entries.resize(entries.size() - rows_deleted);
    }
    else if (op == '<') {
        CompLess pred(input, coltype, col_idx, rows_deleted);
        std::remove_if(entries.begin(), entries.end(), pred);
        entries.resize(entries.size() - rows_deleted);
    }
    else {
        CompEqual pred(input, coltype, col_idx, rows_deleted);
        std::remove_if(entries.begin(), entries.end(), pred);
        entries.resize(entries.size() - rows_deleted);
    }

    // If existing hash or bst found, re-generate the index
    if (rows_deleted != 0) {
        if (!hash.empty() && hash_exists) {
            generate_local(hash_col_name, 'h'); // RE-GENERATE AT ORIGINAL INDEX HOLY FUCK THIS TOOK A WHILE
        }
        if (!bst.empty() && bst_exists) {
            generate_local(bst_col_name, 'b');
        }
    }
    std::cout << "Deleted " << rows_deleted << " rows from " << name << '\n';
    end_idx -= rows_deleted;
}

// GENERATE LOCAL
void Table::generate_local(std::string& colname, char mode) {
    // CHECK IF EXISTING HASH/BST EXISTS AT THE EXISTING COL INDEX; IF SO, DELETE IT
    if (hash_exists) {
        hash.erase(hash.begin(), hash.end());
        hash_exists = false;
    }
    // SAME HERE FOR BST -> MIGHT HAVE TO ONLY ERASE IF !BST.EMPTY()
    if (bst_exists) {
        bst.erase(bst.begin(), bst.end());
        bst_exists = false;
    }
    // Finally we can generate the bst/hash
    uint32_t col_idx = names[colname];

    if (mode == 'h') {
        for (uint32_t i = 0; i < entries.size(); i++) {
            hash[entries[i][col_idx]].push_back(i);
        }
        hash_col_name = colname;
        hash_col_idx = col_idx;
        hash_exists = true;
    }
    else {
        for (uint32_t i = 0; i < entries.size(); i++) {
            bst[entries[i][col_idx]].push_back(i);
        }
        bst_col_name = colname;
        bst_col_idx = col_idx;
        bst_exists = true;
    }
}

// PRINT_ALL
void Table::print_all_local(std::string& name, uint32_t ncols, 
        std::vector<std::string>& printed, bool is_quiet) {

    uint32_t rows_printed = 0;
    // Print title & all rows (IF NOT QUIET)
    if (is_quiet) {
        rows_printed = (uint32_t) entries.size();
    }
    else {
        for (uint32_t i = 0; i < ncols; i++) {
            std::cout << printed[i] << ' ';
        }
        std::cout << '\n';

        for (uint32_t i = 0; i < entries.size(); i++) {
            for (uint32_t j = 0; j < ncols; j++) {
                std::cout << entries[i][names[printed[j]]] << ' ';
            }
            std::cout << '\n';
            rows_printed++;
        }
    }
    std::cout << "Printed " << rows_printed << " matching rows from " << name << '\n';
}

void Table::print_local(std::string& name, std::string& colname, char op, uint32_t ncols, 
        std::vector<std::string>& printed, bool is_quiet) {

    uint32_t rows_printed = 0;
    uint32_t col_idx = names[colname];
    EntryType coltype = types[col_idx];
        
    // Input = the value you want to compare to 'colname' using functor
    std::string input;
    std::cin >> input;

    // Print title
    if (!is_quiet) {
        for (uint32_t i = 0; i < ncols; i++) {
            std::cout << printed[i] << ' ';
        }
        std::cout << '\n';
    }

    // IF BST EXISTS, DO EVERYTHING DIFFERENTLY
    if (bst_exists && !bst.empty() && col_idx == bst_col_idx) {
        std::vector<TableEntry> val; // size 1

        switch (coltype) {
            case EntryType::String:
                val.emplace_back(input);   
                break;
            case EntryType::Double:
                val.emplace_back(stod(input));
                break;
            case EntryType::Int:
                val.emplace_back(stoi(input));
                break;
            case EntryType::Bool:
                if (input[0] == 't') {
                    val.emplace_back(true);
                }
                else {
                    val.emplace_back(false);
                }
                break;
        }

        TableEntry value(val[0]);
        std::vector<uint32_t> idx_list;
        auto it = bst.find(value);

        // COULD SPEED UP QUIET MODE HERE
        switch (op) {
            case '=':
                if (it != bst.end()) {
                    // This time directly access it to get vector of matches
                    for (auto i : it->second) {
                        idx_list.push_back(i);
                        rows_printed++;
                    }
                }
                break;

            case '>':
                if (it == bst.end()) {
                    it = bst.upper_bound(value);
                }
                else {
                    it++;
                }
                for (auto start = it; start != bst.end(); start++) {
                    for (auto i : start->second) {
                        idx_list.push_back(i);
                        rows_printed++;
                    }
                }
                break;

            case '<':
                if (it == bst.end()) {
                    it = bst.lower_bound(value);
                }
                for (auto start = bst.begin(); start != it; start++) {
                    for (auto i : start->second) {
                        idx_list.push_back(i);
                        rows_printed++;
                    }
                }
                break;
        }
        // If not in quiet mode: print all rows & all columns for each row
        if (!is_quiet) {
            for (uint32_t idx : idx_list) {
                for (uint32_t j = 0; j < ncols; j++) {
                    std::cout << entries[idx][names[printed[j]]] << ' ';
                }
                std::cout << '\n'; 
            }
        }
    }
    // NO BST FOUND --> check hash
    else {
        if (op == '>') {
            CompGreater pred(input, coltype, col_idx, rows_printed); 
            // check hash here????
            // HOPEFULLY OPTIMIZED
            if (is_quiet) {
                for (uint32_t i = 0; i < entries.size(); i++) {
                    pred(entries[i]); // loops thru every row in entries, using functor on entire row
                }
            }
            else {
                for (uint32_t i = 0; i < entries.size(); i++) {
                    // If functor @ colname returns true, print the values we want
                    if (pred(entries[i])) {
                        // For every column we want
                        for (uint32_t j = 0; j < ncols; j++) {
                            std::cout << entries[i][names[printed[j]]] << ' ';
                        }
                        std::cout << '\n';
                    }
                }
            } 
        }
        else if (op == '<') {
            CompLess pred(input, coltype, col_idx, rows_printed);
            if (is_quiet) {
                for (uint32_t i = 0; i < entries.size(); i++) {
                    pred(entries[i]);
                }
            }
            else {
                for (uint32_t i = 0; i < entries.size(); i++) {
                    // If functor @ colname returns true, print the values we want
                    if (pred(entries[i])) {
                        // For every column we want
                        for (uint32_t j = 0; j < ncols; j++) {
                            std::cout << entries[i][names[printed[j]]] << ' ';
                        }
                        std::cout << '\n';
                    }
                }
            }
        }
        else { // EQUALS
            CompEqual pred(input, coltype, col_idx, rows_printed);
            if (is_quiet) {
                for (uint32_t i = 0; i < entries.size(); i++) {
                    pred(entries[i]);
                }
            }
            else {
                for (uint32_t i = 0; i < entries.size(); i++) {
                    // If functor @ colname returns true, print the values we want
                    if (pred(entries[i])) {
                        // For every column we want
                        for (uint32_t j = 0; j < ncols; j++) {
                            std::cout << entries[i][names[printed[j]]] << ' ';
                        }
                        std::cout << '\n';
                    }
                }
            }
        }
    }
    std::cout << "Printed " << rows_printed << " matching rows from " << name << '\n';
}

// -------------------- LANGUAGE CLASS --------------------
// CREATE (REVISED)
void Language::create_table(std::string name, uint32_t n) {
    // If table already exists
    if (dataset.find(name) != dataset.end()) {
        std::cout << "Error during CREATE: Cannot create already existing table " 
                << name << "\n";

        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    dataset[name].create_local(name, n); 
}

// INSERT (REVISED)
void Language::insert_rows(std::string name, uint32_t m) {
    if (dataset.find(name) == dataset.end()) {
        std::cout << "Error during INSERT: " << name
                << " does not name a table in the database\n";
        
        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    dataset[name].insert_local(m);
}

// DELETE FROM (REVISED)
void Language::delete_from(std::string name, std::string colname, char op) {
    // First check if the table even exists
    if (dataset.find(name) == dataset.end()) {
        std::cout << "Error during DELETE: " << name
                << " does not name a table in the database\n";
        
        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    // Then check if colname is even in the table
    if (dataset[name].names.count(colname) == 0) {
        std::cout << "Error during DELETE: " <<  colname 
                << " does not name a column in " << name << '\n';
        
        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    dataset[name].delete_local(name, colname, op);
}

// GENERATE INDEX (REVISED)
void Language::generate_index(std::string name, std::string colname, char mode) {
    // First check if the table even exists
    if (dataset.find(name) == dataset.end()) {
        std::cout << "Error during GENERATE: " << name
                << " does not name a table in the database\n";

        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    // Then check if colname is even in the table
    if (dataset[name].names.count(colname) == 0) {
        std::cout << "Error during GENERATE: " <<  colname 
                << " does not name a column in " << name << '\n';

        std::cin.ignore(INT_MAX, '\n');
        return;
    }

    dataset[name].generate_local(colname, mode);

    // Spec prints
    if (mode == 'h') {
        std::cout << "Created hash index for table " << name 
                << " on column " << colname << '\n';
    }
    else {
        std::cout << "Created bst index for table " << name 
        << " on column " << colname << '\n';
    }
}

void Language::print_all(std::string name, uint32_t ncols, std::vector<std::string>& printed) {
    // First check if the table even exists
    if (dataset.find(name) == dataset.end()) {
        std::cout << "Error during PRINT: " << name
                << " does not name a table in the database\n";

        std::cin.ignore(INT_MAX, '\n');
        return;
    }

    // Error check every column
    auto temp_names = &dataset[name].names;
    for (auto column : printed) {
        if (temp_names->count(column) == 0) {
            std::cout << "Error during PRINT: " <<  column 
            << " does not name a column in " << name << '\n';

            std::cin.ignore(INT_MAX, '\n');
            return;
        }
    }
    dataset[name].print_all_local(name, ncols, printed, quiet_mode);
}

// PRINT WHERE -->        "281class"            "Y/N"           operation   <print_row1> ... <print_rowN>
void Language::print_where(std::string name, std::string colname, char op, 
        uint32_t ncols, std::vector<std::string>& printed) {
    
    // First check if the table even exists
    if (dataset.find(name) == dataset.end()) {
        std::cout << "Error during PRINT: " << name
                << " does not name a table in the database\n";

        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    auto temp_names = &dataset[name].names;
    // Then check if colname is even in the table
    if (temp_names->count(colname) == 0) {
        std::cout << "Error during PRINT: " <<  colname 
                << " does not name a column in " << name << '\n';

        std::cin.ignore(INT_MAX, '\n');
        return;
    }

    // Error check every column
    for (auto column : printed) {
        if (temp_names->count(column) == 0) {
            std::cout << "Error during PRINT: " <<  column 
            << " does not name a column in " << name << '\n';

            std::cin.ignore(INT_MAX, '\n');
            return;
        }
    }
    dataset[name].print_local(name, colname, op, ncols, printed, quiet_mode);
}

//JOIN TABLES
void Language::join_tables(std::string name1, std::string name2, std::string colname1, 
        std::string colname2, uint32_t ncols, std::vector<PrintPair>& pairs) {
    
    // Chcek if table names 1 & 2 exist in the database
    if (dataset.find(name1) == dataset.end()) {
        std::cout << "Error during JOIN: " << name1
                << " does not name a table in the database\n";

        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    // Same but for table 2
    if (dataset.find(name2) == dataset.end()) {
        std::cout << "Error during JOIN: " << name1
                 << " does not name a table in the database\n";

        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    // Check if both columns are even in the table
    if (dataset[name1].names.count(colname1) == 0) {
        std::cout << "Error during JOIN: " <<  colname1 
                << " does not name a column in " << name1 << '\n';

        std::cin.ignore(INT_MAX, '\n');
        return;
    }
    // Same but for table 2
    if (dataset[name2].names.count(colname2) == 0) {
        std::cout << "Error during JOIN: " <<  colname2 
                << " does not name a column in " << name2 << '\n';

        std::cin.ignore(INT_MAX, '\n');
        return;
    }

    auto temp_table1 = &dataset[name1].names;
    auto temp_table2 = &dataset[name2].names;
    for (auto pair : pairs) {
        // If the colname is for table 1 & no such colname exists
        if (pair.table == 1 && temp_table1->count(pair.colname) == 0) {
            std::cout << "Error during JOIN: " <<  pair.colname 
                    << " does not name a column in " << name1 << '\n';

            std::cin.ignore(INT_MAX, '\n');
            return;
        }
        else if (pair.table == 2 && temp_table2->count(pair.colname) == 0) {
            std::cout << "Error during JOIN: " <<  pair.colname 
                    << " does not name a column in " << name2 << '\n';

            std::cin.ignore(INT_MAX, '\n');
            return;
        }
    }

    auto table_1 = &dataset[name1];
    auto table_2 = &dataset[name2];
    uint32_t col1_idx = table_1->names[colname1];
    uint32_t col2_idx = table_2->names[colname2];
    uint32_t num_printed = 0;

    // Print titles (OPTIMIZED?)
    if (!quiet_mode) {
        for (uint32_t i = 0; i < ncols; i++) {
            std::cout << pairs[i].colname << ' ';
        }
        std::cout << '\n';
    }

    // If a correct hash at column colname2 exists, might as well use it
    if (!table_2->hash.empty() && table_2->hash_exists
            && table_2->hash_col_idx == col2_idx) {
        
        if (quiet_mode) {
            for (uint32_t i = 0; i < table_1->entries.size(); i++) { // go thru all rows of table 1
                // Every entry in table 1 @ colname1 stored here --> SPEC: Sith, Paoletti
                TableEntry temp_entry = table_1->entries[i][col1_idx];
                num_printed += (uint32_t) table_2->hash[temp_entry].size();
            }
        }
        else {
            for (uint32_t i = 0; i < table_1->entries.size(); i++) { // go thru all rows of table 1
                // Every entry in table 1 @ colname1 stored here --> SPEC: Sith, Paoletti
                TableEntry temp_entry = table_1->entries[i][col1_idx];
                // If non-empty vector, go through 
                for (uint32_t j = 0; j < table_2->hash[temp_entry].size(); j++) { // go thru ROW-IDX vec of all matching entries in TABLE 2
                    // Print all SPECIFIED columns in this vector
                    for (uint32_t k = 0; k < ncols; k++) {
                        if (pairs[k].table == 1) {
                            // If table 1, then we can just use 'i' to get the row we want for table1
                            std::cout << table_1->entries[i][table_1->names[pairs[k].colname]] << ' ';
                        }
                        else {
                            // table 2hash[temp_entry] gives vector of INDICES FOR TABLE 2 ROWS that match temp_entry
                            std::cout << table_2->entries[table_2->hash[temp_entry][j]][table_2->names[pairs[k].colname]] << ' ';
                        }
                    }
                    std::cout << '\n';
                    num_printed++;
                }
            }
        }
    }
    else {
        // Make temp hash for all colname2 values if table2 does not already have a hash there
        std::unordered_map<TableEntry, std::vector<uint32_t> > temp_hash;
        for (uint32_t i = 0; i < table_2->entries.size(); i++) {
            temp_hash[table_2->entries[i][col2_idx]].push_back(i);
        }

        if (quiet_mode) {
            for (uint32_t i = 0; i < table_1->entries.size(); i++) { // go thru all rows of table 1
                // Every entry in table 1 @ colname1 stored here --> SPEC: Sith, Paoletti
                TableEntry temp_entry = table_1->entries[i][col1_idx];
                num_printed += (uint32_t) temp_hash[temp_entry].size();
            }
        }
        else {
            for (uint32_t i = 0; i < table_1->entries.size(); i++) { // go thru all rows of table 1
                // Every entry in table 1 @ colname1 stored here --> SPEC: Sith, Paoletti
                TableEntry temp_entry = table_1->entries[i][col1_idx];
                // If non-empty vector, go through 
                for (uint32_t j = 0; j < temp_hash[temp_entry].size(); j++) { // go thru ROW-IDX vec of all matching entries in TABLE 2
                    // Print all SPECIFIED columns in this vector
                    for (uint32_t k = 0; k < ncols; k++) {
                        if (pairs[k].table == 1) {
                            // If table 1, then we can just use 'i' to get the row we want for table1
                            std::cout << table_1->entries[i][table_1->names[pairs[k].colname]] << ' ';
                        }
                        else {
                            // temp hash --> [temp_entry] gives vector of INDICES FOR TABLE 2 ROWS that match temp_entry
                            std::cout << table_2->entries[temp_hash[temp_entry][j]][table_2->names[pairs[k].colname]] << ' ';
                        }
                    }
                    std::cout << '\n';
                    num_printed++;
                } 
            }
        }
    }
    std::cout << "Printed " << num_printed << " rows from joining " << name1 << " to " << name2 << '\n';
}

// REMOVE TABLE
void Language::remove_table(std::string name) {
    // If a Table w/ given name exists, delete it; else print error
    if (dataset.find(name) != dataset.end()) {
        dataset.erase(name);
        std::cout << "Table " << name << " deleted\n";
    }
    else {
        std::cout << "Error during REMOVE: " << name
                << " does not name a table in the database\n";
        std::cin.ignore(INT_MAX, '\n');
    }
}

void Language::get_options(int argc, char** argv) {
    int option_idx = 0, option = 0;
    opterr = false;
    struct option long_opts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "quiet", no_argument, nullptr, 'q' }};
    
    while ((option = getopt_long(argc, argv, "hq", long_opts, &option_idx)) != -1) {
        switch (option) {
            case 'h':
                std::cout << "Commands: CREATE, INSERT INTO, DELETE FROM, GENERATE INDEX, PRINT, JOIN, REMOVE, QUIT\n";
                exit(0);
                break;

            case 'q':
                quiet_mode = true;
                break;
        }
    }
}

void Language::run() {
    std::string command, name_in, trash;
    char cmd_in;
    do {
        // Read in command & call respective function
        std::cout << "% ";
        std::cin >> command;
        cmd_in = command[0];

        if (command.substr(0,1) == "#") {
            std::getline(std::cin, command);
        }
        else if (cmd_in == 'C') {
            uint32_t cols_in;
            std::cin >> name_in;
            std::cin >> cols_in;
            create_table(name_in, cols_in);
        }
        else if (cmd_in == 'I') {
            uint32_t rows_in;
            std::cin >> trash >> name_in >> rows_in >> trash;
            insert_rows(name_in, rows_in);
        }
        else if (cmd_in == 'D') {
            char operand;
            std::string col_name;
            std::cin >> trash >> name_in;
            std::cin >> trash >> col_name;
            std::cin >> operand;
            delete_from(name_in, col_name, operand);
        }
        else if (cmd_in == 'G') {
            std::string index_type, col_name;
            std::cin >> trash >> name_in >> index_type;
            std::cin >> trash >> trash >> col_name;
            generate_index(name_in, col_name, index_type[0]);
        }
        else if (cmd_in == 'P') {
            char operand;
            uint32_t ncols;
            std::string col_elt, col_name;

            // Read in number of cols, then push their names into vec
            std::cin >> trash >> name_in >> ncols;
            std::vector<std::string> printed_cols;

            for (uint32_t i = 0; i < ncols; i++) {
                std::cin >> col_elt;
                printed_cols.push_back(col_elt);
            }

            std::cin >> trash; // "WHERE"
            if (trash[0] == 'W') { // print where
                std::cin >> col_name >> operand; 
                print_where(name_in, col_name, operand, ncols, printed_cols);
            }
            else { // print all
                print_all(name_in, ncols, printed_cols);
            }
        }
        else if (cmd_in == 'J') {
            std::string table1, col1;
            std::string table2, col2;
            uint32_t ncols;
            std::vector<PrintPair> pairs;
            
            std::cin >> table1 >> trash >> table2;
            std::cin >> trash >> col1 >> trash >> col2;
            std::cin >> trash >> trash >> ncols;
            pairs.resize(ncols);

            for (uint32_t i = 0; i < ncols; i++) {
                std::cin >> pairs[i].colname >> pairs[i].table;
            }
            join_tables(table1, table2, col1, col2, ncols, pairs);
        }
        else if (cmd_in == 'R') {
            std::cin >> name_in;
            remove_table(name_in);
        }
        // Comments + invalid commands
        else if (cmd_in != 'Q') {
            std::cout << "Error: unrecognized command\n";
            std::cin.ignore(INT_MAX, '\n');
        }

    } while (cmd_in != 'Q');

    // Print spec end message & return 0 (if no errors)
    std::cout << "Thanks for being silly!\n";
}


// ------------------------- MAIN -------------------------
int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin >> std::boolalpha;
    std::cout << std::boolalpha;

    // Create Language Object and run program
    Language language;
    language.get_options(argc, argv);
    language.run();
    return 0;
}