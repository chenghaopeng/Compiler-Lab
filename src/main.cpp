#include <string>
#include <vector>
#include <iostream>
#include <cstdarg>
#include <iomanip>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "global.h"
using namespace std;

extern "C" {
    void yyread(char*);
    int yyparse(void);
    char* yytext;
    int yydebug;
}

int flag = 1;

vector<AstNode> nodes;

extern "C"
int newAstNode (char* name, int num, ...) {
    va_list args;
    va_start(args, num);
    AstNode node;
    node.name = string(name);
    if (num) {
        int tmp = node.lc = va_arg(args, int);
        node.lineno = nodes[tmp].lineno;
        for (int i = 1; i < num; ++i) {
            nodes[tmp].rc = va_arg(args, int);
            tmp = nodes[tmp].rc;
        }
    }
    else {
        node.lineno = va_arg(args, int);
        node.opt = string(yytext);
    }
    nodes.push_back(node);
    return nodes.size() - 1;
}

void print_number (string number) {
    size_t dot = number.find('.');
    if (dot == string::npos) {
        unsigned int n = 0;
        if (number[0] == '0') {
            if (number.size() == 1) n = 0;
            else if (number[1] == 'x' || number[1] == 'X') {
                for (int i = 2; i < number.size(); ++i) {
                    int t = 0;
                    if (number[i] >= '0' && number[i] <= '9') t = number[i] - '0';
                    if (number[i] >= 'a' && number[i] <= 'z') t = number[i] - 'a' + 10;
                    if (number[i] >= 'A' && number[i] <= 'Z') t = number[i] - 'A' + 10;
                    n = n * 16 + t;
                }
            }
            else {
                for (int i = 1; i < number.size(); ++i) {
                    n = n * 8 + number[i] - '0';
                }
            }
        }
        else {
            for (int i = 0; i < number.size(); ++i) {
                n = n * 10 + number[i] - '0';
            }
        }
        fprintf(stderr, "%u", n);
    }
    else {
        // double f = 0, a = 0, b = 0;
        // int i, j = 0;
        // for (i = 0; number[i] != '.'; ++i) a = a * 10 + number[i] - '0';
        // for (i++; i < number.size() && number[i] != 'e' && number[i] != 'E'; ++i, ++j) b = b * 10 + number[i] - '0';
        // while (j--) b /= 10;
        // f = a + b;
        // if (i < number.size()) {
        //     int n = atoi(number.c_str() + i + 1);
        //     if (n > 0) for (int i = 0; i < n; ++i) f *= 10;
        //     if (n < 0) for (int i = 0; i < -n; ++i) f /= 10;
        // }
        fprintf(stderr, "%f", atof(number.c_str()));
    }
}

void print (int u, int dep) {
    if (u < 0) return;
    if (nodes[u].lineno != -1) {
        for (int i = 0; i < dep; ++i) fprintf(stderr, "  ");
        fprintf(stderr, "%s", nodes[u].name.c_str());
        if (nodes[u].name[1] >= 'a' && nodes[u].name[1] <= 'z') fprintf(stderr, " (%d)", nodes[u].lineno);
        else {
            if (nodes[u].name == "ID" || nodes[u].name == "TYPE") fprintf(stderr, ": %s", nodes[u].opt.c_str());
            else if (nodes[u].name == "INT" || nodes[u].name == "FLOAT") {
                fprintf(stderr, ": ");
                print_number(nodes[u].opt);
            }
        }
        fprintf(stderr, "\n");
    }
    print(nodes[u].lc, dep + 1);
    print(nodes[u].rc, dep);
}

int main (int argc, char* argv[]) {
    srand(time(NULL));
    if (argc > 2) {
        yyread(argv[1]);
        // yydebug = 1;
        yyparse();
        // if (flag || yydebug) print(nodes.size() - 1, 0);
        int root = nodes.size() - 1;
        init(&nodes);
        semantic(root);
        ir(root);
        FILE* fp = fopen(argv[2], "w");
        irPrint(fp);
        fclose(fp);
    }
    return 0;
}