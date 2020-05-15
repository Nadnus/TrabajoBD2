#include <math.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

template <class T>
class static_hash
{
private:
    bool is_static;

    long records = 0;
    int depth = 0;

public:
    ~static_hash(){};

    void read_tuple(std::fstream *my_file, T &buffer)
    {
        (*my_file).read((char *)&buffer, sizeof(T));
    }
    void insert_static(T tuple)
    {
        long my_key = tuple.key;
        long bucket_number = my_key % depth;
        std::ofstream my_file(std::to_string(bucket_number) + ".txt", std::ios::app);
        //my_file::seekg(std::ios_base::end);
        my_file.write((char *)&tuple, sizeof(T));
    }
    void insert(T tuple)
    {
        insert_static(tuple);
    }

    static_hash(long TOTAL_FILES)
    {
        chdir("buckets");

        depth = log(TOTAL_FILES);
        for (int i = 0; i < depth; i++)
        {
            std::ofstream my_file(std::to_string(i) + ".txt", std::ios::app);
            my_file.close();
        }
    }
    bool search(long my_key, T *buffer)
    {
        long bucket_number = my_key % depth;
        std::fstream my_file(std::to_string(bucket_number) + ".txt");
        while (!my_file.eof())
        {
            read_tuple(&(my_file), (*buffer));
            if ((*buffer).key == my_key)
                return true;
        }
        return false;
    }
};
