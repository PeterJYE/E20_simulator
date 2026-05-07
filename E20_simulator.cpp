/*
CS-UY 2214
Adapted from Jeff Epstein
Starter code for E20 cache Simulator
simcache.cpp
*/

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <iomanip>
#include <regex>

using namespace std;
//The class Cache
class Cache{
//All the private member variables of  Cache
private:
    string name;//Cache name(level):L1 or L2
    int size;//Cache size
    int associativity;//Cache associativity
    int blocksize;//Cache blocksize
    int rows;// Cache rows, it will be calculated in constructor
    vector<vector<int>> content; // 2D vector that stores tags, the size of it is depends on value above, which will be set in constructor
public:
    //The initializtion of the Cache, name, size, and block size is the i nput,
    //row and content are setted based on these input
    Cache(string name1, int size1, int associativity1, int blocksize1):name(name1),size(size1), 
            associativity(associativity1), blocksize(blocksize1){
        rows = size / blocksize / associativity;//calculation of rows
        content.resize(rows, vector<int>(associativity, -1));//resize the content for each cache, and set every element to -1
    }
    //The print method provided in start code, they are changed to be able to work with my code
    void print_cache_info(){
        cout << "Cache " << name << " has size " << size <<
        ", associativity " << associativity << ", blocksize " << blocksize <<
        ", rows " << rows << endl;
    };
    //The cache method when there is a sw operation, and the input is memory address and pc
    void case_sw(uint16_t address, uint16_t pc){
        uint16_t blockID=address / blocksize;//calculation of blockID for the address
        uint16_t tag=blockID / rows;//calculation of tag for the address
        bool exist=false;// A boolean will be true when there is a hit in the cache, otherwise false
        int row=blockID % rows; // calculation of row for the address
        // The loop that go over everything to check every tag in the row calculated above is equal to the tag of address
        for (int i = 0; i < associativity; i++) {
        //When there is a hit in the row. The value of exist change to true. Erase the tag in the original place, and push back-1.
            if (content[row][i]==tag) {
                exist = true;
                content[row].erase(content[row].begin() + i);
                content[row].push_back(-1);
                //This loop is to go over the row, and find the first -1(empty spot) and change it to tag, and break when it is done.
                for (int j = 0; j < associativity; j++) {
                    if (content[row][j]==-1) {
                        content[row][j]=tag;
                        break;
                    }
                }
                //Break the loop when there is a hit and every change is done
                break;
            }
        }
        //If the exist is still false after first loop is done. It is a Miss.
        if(!exist){
            //To check whether the last spot in the row is -1(empty). When it is not empty, that means the first element will be erased, and push back the tag of this address.
            if(content[row][associativity-1] != -1){
                content[row].erase(content[row].begin());
                content[row].push_back(tag);
            }
            // Case last spot is empty: The loop find the first -1(empty) in the row and store the tag.
            else{
                for (int i = 0; i < associativity; ++i) {
                    if (content[row][i] == -1) {
                        content[row][i] = tag;
                        break;
                        }
                    }
                }
            }
    //The print codes are provided in starter code, and changed to work with my code
     cout << left << setw(8) << name + " " + "SW" <<  right <<
        " pc:" << setw(5) << pc <<
        "\taddr:" << setw(5) << address <<
        "\trow:" << setw(4) << row << endl;
    };
    //The cache method when there is a lw operation, and the input is memory address and pc
    int case_lw(uint16_t address, uint16_t pc){
        //The set up is as same as sw
        uint16_t blockID= address / blocksize;
        uint16_t tag= blockID / rows;
        int exist = 0;
        int row= blockID % rows;
        //row_copy is a copy of the row, to solve edge case. When there is no empty spot in the row and when there is only one element and the tag in the is equal to tag needs to be stored.
        vector<int> row_copy=content[row];
        //This loop is to check everthing in the row to check whether there is a hit in the row or a empty spot in the loop
        for (int i = 0; i < associativity; i++) {
            // When there is a hit, we do the same thing as sw.
            if (content[row][i] == tag) {
                exist = 1;
                content[row].erase(content[row].begin()+i);
                content[row].push_back(-1);
                for (int j= 0; j< associativity; j++) {
                    if (content[row][j] == -1){
                        content[row][j]=tag;
                        break;
                    }
                }
                //The loop break when there is a hit and every change is done
                break;
            }
            //When there is an empty the spot, store tag in that empty spot and break the loop.
            else if (content[row][i] ==-1) {
                content[row][i]=tag;
                break;
            }
        }
        //This is for two edge cases mentioned above. For two cases, we need to erase the first element and store the tag in the first empty spot.
        if (content[row]==row_copy){
            content[row].erase(content[row].begin());
            content[row].push_back(-1);
            for (int j= 0; j< associativity; j++) {
                if (content[row][j] == -1){
                    content[row][j]=tag;
                    break;
                    }
                }
        }
    // The following are print code based on whether it is a hit or miss
    if(exist==1){
        //The print codes are provided in starter code, and changed to work with my code
        cout << left << setw(8) << name + " " + "HIT" <<  right <<
        " pc:" << setw(5) << pc <<
        "\taddr:" << setw(5) << address <<
        "\trow:" << setw(4) << row << endl;
    }
    else{
        //The print codes are provided in starter code, and changed to work with my code
        cout << left << setw(8) << name + " " + "MISS" <<  right <<
        " pc:" << setw(5) << pc <<
        "\taddr:" << setw(5) << address <<
        "\trow:" << setw(4) << row << endl;
    }
    return exist;
    };
    };

/*
    Prints out the correctly-formatted configuration of a cache.

    @param cache_name The name of the cache. "L1" or "L2"

    @param size The total size of the cache, measured in memory cells.
        Excludes metadata

    @param assoc The associativity of the cache. One of [1,2,4,8,16]

    @param blocksize The blocksize of the cache. One of [1,2,4,8,16,32,64])

    @param num_rows The number of rows in the given cache.
*/
void print_cache_config(const string &cache_name, int size, int assoc, int blocksize, int num_rows) {
    cout << "Cache " << cache_name << " has size " << size <<
        ", associativity " << assoc << ", blocksize " << blocksize <<
        ", rows " << num_rows << endl;
}


size_t const static NUM_REGS = 8;
size_t const static MEM_SIZE = 1<<13;
size_t const static REG_SIZE = 1<<16;

/*
    Loads an E20 machine code file into the list
    provided by mem. We assume that mem is
    large enough to hold the values in the machine
    code file.

    @param f Open file to read from
    @param mem Array represetnting memory into which to read program
*/
void load_machine_code(ifstream &f, uint16_t mem[]) {
    regex machine_code_re("^ram\\[(\\d+)\\] = 16'b(\\d+);.*$");
    size_t expectedaddr = 0;
    string line;
    while (getline(f, line)) {
        smatch sm;
        if (!regex_match(line, sm, machine_code_re)) {
            cerr << "Can't parse line: " << line << endl;
            exit(1);
        }
        size_t addr = stoi(sm[1], nullptr, 10);
        unsigned instr = stoi(sm[2], nullptr, 2);
        if (addr != expectedaddr) {
            cerr << "Memory addresses encountered out of sequence: " << addr << endl;
            exit(1);
        }
        if (addr >= MEM_SIZE) {
            cerr << "Program too big for memory" << endl;
            exit(1);
        }
        expectedaddr ++;
        mem[addr] = instr;
    }
}

void E20_simulator_One_Cache(ifstream &f, Cache L1){
    uint16_t mem[MEM_SIZE] = {0};
    load_machine_code(f, mem);
    uint16_t pc = 0;
    uint16_t regs[NUM_REGS] = {0};
    while(true){
        uint16_t pc_check = pc;
        uint16_t instruction = mem[pc%8192];
        int fst3 = instruction >>13;
        int dst = (instruction & 0b0000000001110000)>>4;
        int regA = (instruction & 0b0001110000000000)>>10;
        int regB = (instruction & 0b0000001110000000)>>7;
        uint16_t imm=instruction & 0b0000001111111;
        if((imm>>6)==1){
            imm = imm | 0b1111111110000000;
        }
        uint16_t jp= (instruction & 0b0001111111111111);
        regs[0]=0;
        if(fst3 == 0b010){
                pc=jp-1;
            }
        else if(fst3==0b000){
            int lst4=instruction & 0b0000000000001111;
            if(lst4==0b0000){//add
                regs[dst]=regs[regA]+regs[regB];
            }
            else if(lst4==0b0001){//sub
                regs[dst]=regs[regA]-regs[regB];
            }
            else if(lst4==0b0010){//or
                regs[dst]=regs[regA] | regs[regB];
            }
            else if(lst4==0b0011){//and
                regs[dst]=regs[regA] & regs[regB];
            }
            else if(lst4==0b0100){//slt
                regs[dst]=(regs[regA]<regs[regB])? 1 : 0;
            }
            else if(lst4==0b1000){//jr
                pc=regs[regA]-1;
            }
        }
        else if(fst3==0b111){//slti
            regs[regB]=(regs[regA]<imm)? 1 : 0;
        }
        else if(fst3==0b100){//lw
            //adjustment made
            regs[regB]=mem[(imm+regs[regA]) % 8191];
            uint16_t address=(imm+regs[regA]) % 8191;
            L1.case_lw(address, pc);
        }
        else if(fst3==0b101){//sw
            //adjustment made
            mem[(imm+regs[regA]) % 8191]=regs[regB];
            uint16_t address=(imm+regs[regA]) % 8191;
            L1.case_sw(address, pc);
        }
        else if(fst3==0b001){//addi
            regs[regB]=regs[regA]+imm;
        }
        else if(fst3==0b011){//jal
            regs[7]=pc+1;
            pc=jp-1;
        }
        else if(fst3==0b110){//jeq
            pc=(regs[regA] == regs[regB]) ? pc+imm : pc;
        }
        pc += 1;//pc plus one
        if (pc==pc_check){//break the loop
            break;
        }
  
    }
}
void E20_simulator_Two_Cache(ifstream &f, Cache L1, Cache L2){
    uint16_t mem[MEM_SIZE] = {0};
    load_machine_code(f, mem);
    uint16_t pc = 0;
    uint16_t regs[NUM_REGS] = {0};
    while(true){
        uint16_t pc_check = pc;
        uint16_t instruction = mem[pc%8192];
        int fst3 = instruction >>13;
        int dst = (instruction & 0b0000000001110000)>>4;
        int regA = (instruction & 0b0001110000000000)>>10;
        int regB = (instruction & 0b0000001110000000)>>7;
        uint16_t imm=instruction & 0b0000001111111;
        if((imm>>6)==1){
            imm = imm | 0b1111111110000000;
        }
        uint16_t jp= (instruction & 0b0001111111111111);
        regs[0]=0;
        if(fst3 == 0b010){
                pc=jp-1;
            }
        else if(fst3==0b000){
            int lst4=instruction & 0b0000000000001111;
            if(lst4==0b0000){//add
                regs[dst]=regs[regA]+regs[regB];
            }
            else if(lst4==0b0001){//sub
                regs[dst]=regs[regA]-regs[regB];
            }
            else if(lst4==0b0010){//or
                regs[dst]=regs[regA] | regs[regB];
            }
            else if(lst4==0b0011){//and
                regs[dst]=regs[regA] & regs[regB];
            }
            else if(lst4==0b0100){//slt
                regs[dst]=(regs[regA]<regs[regB])? 1 : 0;
            }
            else if(lst4==0b1000){//jr
                pc=regs[regA]-1;
            }
        }
        else if(fst3==0b111){//slti
            regs[regB]=(regs[regA]<imm)? 1 : 0;
        }
        else if(fst3==0b100){//lw
            //adjustment made
            regs[regB]=mem[(imm+regs[regA]) % 8191];
            uint16_t address=(imm+regs[regA]) % 8191;
            //To check if it is MISS or HIT in L1, load L2 when it is a MISS
            if(L1.case_lw(address, pc)==0){
                L2.case_lw(address, pc);
            }
        }
        else if(fst3==0b101){//sw
            mem[(imm+regs[regA]) % 8191]=regs[regB];
            uint16_t address=(imm+regs[regA]) % 8191;
            L1.case_sw(address, pc);
            L2.case_sw(address, pc);
        }
        else if(fst3==0b001){//addi
            regs[regB]=regs[regA]+imm;
        }
        else if(fst3==0b011){//jal
            regs[7]=pc+1;
            pc=jp-1;
        }
        else if(fst3==0b110){//jeq
            pc=(regs[regA] == regs[regB]) ? pc+imm : pc;
        }
        pc += 1;//pc plus one
        if (pc==pc_check){//break the loop
            break;
        }
  
    }
}
/**
    Main function
    Takes command-line args as documented below
*/
int main(int argc, char *argv[]) {
    /*
        Parse the command-line arguments
    */
    char *filename = nullptr;
    bool do_help = false;
    bool arg_error = false;
    string cache_config;
    for (int i=1; i<argc; i++) {
        string arg(argv[i]);
        if (arg.rfind("-",0)==0) {
            if (arg== "-h" || arg == "--help")
                do_help = true;
            else if (arg=="--cache") {
                i++;
                if (i>=argc)
                    arg_error = true;
                else
                    cache_config = argv[i];
            }
            else
                arg_error = true;
        } else {
            if (filename == nullptr)
                filename = argv[i];
            else
                arg_error = true;
        }
    }
    /* Display error message if appropriate */
    if (arg_error || do_help || filename == nullptr) {
        cerr << "usage " << argv[0] << " [-h] [--cache CACHE] filename" << endl << endl;
        cerr << "Simulate E20 cache" << endl << endl;
        cerr << "positional arguments:" << endl;
        cerr << "  filename    The file containing machine code, typically with .bin suffix" << endl<<endl;
        cerr << "optional arguments:"<<endl;
        cerr << "  -h, --help  show this help message and exit"<<endl;
        cerr << "  --cache CACHE  Cache configuration: size,associativity,blocksize (for one"<<endl;
        cerr << "                 cache) or"<<endl;
        cerr << "                 size,associativity,blocksize,size,associativity,blocksize"<<endl;
        cerr << "                 (for two caches)"<<endl;
        return 1;
    }

    /* parse cache config */
    if (cache_config.size() > 0) {
        vector<int> parts;
        size_t pos;
        size_t lastpos = 0;
        while ((pos = cache_config.find(",", lastpos)) != string::npos) {
            parts.push_back(stoi(cache_config.substr(lastpos,pos)));
            lastpos = pos + 1;
        }
        parts.push_back(stoi(cache_config.substr(lastpos)));
        ifstream f(filename);
        if (!f.is_open()) {
            cerr << "Can't open file "<<filename<<endl;
            return 1;
        }
        if (parts.size() == 3) {
            int L1size = parts[0];
            int L1assoc = parts[1];
            int L1blocksize = parts[2];
            Cache L1("L1", L1size, L1assoc, L1blocksize);
            L1.print_cache_info();
            E20_simulator_One_Cache(f, L1);
            // TODO: execute E20 program and simulate one cache here
        } else if (parts.size() == 6) {
            int L1size = parts[0];
            int L1assoc = parts[1];
            int L1blocksize = parts[2];
            int L2size = parts[3];
            int L2assoc = parts[4];
            int L2blocksize = parts[5];
            // TODO: execute E20 program and simulate two caches here
            Cache L1("L1", L1size, L1assoc, L1blocksize);
            Cache L2("L2", L2size, L2assoc, L2blocksize);
            L1.print_cache_info();
            L2.print_cache_info();
            E20_simulator_Two_Cache(f,L1, L2);
        } else {
            cerr << "Invalid cache config"  << endl;
            return 1;
        }
    }

    return 0;
}
//ra0Eequ6ucie6Jei0koh6phishohm9
