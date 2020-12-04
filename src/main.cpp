extern "C" {
    int yylex(void);
    void print_result(void);
    void read_file(char*);
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        read_file(argv[1]);
        yylex();
        print_result();
    }
    return 0;
}