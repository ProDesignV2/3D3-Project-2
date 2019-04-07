#include <fstream>
#include <unistd.h>
#include <string>

#define FILE_NAME "testing.txt"

int main(int argc, char *argv[]){
    // Get command-line argument
    std::string temp(argv[1]);
    temp.insert(0, "ROUTER [");
    temp.push_back(']');
    // Sleep for specified time
    sleep(std::stoi(argv[1]));
    // Open file
    std::ofstream textFile;
    textFile.open(FILE_NAME, std::ios_base::app);
    // Write line to file 
    textFile << temp << std::endl;
    // Close file
    textFile.close();
    return 0;
}