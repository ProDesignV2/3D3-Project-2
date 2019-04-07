#include <fstream>
#include <unistd.h>
#include <string>

int main(int argc, char *argv[]){
    // Sleep for specified time
    sleep(std::stoi(argv[1]));
    return 0;
}