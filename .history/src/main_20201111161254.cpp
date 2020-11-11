#include <iostream>
#include <cstdlib>
using namespace std;

int main (int argc, char* argv[]) {
    string cmd = "cat ";
    cmd += argv[1];
    system(cmd.c_str());
    return 0;
}