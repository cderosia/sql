//  Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  table.h
//  p3-sql
//
//  Created by Carter DeRosia on 3/9/23.
//

#ifndef table_h
#define table_h

#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include "TableEntry.h"
#include <unordered_map>
#include <map>


class LessThan {
public:
	bool operator()(const TableEntry& left, const TableEntry& right) {
		return left < right;
	}
};

class GreaterThan {
public:
	bool operator()(const TableEntry& left, const TableEntry& right) {
		return left > right;
	}
};

class EqualTo {
public:
	bool operator()(const TableEntry& left, const TableEntry& right) {
		return left == right;
	}
};




class table {
	
public:
	
	table() : idx_col_idx(999999), num_cols(0), num_rows(0) {}
	
	explicit table(int num_of_cols) : idx_col_idx(999999), num_cols(num_of_cols), num_rows(0) {}
	
	void set_num_cols(int i) {
		num_cols = i;
	}
	
	
	int get_num_rows() {
		return num_rows;
	}
	
	
	
	
	TableEntry& get_col_name(int i) {
		
		return data[0][size_t(i)];
	}
	
	std::string get_col_type(int i) {
		return col_types[size_t(i)];
	}
	
	
	void push_col_types(std::string s) {
		col_types.push_back(s);
	}
	
	void resize_data(size_t i) {
		data.resize(i);
	}
	
	
	void push_to_col_names(std::string s) {
		
		col_names.push_back(s);
	}
	
	
	bool is_valid_col(std::string s) {
		
		//std::cout << "col names size" << col_names.size() << std::endl;
		
		for (size_t i = 0; i < col_names.size(); i++) {
			if (col_names[i] == s) {
				return true;
			}
		}
		return false;
	}
	
	
	
	size_t get_col_index(std::string s) {
		for (size_t i = 0; i < col_names.size(); i++) {
			if (col_names[i] == s) {
				return i;
			}
		}
		return 99999;
	}
	
	TableEntry& get_data(int i, int j) {
		
		return data[size_t(i)][size_t(j)];
	}
	
	
	
	
	
	void add_row(const std::vector<TableEntry>& row) {
		
		
		for (size_t i = 0; i < row.size(); i++) {
			if (bst_exists && idx_col_idx == i) {
				bst[row[i]].emplace_back(static_cast<unsigned long>(num_rows));
			} else if (hash_exists && idx_col_idx == i) {
				hash[row[i]].emplace_back(static_cast<unsigned long>(num_rows));
			}
			// add the current entry to the vector of entries for the current row
			data[size_t(num_rows)].emplace_back(row[i]);
		}
		
		++num_rows;
		
		// Regenerate indexes
		if (bst_exists) {
			generate_bst_index(idx_col_name);
		} else if (hash_exists) {
			generate_hash_index(idx_col_name);
		}
	}
	
	
	
	void add_rows(int num_r) {
		
		for (size_t i = 0; i < size_t(num_r); i++) {
			data[i].reserve(size_t(num_cols));
			for (int j = 0; j < static_cast<int>(num_cols); j++) {
				if (get_col_type(j) == "string") {
					
					std::string value;
					std::cin >> value;
					
					data[size_t(num_rows)].emplace_back(value);
				}
				else if (get_col_type(j) == "int") {
					int value;
					std::cin >> value;
					
					data[size_t(num_rows)].emplace_back(value);
				}
				else if (get_col_type(j) == "bool") {
					std::string value;
					std::cin >> value;
					
					data[size_t(num_rows)].emplace_back(value);
					
					
					
				}
				else {
					double value;
					std::cin >> value;
					
					data[size_t(num_rows)].emplace_back(value);
				}
			}
			num_rows++;
		}
		
		
		if (bst_exists) {
			generate_bst_index(idx_col_name);
		} else if (hash_exists) {
			generate_hash_index(idx_col_name);
		}
		
		
	}
	
	
	
	int print_all(const std::vector<std::string>& col_vec, bool qm) {
		
		
		
		if (!qm) {
			for (size_t i = 1; i < size_t(get_num_rows()); i++) {
				for (const auto& col : col_vec) {
					size_t col_idx = get_col_index(col);
					if (col_idx < data[i].size()) {
						
						TableEntry te(data[i][col_idx]);
						std::cout << te << " ";
					}
				}
				std::cout << "\n";
			}
			
			
			
			
		}
		
		
		return num_rows - 1;
	}
	
	int print_where(const std::vector<std::string>& col_vec, std::string col_name, char op, TableEntry value, bool qm) {
		size_t col_index = get_col_index(col_name);
		int row_num = 0;
		int count = 0;
		
		if (bst_exists && idx_col_name == col_name) {
			// Get the column index for each column in col_vec
			std::vector<size_t> col_indices;
			for (const auto& col : col_vec) {
				col_indices.push_back(get_col_index(col));
			}
			// Loop through the rows in the order of the bst map
			for (const auto& pair : bst) {
				const auto& entry = pair.first;
				const auto& row = pair.second;
				bool match = false;
				if (op == '<') {
					LessThan cmp;
					match = cmp(entry, TableEntry(value));
				} else if (op == '>') {
					GreaterThan cmp;
					match = cmp(entry, TableEntry(value));
				} else if (op == '=') {
					EqualTo cmp;
					match = cmp(entry, TableEntry(value));
				}
				if (match) {
					for (const auto& row_idx : row) {
						const auto& row = data[row_idx];
						const TableEntry& entry(row[col_index]);
						bool match = false;
						if (op == '<') {
							LessThan cmp;
							match = cmp(entry, TableEntry(value));
						} else if (op == '>') {
							GreaterThan cmp;
							match = cmp(entry, TableEntry(value));
						} else if (op == '=') {
							EqualTo cmp;
							match = cmp(entry, TableEntry(value));
						}
						if (match) {
							count++;
							if (!qm) {
								for (const auto& col_idx : col_indices) {
									if (col_idx < row.size()) {
										std::cout << row[col_idx] << " ";
									}
								}
								std::cout << "\n";
							}
						}
					}
				}
			}
		} else {
			for (const auto& row : data) {
				if (row_num != 0 && size_t(col_index) < row.size()) {
					const TableEntry& entry(row[size_t(col_index)]);
					bool match = false;
					if (op == '<') {
						LessThan cmp;
						match = cmp(entry, TableEntry(value));
					} else if (op == '>') {
						GreaterThan cmp;
						match = cmp(entry, TableEntry(value));
					} else if (op == '=') {
						EqualTo cmp;
						match = cmp(entry, TableEntry(value));
					}
					if (match) {
						count++; // Increment count by 1 for each row that matches the condition
						if (!qm) {
							for (const auto& col : col_vec) {
								size_t col_idx = get_col_index(col);
								if (size_t(col_idx) < row.size()) {
									std::cout << row[size_t(col_idx)] << " ";
								}
							}
							std::cout << "\n";
						}
					}
				}
				row_num++;
			}
		}
		return count;
	}
	
	
	
	
	
	int delete_rows(std::string col_name, char op, TableEntry value) {
		int count = 0;
		size_t col_index = get_col_index(col_name);
		
		auto row_predicate = [&](const std::vector<TableEntry>& row) {
			if (col_index >= row.size()) {
				return false;
			}
			const TableEntry& entry(row.at(col_index));
			bool match = false;
			if (op == '<') {
				LessThan cmp;
				match = cmp(entry, TableEntry(value));
			} else if (op == '>') {
				GreaterThan cmp;
				match = cmp(entry, TableEntry(value));
			} else if (op == '=') {
				EqualTo cmp;
				match = cmp(entry, TableEntry(value));
			}
			return match;
		};
		
		auto new_end = std::remove_if(data.begin() + 1, data.end(), row_predicate);
		count = static_cast<int>(std::distance(new_end, data.end()));
		data.erase(new_end, data.end());
		num_rows -= count;
		
		
		
		if (bst_exists) {
			generate_bst_index(idx_col_name);
		}
		else if (hash_exists) {
			generate_hash_index(idx_col_name);
		}
		
		return count;
	}
	
	
	
	
	
	void generate_hash_index(std::string colname) {
		hash_exists = true;
		bst_exists = false;
		size_t idx = size_t(get_col_index(colname));
		hash.clear();
		bst.clear();
		for (size_t i = 1; i < size_t(num_rows); ++i) {
			TableEntry key(data[i][idx]);
			hash[key].emplace_back(i);
		}
		idx_col_name = colname;
		idx_col_idx = idx;
		
	}
	
	void generate_bst_index(std::string colname) {
		bst_exists = true;
		hash_exists = false;
		size_t idx = size_t(get_col_index(colname));
		bst.clear();
		hash.clear();
		for (size_t i = 1; i < size_t(num_rows); ++i) {
			TableEntry key(data[i][idx]);
			bst[key].emplace_back(i);
			//std::cout << key << std::endl;
		}
		//std::cout << "bst size, should be 4 not 5 is actually: " << bst.size() << std::endl;
		idx_col_name = colname;
		idx_col_idx = idx;
		
	}
	
	
	
	
private:
	
	bool bst_exists = false;
	bool hash_exists = false;
	
	size_t idx_col_idx;
	
	int num_cols;
	int num_rows;
	
	std::string idx_col_name;
	
	
	
	//std::unordered_map<std::string, int> col_index;
	
	std::vector<std::string> col_types;
	std::vector<std::string> col_names;
	
	
	std::unordered_map<TableEntry, std::vector<size_t>> hash;
	std::map<TableEntry, std::vector<size_t>> bst;
	
	
	
	
	
	
	std::vector<std::vector<TableEntry>> data;
	
};



#endif /* table_h */
