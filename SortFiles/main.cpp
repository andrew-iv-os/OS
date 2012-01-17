/* 
 * File:   main.cpp
 * Author: andrew
 * Судя по примеру одногрупников. Решил что мне будет проще нормально переписать на C++ в ООП стиле с использованием стандартных библиотек,
 *  чем на С.
 * Created on 16 Январь 2012 г., 14:00
 */

#include <cstdlib>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <tr1/memory> 
#include <limits.h>
#include <stdexcept>

using namespace std;
using namespace std::tr1;

class NumberCollectionException : exception {
protected:
    string message;

public:

    NumberCollectionException(string msg) throw () {
        message = msg;

    }

    virtual string what() {
        return message;
    };

    virtual ~NumberCollectionException() throw () {
    };
};

class NumberCollectionIOException : NumberCollectionException {
public:

    NumberCollectionIOException(string msg) : NumberCollectionException(msg) {
        return;
    };
    virtual string what() {
        return message;
    };
};

class NumberReader {
private:
    const static int local_buffer_size = 0x1000;
    const static int max_number_size = 0x7fffffff;
    shared_ptr <ifstream> input_stream;    
    char char_buffer[local_buffer_size];

public:
    string error_prefix;

    NumberReader(shared_ptr <ifstream> input_stream) {
        this->input_stream = input_stream;
    }

    shared_ptr< vector<int> > get_numbers() {
        long long cur_number = 0;
        bool is_number_ready = false;
        bool is_number_to_long = false;
        int local_data_size = 0;
        int cur_pointer = 0;

        shared_ptr< vector<int> > result (new vector<int>());
        while (true) {
            if (cur_pointer >= local_data_size) {
                this->input_stream->read(char_buffer, local_buffer_size);
                local_data_size = this->input_stream->gcount();
                if (local_data_size == 0) {
                    if (is_number_ready && cur_number <= max_number_size)
                        result->push_back(cur_number);
                    return result;
                }
                cur_pointer = 0;
            };
            if (char_buffer[cur_pointer] >= '0' && char_buffer[cur_pointer] <= '9') {
                is_number_ready = true;
                cur_number = cur_number * 10 + char_buffer[cur_pointer] - '0';
            } else {
                if (is_number_ready && cur_number <= max_number_size)
                    result->push_back(cur_number);
                is_number_ready = false;
                is_number_to_long = false;

            }
            cur_pointer++;
        }
        return result;
    };
};

void exit_with_error(char * msg, int status = 1) {
    cout << msg;
    exit(status);
}

class NumberCollection {
private:
    vector<int> numbers;
public:

    void load_from_file(char * filename) {        

        shared_ptr <ifstream> _in_stream(new ifstream(filename, ifstream::in));
        if (_in_stream->fail()) throw NumberCollectionIOException(string(string("Can't Open file: ") + filename));
        try {
            NumberReader nums_reader(_in_stream);
            nums_reader.error_prefix = "eee:"; // filename+string(":");
            shared_ptr< vector<int> > new_numbers = nums_reader.get_numbers();            
            numbers.insert(numbers.end(), new_numbers->begin(), new_numbers->end());
            
        } catch (ifstream::failure& exc) {
            string msg = "Problem in reading file :";
            msg += filename;
            throw NumberCollectionIOException(msg);
        } catch (bad_alloc& exc) {
            throw NumberCollectionException(string("Bad memory allocation exception in reading from file: ") + filename);
        } catch (overflow_error& exc) {
            throw NumberCollectionException(string("Overflow in reading from file: ") + filename);
        } catch (exception& exc) {
            throw NumberCollectionException(string("Unknown Exception in reading from file: ") + filename);
        } catch (...) {
            throw NumberCollectionException(string("Unknown Error in reading from file: ") + filename);
        }

    };

    void sort_numbers() {
        try {
            sort(numbers.begin(), numbers.end());
        } catch (bad_alloc& exc) {
            throw NumberCollectionException(string("Bad memory allocation exception in sorting: "));
        } catch (overflow_error& exc) {
            throw NumberCollectionException(string("Overflow  in sorting "));
        } catch (exception& exc) {
            throw NumberCollectionException(string("Unknown Exception  in sorting"));
        } catch (...) {
            throw NumberCollectionException(string("Unknown Error  in sorting"));
        }
    }

    void print_to_stream(ostream &stream) {
        try {
            for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); it++) {
                stream << *it << endl;
            }
        }        catch (...) {
            throw NumberCollectionIOException(string("Error in printing to file"));
        }
    };
};

/*
 * 
 */
int main(int argc, char** argv) {
    NumberCollection num_col;
    ostream *output_stream = &cout;
    bool read_output = false;
    if(argc == 1)
    {
        cerr<<"usage: ./sortfiles [file1] ... [filen] [--output \"filename\"]\n";
        return 0;
    }
    
    for (int argn = 1; argn < argc; argn++)
    {
        try {
            if (strcmp(argv[argn], "--output") == 0) {
                read_output = true;
            } else {
                if (read_output) {
                    output_stream = new ofstream(argv[argn], ofstream::out);
                    if (output_stream->fail()) {
                        exit_with_error("Can't open output file");
                    }
                    read_output = false;
                } else {
                    num_col.load_from_file(argv[argn]);
                }
            }
        }
        catch (NumberCollectionIOException &exc) {
            cerr << exc.what() << endl;
        }
        catch (NumberCollectionException &exc) {
            cerr << exc.what() << endl;
        }        ;
    }
    try {
    num_col.sort_numbers();
    num_col.print_to_stream(*output_stream);
    
    }
            catch (NumberCollectionException &exc) {
            cerr << exc.what() << endl;
     };
    
    /*if (output_stream != &cout)
        delete output_stream;*/
    return 0;
}

