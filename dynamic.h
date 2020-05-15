#include <math.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <bitset>
#include <vector>
#define elementos 32
#define max_tuples 2
//TODO: max depth
template <class T>
class dynamic_hash
{
public:
    class prefix_file;
    typedef std::vector<prefix_file *> prefix_container;
    prefix_container prefixes;
    T buffer;
    dynamic_hash()
    {
        auto prefix_file0 = new prefix_file("0");
        auto prefix_file1 = new prefix_file("1");
        prefixes.push_back(prefix_file0);
        prefixes.push_back(prefix_file1);
    };
    ~dynamic_hash(){};


    class prefix_file
    {
        //current depth es el numero de bits, count el numero de elementos para saber cuando overflowear

    public:
        int current_depth;
        long count = 0;
        std::string bits;
        prefix_file(std::string _bits)
        {

            bits = _bits;
            current_depth = _bits.length();
            count = 0;
            std::fstream file(bits, std::ios::app);
            file.close();
        }
        ~prefix_file(){}
        void read_tuple(std::fstream *my_file, T &buffer)
        {
            (*my_file).read((char *)&buffer, sizeof(T));
        }

        void insert_to_bucket(T tuple, int bucket_index, dynamic_hash *parent)
        {
            count++;

            std::ofstream file(bits, std::ios::app);
            file.write((char *)&tuple, sizeof(T));
            if (count >= max_tuples)
            {
                parent->split(bucket_index);
            }
        }
        bool search_in_bucket(long my_key, T * tuple){

            std::fstream my_file(bits);
            while (!my_file.eof()){
                read_tuple(&(my_file), (*tuple));
                if(tuple->key == my_key)return true;
            } return false;
        }
    };
    void split(int index)
    {
        prefix_file *my_prefix_file = prefixes[index];
        std::string zero = "0";
        std::string one = "1";
        std::string prefix0 = zero.append(my_prefix_file->bits);
        std::string prefix1 = one.append(my_prefix_file->bits);
        auto prefix_file0 = new prefix_file(prefix0);
        auto prefix_file1 = new prefix_file(prefix1);
        std::fstream file(my_prefix_file->bits);

        while (!file.eof())
        {
            file.read((char *)&buffer, sizeof(T));
            auto bitsn = std::bitset<elementos>(buffer.key);
            std::string bits_str = bitsn.to_string();
            //resto el uno extra porque usa el current len del pre split
            //TODO
            bits_str = bits_str.substr(bits_str.length() - my_prefix_file->current_depth -1, bits_str.length() - 1);
            if (bits_str == prefix0)
            {
                std::fstream split_file(prefix0, std::ios::app);
                split_file.write((char *)&buffer, sizeof(T));
            }
            else if (bits_str == prefix1)
            {
                std::fstream split_file(prefix1, std::ios::app);
                split_file.write((char *)&buffer, sizeof(T));
            }
        }

        //TODO: the delete and remove. this has a huge memleak
        const char * dump =(my_prefix_file->bits).c_str();
        remove(dump);
        prefixes[index] = prefix_file0;
        prefixes.insert(prefixes.begin() + 1 + index, prefix_file1);
        delete (my_prefix_file);
    }
    void insert_to_hash(T tuple)
    {
        long key = tuple.key;
        std::bitset<elementos> bit_num = std::bitset<elementos>(key);

        for (int i = 0; i < prefixes.size(); i++)
        {
            std::string bit_str = bit_num.to_string();
            int pos1 =bit_str.length()  - prefixes[i]->current_depth ;
            int pos2 =bit_str.length() - 1;
            bit_str = bit_str.substr(pos1,pos2);
            if (bit_str == prefixes[i]->bits)
            {
                prefixes[i]->insert_to_bucket(tuple, i, this);
                break;
            }
        }
    }
    bool search_in_hash(long key, T * buffer){
        std::bitset<elementos> bit_num = std::bitset<elementos>(key);

        for (int i = 0; i < prefixes.size(); i++)
        {
            std::string bit_str = bit_num.to_string();
            int pos1 =bit_str.length()  - prefixes[i]->current_depth ;
            int pos2 =bit_str.length() - 1;
            bit_str = bit_str.substr(pos1,pos2);
            if (bit_str == prefixes[i]->bits)
            {
                return (prefixes[i]->search_in_bucket(key,buffer));

            }
        }
    }

};
