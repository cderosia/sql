//  Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  hashmap.h
//  p3-sql
//
//  Created by Carter DeRosia on 3/9/23.
//

#ifndef command_h
#define command_h

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include "table.h"
#include "TableEntry.h"



class database {
	
public:
	
	
	
	explicit database(bool qm) : quiet_mode(qm) {}
	
	// handles CREATE command
	// CREATE: Possible errors
	//A table named <tablename> already exists in the database
	void create() {
		
		std::string tablename;
		std::cin >> tablename;
		
		int numCols;
		std::cin >> numCols;
		
		if (db.count(tablename) != 0) {
			std::cout << "Error during CREATE: Cannot create already existing table "
			<< tablename << "\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		// constructs a table with read in name and number of columns
		table t(numCols);
		// insert the new table into the hashmap using []
		std::pair<std::string, table> table_entry (tablename, t);
		db.insert(table_entry);
		
		db[tablename].resize_data(size_t(numCols));
		
		// read in col types
		std::string colType;
		for (int i = 0; i < numCols; i++) {
			std::cin >> colType;
			db[tablename].push_col_types(colType);
			
			
		}
		
		//db[tablename].print_col_types();
		
		
		std::vector<TableEntry> col_name_vec;
		// read in col names
		std::string colName;
		for (int i = 0; i < numCols; i++) {
			std::cin >> colName;
			TableEntry te(colName);
			col_name_vec.emplace_back(te);
			db[tablename].push_to_col_names(colName);
			//std::cout << colName << std::endl;
		}
		
		db[tablename].add_row(col_name_vec);
		
		
		
		std::cout << "New table " << tablename << " ";
		
		std::cout << "with column(s) ";
		for (int i = 0; i < numCols; ++i) {
			std::cout << db[tablename].get_col_name(i) << " ";
		}
		
		std::cout << "created\n";
	} // create
	
	
	
	// handles REMOVE command
	// REMOVE: Possible errors
	// <tablename> is not the name of a table in the database
	void remove_table() {
		
		std::string tablename;
		std::cin >> tablename;
		
		if (db.count(tablename) == 0) {
			std::string line;
			std::getline(std::cin, line);
			std::cout << "Error during REMOVE: " << tablename << " does not name a table in the database\n";
			return;
		}
		db.erase(tablename); // remove the table
		
		std::cout << "Table " << tablename << " deleted\n";
		
	} // remove table
	
	
	
	// handles INSERT command
	// INSERT: Possible errors
	// <tablename> is not the name of a table in the database
	void insert_into_table() {
		
		std::string into;
		std::cin >> into;
		
		std::string tablename;
		std::cin >> tablename;
		
		
		
		
		int numRows;
		std::cin >> numRows;
		
		std::string ROWS;
		std::cin >> ROWS;
		
		
		
		// Check if the table exists in the database
		if (db.count(tablename) == 0) {
			
			std::cout << "Error during INSERT: " << tablename << " does not name a table in the database\n";
			return;
		}
		
		db[tablename].resize_data(size_t(db[tablename].get_num_rows() + numRows));
		
		
		
		int p1 = db[tablename].get_num_rows();
		
		
		
		
		// Table exists in database, so insert new rows
		
		
		
		
		db[tablename].add_rows(numRows);
		
		
		
		std::cout << "Added " << numRows << " rows to " << tablename << " from position " << p1 - 1<< " to " << p1 + numRows - 2  << "\n";
		
		
		
		
	} // insert
	
	
	// handles PRINT command
	// PRINT: Possible errors
	// <tablename> is not the name of a table in the database
	// <colname> is not the name of a column in the table specified by <tablename>
	// One (or more) of the <print_colname>s are not the name of a column in the table specified by <tablename> (only print the name of the first such column encountered)
	void print_table() {
		
		
		std::string from;
		std::cin >> from;
		
		std::string tablename;
		std::cin >> tablename;
		
		if (db.count(tablename) == 0) {
			std::string line;
			std::getline(std::cin, line);
			std::cout << "Error during PRINT: " << tablename << " does not name a table in the database\n";
			return;
		}
		
		
		int num_to_print;
		std::cin >> num_to_print;
		
		// vector of strings to hold the cols to print
		std::vector<std::string> cols_vec;
		
		// loop n times to read in the col names and print them and push them to cols_vec
		for (int i = 0; i < num_to_print; i++) {
			std::string col_to_print;
			std::cin >> col_to_print;
			
			if (!db[tablename].is_valid_col(col_to_print)) {
				std::cout << "Error during PRINT: " << col_to_print << " does not name a column in " <<tablename << "\n";
				std::string line;
				getline(std::cin, line);
				return;
			}
			
			cols_vec.emplace_back(col_to_print);
			
		}
		
		std::string all_or_where;
		std::cin >> all_or_where;
		
		std::string col_name;
		
		if (all_or_where == "WHERE") {
			
			std::cin >> col_name;
			
			if (!db[tablename].is_valid_col(col_name)) {
				std::cout << "Error during PRINT: " << col_name << " does not name a column in " << tablename << "\n";
				std::string line;
				getline(std::cin, line);
				return;
			}
			
		}
		
		
		
		for (int i = 0; i < num_to_print; i++) {
			if (!quiet_mode) {
				std::cout << cols_vec[size_t(i)] << " ";
			}
		}
		if (!quiet_mode) {
			std::cout << "\n";
		}
		
		
		
		
		
		//std::cout << "cols_vec.size() " <<  cols_vec.size() << std::endl;
		//std::cout << "database[tablename].get_num_rows() " << database[tablename].get_num_rows() << std::endl;
		bool qmode = quiet_mode;
		
		if (all_or_where == "ALL") {
			
			int num_printed = db[tablename].print_all(cols_vec, qmode);
			
			
			
			
			std::cout << "Printed " << num_printed << " matching rows from " << tablename << "\n";
			
		}
		
		else if (all_or_where == "WHERE") {
			
			
			
			
			char op;
			std::cin >> op;
			
			int cond_col_idx = static_cast<int>(db[tablename].get_col_index(col_name));
			int num_printed;
			
			if (db[tablename].get_col_type(cond_col_idx) == "string") {
				std::string value;
				std::cin >> value;
				TableEntry te(value);
				num_printed = db[tablename].print_where(cols_vec, col_name, op, te, qmode);
			}
			else if (db[tablename].get_col_type(cond_col_idx) == "int") {
				int value;
				std::cin >> value;
				TableEntry te(value);
				num_printed = db[tablename].print_where(cols_vec, col_name, op, te, qmode);
			}
			else if (db[tablename].get_col_type(cond_col_idx) == "bool") {
				std::string value;
				std::cin >> value;
				TableEntry te(value);
				num_printed = db[tablename].print_where(cols_vec, col_name, op, te, qmode);
			}
			else {
				double value;
				std::cin >> value;
				TableEntry te(value);
				num_printed = db[tablename].print_where(cols_vec, col_name, op, te, qmode);
			}
			
			
			
			
			std::cout << "Printed " << num_printed << " matching rows from " << tablename << "\n";
			
			
		}
		
		
		
		
	} // print
	
	
	
	
	// handles DELETE command
	// Deletes all rows from the table specified by <tablename> where the value of the entry in <colname> satisfies the operation <OP> with the given value <value>. You can assume that <value> will always be of the same type as <colname>
	// DELETE: Possible errors
	// <tablename> is not the name of a table in the database
	// <colname> is not the name of a column in the table specified by <tablename>
	void delete_from_table() {
		
		std::string from;
		std::cin >> from;
		
		std::string tablename;
		std::cin >> tablename;
		
		if (db.count(tablename) == 0) {
			std::cout << "Error during DELETE: " << tablename << " does not name a table in the database\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		
		
		
		std::string where;
		std::cin >> where;
		
		std::string col_name;
		std::cin >> col_name;
		
		if (!db[tablename].is_valid_col(col_name)) {
			std::cout << "Error during DELETE: " << col_name << " does not name a column in " << tablename << "\n";
			
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		char op;
		std::cin >> op;
		
		int cond_col_idx = static_cast<int>(db[tablename].get_col_index(col_name));
		int num_rows_deleted;
		
		if (db[tablename].get_col_type(cond_col_idx) == "string") {
			
			std::string value;
			std::cin >> value;
			TableEntry te(value);
			num_rows_deleted = db[tablename].delete_rows(col_name, op, te);
		}
		else if (db[tablename].get_col_type(cond_col_idx) == "int") {
			
			int value;
			std::cin >> value;
			TableEntry te(value);
			num_rows_deleted = db[tablename].delete_rows(col_name, op, te);
		}
		else if (db[tablename].get_col_type(cond_col_idx) == "bool") {
			
			std::string value;
			std::cin >> value;
			TableEntry te(value);
			num_rows_deleted = db[tablename].delete_rows(col_name, op, te);
		}
		else {
			double value;
			std::cin >> value;
			TableEntry te(value);
			num_rows_deleted = db[tablename].delete_rows(col_name, op, te);
		}
		
		
		
		std::cout << "Deleted " << num_rows_deleted << " rows from " << tablename << "\n";
		
		
	} // delete
	
	// handles JOIN command
	// JOIN: Possible errors
	// <tablenameX> is not the name of a table in the database
	// One (or more) of the <colname>s or <print_colname>s are not the name of a column in the table specified by <tablenameX> (only print the name of the first such column encountered)
	
	void join_tables() {
		// Parse command
		std::string table1, andd, table2, where, colname1, colname2, PRINT;
		std::vector<std::string> print_colnames;
		std::vector<int> print_table_num;
		char equal;
		int n;
		std::cin >> table1 >> andd >> table2 >> where >> colname1 >> equal >> colname2 >> andd >> PRINT >> n;
		
		
		
		// Check tables exist
		if (db.count(table1) == 0) {
			std::cout << "Error during JOIN: " << table1 << " does not name a table in the database\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		if (db.count(table2) == 0) {
			std::cout << "Error during JOIN: " << table2 << " does not name a table in the database\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		// Check columns exist
		if (!db[table1].is_valid_col(colname1)) {
			std::cout << "Error during JOIN: " << colname1 << " does not name a column in " << table1 << "\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		if (!db[table2].is_valid_col(colname2)) {
			std::cout << "Error during JOIN: " << colname2 << " does not name a column in " << table2 << "\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		
		for (int i = 0; i < n; i++) {
			std::string print_col_name;
			std::cin >> print_col_name;
			print_colnames.emplace_back(print_col_name);
			int table_num;
			std::cin >> table_num;
			print_table_num.push_back(table_num);
			
			if (table_num == 1) {
				if (!db[table1].is_valid_col(print_col_name)) {
					std::cout << "Error during JOIN: " << print_col_name << " does not name a column in " << table1 << "\n";
					std::string line;
					getline(std::cin, line);
					return;
				}
			}
			
			if (table_num == 2) {
				if (!db[table2].is_valid_col(print_col_name)) {
					std::cout << "Error during JOIN: " << print_col_name << " does not name a column in " << table2 << "\n";
					std::string line;
					getline(std::cin, line);
					return;
				}
			}
			
		}
		
		if (!quiet_mode) {
			for (const auto& colname : print_colnames) {
				std::cout << colname << " ";
			}
			std::cout << "\n";
		}
		
		
		
		// finds the value in table2 at col_name_2
		// puts those values in an unordered map
		std::unordered_map<TableEntry, std::deque<int>> temp_hash;
		int col_2_idx = static_cast<int>(db[table2].get_col_index(colname2));
		for (int j = 1; j < db[table2].get_num_rows(); j++) {
			TableEntry key(db[table2].get_data(j, col_2_idx));
			temp_hash[key].emplace_back(j);
		}
		
		int col_1_idx = static_cast<int>(db[table1].get_col_index(colname1));
		int num_printed = 0;
		//	std::cout << db[table1].get_num_rows() << std::endl;
		for (int i = 1; i < db[table1].get_num_rows(); i++) {
			TableEntry key(db[table1].get_data(i, col_1_idx));
			
			// if its in both
			if (temp_hash.count(key) > 0) {
				
				size_t num_occurances = temp_hash[key].size();
				for (int j = 0; j < static_cast<int>(num_occurances); j++) {
					for (int k = 0; k < static_cast<int>(print_colnames.size()); k++) {
						if (!quiet_mode) {
							if (print_table_num[size_t(k)] == 1) {
								std::cout << db[table1].get_data(i, static_cast<int>(db[table1].get_col_index(print_colnames[size_t(k)]))) << " ";
							}
							else {
								std::cout << db[table2].get_data(temp_hash[key][size_t(j)], static_cast<int>(db[table2].get_col_index(print_colnames[size_t(k)]))) << " ";
							}
						}
					}
					num_printed++;
					if (!quiet_mode) {
						std::cout << "\n";
					}
				}
				
			}
		}
		
		
		
		std::cout << "Printed " << num_printed << " rows from joining " << table1 << " to " << table2 << "\n";
	}
	
	
	// handles GENERATE command
	void generate_index() {
		
		std::string forr;
		std::cin >> forr;
		
		std::string tablename;
		std::cin >> tablename;
		
		if (db.count(tablename) == 0) {
			std::cout << "Error during GENERATE: " << tablename << " does not name a table in the database\n";
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		std::string type;
		std::cin >> type;
		
		std::string INDEX;
		std::cin >> INDEX;
		
		std::string ON;
		std::cin >> ON;
		
		std::string colname;
		std::cin >> colname;
		
		if (!db[tablename].is_valid_col(colname)) {
			std::cout << "Error during GENERATE: " << colname << " does not name a column in " << tablename << "\n";
			
			std::string line;
			getline(std::cin, line);
			return;
		}
		
		
		
		if (type == "hash") {
			
			db[tablename].generate_hash_index(colname);
			std::cout << "Created " << type << " index for table " << tablename << " on column " << colname << "\n";
			
		}
		
		else if (type == "bst") {
			
			db[tablename].generate_bst_index(colname);
			std::cout << "Created " << type << " index for table " << tablename << " on column " << colname << "\n";
			
		}
		
		
		
		
		
		
	} // generate
	
	
	
	// sets up the do while for the input file
	void read_input() {
		
		std::string cmd;
		do {
			std::cout << "% ";
			
			std::cin >> cmd;
			
			// process commands
			
			if (cmd.find("#") != std::string::npos) {
				std::string line;
				getline(std::cin, line);
			}
			
			else if (cmd == "CREATE") {
				
				create();
				
			}
			else if (cmd == "REMOVE") {
				
				remove_table();
			}
			else if (cmd == "INSERT") {
				
				insert_into_table();
			}
			
			
			else if (cmd == "PRINT") {
				
				print_table();
				
			}
			
			else if (cmd == "DELETE") {
				
				delete_from_table();
			}
			
			
			else if (cmd == "JOIN") {
				join_tables();
			}
			
			
			else if (cmd == "GENERATE") {
				generate_index();
				
			}
			else if (cmd != "QUIT") {
				std::cout << "Error: unrecognized command\n" ;
				std::string line;
				getline(std::cin, line);
				
			}
			
			
			
			
		} while (cmd != "QUIT");
		
		std::cout << "Thanks for being silly!\n";
	} // read_input
	
	
	
private:
	
	std::unordered_map<std::string, table> db;
	
	bool quiet_mode;
	
	
	
	
};



#endif /* command_h */
