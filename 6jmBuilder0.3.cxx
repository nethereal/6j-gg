#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <fstream> //file_exists_and_can_be_read_from
#include <unistd.h>
#include <sys/stat.h>
#include <sstream>

const int OSMONELEN = 128; //message character length
const int GENESETLEN = 256;
const int BYTESPERGENE = 8;
const int BYTESPERCHROMO = GENESETLEN * BYTESPERGENE;
#define KEYFILEEXT = ".6jk";

inline bool feaw (const std::string& name) { return ( access( name.c_str(), W_OK ) != -1 ); }

int toASCII(char ch) {
    return ch;
}

std::string toup(std::string input) {
	std::string ret;
	std::locale loc;
	for (std::string::size_type i=0; i<input.length(); ++i)
		ret += std::toupper(input[i],loc);
	return ret;
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

unsigned char* getGeneFromArray(unsigned char* geneArray, uint64_t pos) {
	unsigned char* getgene = new unsigned char[BYTESPERGENE];
	for (uint64_t icnt = 0; icnt < (uint)BYTESPERGENE; icnt++) {
		getgene[icnt] = geneArray[(pos + icnt)];
	}
	return getgene;
} 

int main(int argc, char **argv)
{
	// init vars
	
	unsigned char* loadGene = new unsigned char[BYTESPERGENE];

	// arg count should be 3(inc executable), 2: path/to/keyfile.6jk , 3: "message encased in dbl quotes"
	if (argc != 3) {
		std::cout << "ERROR: wrong number of arguments!" << std::endl; 
		return 3; 
	}
	
	// verify argv[2] (message to be encrypted, surrounded by quotes), max 128 char
	std::string arg2 = argv[2];
	int arg2len = arg2.length();
	if (arg2len > 128) {
		std::cout << "ERROR: message to be encrypted is too long! " << std::endl;
		return 7;
	}

	// verify argv[1] 
	std::string arg1 = argv[1];
	if (arg1.length() < 5) {
		std::cout << "ERROR: filename argument not long enough!" << std::endl;
		return 4;
	}

	// verify arg1 has file extension
	std::string endfile = arg1.substr(arg1.length() - 4);
	if (toup(endfile) != ".6JK") {
		std::cout << "ERROR: argument (filename) incorrectly terminated!" << std::endl;
		return 5;
	}

	// verify existence and read/writeability of file
	if(!feaw(arg1)) {
		std::cout << "ERROR: keyfile supplied does not exist or is not writeable!" << std::endl;
		return 6;
	}
	
	// reserve memory!!!
	int genomeLength = filesize(argv[1]);
	int nextgenesLength = (uint64_t)BYTESPERCHROMO;
	int flagsLength = (uint64_t)GENESETLEN;
	unsigned char* genome;
	unsigned char* nextgenes;
	unsigned char* flags;
	std::cout << "Reserving memory for objects..." << std::endl;
	try {	
		genome = new unsigned char[genomeLength];
		nextgenes = new unsigned char[nextgenesLength];
		flags = new unsigned char[flagsLength];
	}
	catch (std::bad_alloc& ba) {
		std::cerr << "bad_alloc caught: " << ba.what() << " gL: " << genomeLength << std::endl;
		std::cout << "Error: Memory addressing issue - keyfile too large for available memory!" << std::endl;
		return 8;
	}
	std::cout << "Memory reservation OK ..." << std::endl;	

	// read keyfile to memory
	std::ifstream inkeyfile(argv[1], std::ios::binary);
	std::string contents((std::istreambuf_iterator<char>(inkeyfile)), std::istreambuf_iterator<char>());
	genome = (unsigned char*)contents.c_str();
		
	// test output of first gene
	std::cout << "First gene of genome: ";
	loadGene = getGeneFromArray(genome, 0);
	for (int iz = 0; iz < BYTESPERGENE; iz++) {
		std::cout << (int)loadGene[iz] << " ";
	}
	std::cout << std::endl;
	
	int counter = 0;
	
	for (int gcnt = 0; gcnt <= (BYTESPERCHROMO/8); gcnt++) {
		std::cout << "test " << counter << " of " << BYTESPERCHROMO/8 << " ";
		loadGene = getGeneFromArray(genome, gcnt);
		std::cout << "Current char: " << (char)(gcnt/8) << "  Val: " << std::endl;
		counter++;
		std::cout << static_cast<char> (0x5e) << " " << (char)94 << " " << '\x5e';
		//output the hex fals here:
		for (uint8_t tempcnt = 0; tempcnt < BYTESPERGENE; tempcnt++) {
			
		} 
	}
	
	/*
	std::stringstream stream;
	uint64_t icurr = 0;
	uint64_t geneval = 0;
	std::string outhex = "";
	for (int ichar = 0; ichar < GENESETLEN; ichar++) {
		do {
			loadGene = getGeneFromArray(genome, (ichar * BYTESPERGENE));
			for (int ic = 0; ic < BYTESPERGENE; ic++) {
				stream << std::hex << (uint8_t)loadgene[ic];
			}
			std::cout << "Current slot: " << ichar << "  Val: 0x" << stream << std::endl;
			stream.str(std::string());
			geneval = (uint64_t)loadGene;
		} while (geneval == UINT_MAX || geneval == 0);
		//if(ichar <= 127) { std::cout << "Value for char " << ichar << "[here]"; }
		//else { std::cout << "Value for char " << ichar << "[ext" << (ichar-127) << "]"; }
		//std::cout << " is: " << (unsigned) geneval << std::endl;
	} */
	
	return 0;
}
