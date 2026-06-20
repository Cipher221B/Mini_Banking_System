#include "file_handle.h"
#include <fstream>
using namespace std;

string File_Handle::get_query(const string& filename)
{
    string temp_query, query;
    ifstream read(filename);

    if(read.is_open())
    {
        while(getline(read, temp_query))
        {
            query += temp_query + '\n';

        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'" << filename << "'. Read File Failed!!!\n";
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore();  // clear buffer
        cin.get();     // wait for key press
        read.close();
        throw Open_File_Error("Can't get query from file: '" + filename + "'.", __FILE__, __LINE__);
    }
    read.close();
    return query;
}
