# silly sql
This project involves creating a program called "silly" that emulates a basic relational database with a subset of a standard query language. The program will accept various command line arguments and perform operations such as creating tables, inserting data into tables, deleting data from tables, generating search indices, printing rows from tables, joining tables, removing tables, and exiting the program.

The program uses a relational database model where data is stored in tables with columns and rows. Columns represent attributes or descriptors for the data, while rows represent individual entries or records. The program supports commands that allow for the retrieval of specific information from the database using a simplified and modified version of SQL.

The specific functionalities and descriptions of the program's commands are as follows:

CREATE: Adds a new table to the database with specified column names and types.

Syntax: CREATE <tablename> <N> <coltype1> <coltype2> ... <coltypeN> <colname1> <colname2> ... <colnameN>
Output: Prints the confirmation of table creation.
INSERT INTO: Inserts new data into a table.

Syntax: INSERT INTO <tablename> <N> ROWS <value11> <value12> ... <value1M> <value21> <value22> ... <value2M> ... <valueN1> <valueN2> ... <valueNM>
Output: Prints the number of rows inserted.
DELETE FROM: Deletes specific data from a table.

Syntax: DELETE FROM <tablename> WHERE <colname> <OP> <value>
Output: Prints the number of rows deleted.
GENERATE INDEX: Creates a search index on a specified column of a table.

Syntax: GENERATE FOR <tablename> <indextype> INDEX ON <colname>
Output: Prints the confirmation of index creation.
PRINT: Prints specified rows from a table.

Syntax: PRINT FROM <tablename> <N> <print_colname1> <print_colname2> ... <print_colnameN> [WHERE <colname> <OP> <value> | ALL]
Output: Prints the specified columns and their corresponding values for the matching rows.
JOIN: Joins two tables and prints the result.

Syntax: JOIN <tablename1> AND <tablename2> WHERE <colname1> = <colname2> AND PRINT <N> <print_colname1> <1|2> <print_colname2> <1|2> ... <print_colnameN> <1|2>
Output: Prints the specified columns and their corresponding values for the joined rows.
REMOVE: Removes an existing table from the database.

Syntax: REMOVE <tablename>
Output: Prints the confirmation of table deletion.
QUIT: Exits the program.

Syntax: QUIT
Output: Prints a goodbye message.
#: Comment command that is ignored.

The program also supports two optional command line arguments: -h/--help and -q/--quiet. The -h/--help argument prints a helpful message about how to use the program and then exits. The -q/--quiet argument runs the program in quiet mode, where only numerical summaries of the rows affected by certain commands are printed.

The program performs error checking for various scenarios, such as existing table names, invalid table/column names, unrecognized commands, etc. Error messages are printed to stdout.

The project's learning goals include selecting appropriate data structures, evaluating runtime and storage tradeoffs, designing algorithms, and evaluating different representations of data.

Overall, the program provides a basic relational database functionality with a simplified query language.
