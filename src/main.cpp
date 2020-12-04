#include <cstdio>
#include <cstdlib>
using namespace std;

extern FILE* yyin;
extern "C" int yylex(void);
extern int flag;
extern char* buf;

int main (int argc, char* argv[]) {
    yylex();
    if (flag) fprintf(stderr, buf);
    return 0;
}