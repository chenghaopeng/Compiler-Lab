extern "C" {
    void yyread(char*);
    int yyparse(void);
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
    }
    return 0;
}