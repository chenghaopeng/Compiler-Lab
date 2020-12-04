extern "C" {
    int yyparse(void);
}

int main (int argc, char* argv[]) {
    yyparse();
    return 0;
}