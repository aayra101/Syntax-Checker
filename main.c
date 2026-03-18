#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES     500
#define MAX_LINE_LEN  300
#define MAX_VARS      100
#define MAX_FUNCS     50

// ── Keywords & Built-ins ────────────────────────────────────────────────────
const char *keywords[] = {
    "int", "float", "char", "double", "long", "short", "unsigned", "signed",
    "void", "return", "if", "else", "while", "for", "do", "switch", "case",
    "break", "continue", "struct", "union", "typedef", "enum", "const",
    "static", "extern", "sizeof", "include", "define", NULL
};

const char *builtins[] = {
    "printf", "scanf", "gets", "puts", "strlen", "strcpy", "strcat",
    "strcmp", "malloc", "calloc", "free", "fopen", "fclose", "fprintf",
    "fscanf", "exit", "abs", "sqrt", "pow", "rand", "srand", NULL
};

// ── Data Types for memory estimation ───────────────────────────────────────
typedef struct {
    char name[50];
    char type[20];
} Variable;

typedef struct {
    char name[50];
    int  is_builtin;
} Function;

// ── Global Counters ─────────────────────────────────────────────────────────
int keyword_count   = 0;
int variable_count  = 0;
int function_count  = 0;
int builtin_count   = 0;
int line_count      = 0;
int open_curly      = 0, close_curly  = 0;
int open_paren      = 0, close_paren  = 0;
int open_square     = 0, close_square = 0;

Variable  variables[MAX_VARS];
Function  functions[MAX_FUNCS];
char      cleaned_lines[MAX_LINES][MAX_LINE_LEN];
int       original_line_nums[MAX_LINES];

// ── Utility: trim whitespace ────────────────────────────────────────────────
void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    memmove(str, start, strlen(start) + 1);
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1]))
        str[--len] = '\0';
}

// ── Remove single-line comments ─────────────────────────────────────────────
void remove_comment(char *line) {
    char *pos = strstr(line, "//");
    if (pos) *pos = '\0';
}

// ── Check if word is a keyword ──────────────────────────────────────────────
int is_keyword(const char *word) {
    for (int i = 0; keywords[i]; i++)
        if (strcmp(word, keywords[i]) == 0) return 1;
    return 0;
}

// ── Check if word is a built-in function ────────────────────────────────────
int is_builtin(const char *word) {
    for (int i = 0; builtins[i]; i++)
        if (strcmp(word, builtins[i]) == 0) return 1;
    return 0;
}

// ── Memory size of a data type ───────────────────────────────────────────────
int type_size(const char *type) {
    if (strcmp(type, "int")    == 0) return 4;
    if (strcmp(type, "float")  == 0) return 4;
    if (strcmp(type, "double") == 0) return 8;
    if (strcmp(type, "char")   == 0) return 1;
    if (strcmp(type, "long")   == 0) return 8;
    if (strcmp(type, "short")  == 0) return 2;
    return 0;
}

// ── Step 1: Read & clean lines ───────────────────────────────────────────────
int read_and_clean(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("ERROR: Cannot open file '%s'\n", filename);
        return -1;
    }

    char raw[MAX_LINE_LEN];
    int  raw_line = 0;
    int  stored   = 0;

    while (fgets(raw, sizeof(raw), fp) && stored < MAX_LINES) {
        raw_line++;
        raw[strcspn(raw, "\n")] = '\0';

        // Skip blank lines
        char tmp[MAX_LINE_LEN];
        strcpy(tmp, raw);
        trim(tmp);
        if (strlen(tmp) == 0) continue;

        // Remove comment
        remove_comment(raw);
        trim(raw);
        if (strlen(raw) == 0) continue;

        strcpy(cleaned_lines[stored], raw);
        original_line_nums[stored] = raw_line;
        stored++;
    }

    fclose(fp);
    line_count = stored;
    return stored;
}

// ── Step 2: Bracket & parenthesis check ─────────────────────────────────────
void check_brackets(FILE *out) {
    for (int i = 0; i < line_count; i++) {
        for (int j = 0; cleaned_lines[i][j]; j++) {
            char c = cleaned_lines[i][j];
            if (c == '{') open_curly++;
            if (c == '}') close_curly++;
            if (c == '(') open_paren++;
            if (c == ')') close_paren++;
            if (c == '[') open_square++;
            if (c == ']') close_square++;
        }
    }

    fprintf(out, "\n----- BRACKET / PARENTHESIS CHECK -----\n");
    if (open_curly != close_curly)
        fprintf(out, "  [WARNING] Unbalanced curly braces   { %d  vs  } %d\n", open_curly, close_curly);
    else
        fprintf(out, "  [OK] Curly braces balanced          { %d  } %d\n", open_curly, close_curly);

    if (open_paren != close_paren)
        fprintf(out, "  [WARNING] Unbalanced parentheses    ( %d  vs  ) %d\n", open_paren, close_paren);
    else
        fprintf(out, "  [OK] Parentheses balanced           ( %d  ) %d\n", open_paren, close_paren);

    if (open_square != close_square)
        fprintf(out, "  [WARNING] Unbalanced square brackets [ %d  vs  ] %d\n", open_square, close_square);
    else
        fprintf(out, "  [OK] Square brackets balanced       [ %d  ] %d\n", open_square, close_square);
}

// ── Step 3: Keyword extraction ───────────────────────────────────────────────
void extract_keywords(FILE *out) {
    fprintf(out, "\n----- KEYWORDS WITH LINE NUMBER -----\n");

    for (int i = 0; i < line_count; i++) {
        char line[MAX_LINE_LEN];
        strcpy(line, cleaned_lines[i]);

        char *token = strtok(line, " \t,;(){}[]<>*&=+-/\"'");
        while (token) {
            if (is_keyword(token)) {
                fprintf(out, "  Line %d [%d]: %s\n",
                    i + 1, original_line_nums[i], token);
                keyword_count++;
            }
            token = strtok(NULL, " \t,;(){}[]<>*&=+-/\"'");
        }
    }
}

// ── Step 4: Built-in function detection ─────────────────────────────────────
void detect_builtins(FILE *out) {
    fprintf(out, "\n----- BUILT-IN FUNCTIONS USED -----\n");

    for (int i = 0; i < line_count; i++) {
        char line[MAX_LINE_LEN];
        strcpy(line, cleaned_lines[i]);

        char *token = strtok(line, " \t,;(){}[]<>*&=+-/\"'");
        while (token) {
            if (is_builtin(token)) {
                fprintf(out, "  Line %d [%d]: %s()\n",
                    i + 1, original_line_nums[i], token);
                if (builtin_count < MAX_FUNCS) {
                    strcpy(functions[builtin_count].name, token);
                    functions[builtin_count].is_builtin = 1;
                }
                builtin_count++;
            }
            token = strtok(NULL, " \t,;(){}[]<>*&=+-/\"'");
        }
    }

    fprintf(out, "\n  Total Built-in Functions: %d\n", builtin_count);
}

// ── Step 5: User-defined function detection ──────────────────────────────────
void detect_functions(FILE *out) {
    fprintf(out, "\n----- USER-DEFINED FUNCTIONS (INCLUDING MAIN) -----\n");

    for (int i = 0; i < line_count; i++) {
        char line[MAX_LINE_LEN];
        strcpy(line, cleaned_lines[i]);

        // Pattern: return_type funcname(
        char word1[50] = "", word2[50] = "";
        if (sscanf(line, "%49s %49s", word1, word2) == 2) {
            // word1 is a type, word2 contains '('
            char *paren = strchr(word2, '(');
            if (paren && is_keyword(word1) && !is_builtin(word2)) {
                char fname[50];
                strncpy(fname, word2, (int)(paren - word2));
                fname[(int)(paren - word2)] = '\0';
                if (strlen(fname) > 0 && !is_keyword(fname) && !is_builtin(fname)) {
                    fprintf(out, "  Line %d [%d]: %s()\n",
                        i + 1, original_line_nums[i], fname);
                    function_count++;
                }
            }
        }
    }

    fprintf(out, "\n  Total Functions (including main): %d\n", function_count);
}

// ── Step 6: Variable detection & memory estimation ───────────────────────────
void detect_variables(FILE *out) {
    fprintf(out, "\n----- VARIABLES WITH COUNTS -----\n");

    const char *var_types[] = {"int", "float", "char", "double", "long", "short", NULL};
    int type_counts[6] = {0};
    int total_memory   = 0;

    for (int i = 0; i < line_count; i++) {
        char line[MAX_LINE_LEN];
        strcpy(line, cleaned_lines[i]);
        trim(line);

        for (int t = 0; var_types[t]; t++) {
            char prefix[30];
            snprintf(prefix, sizeof(prefix), "%s ", var_types[t]);
            if (strncmp(line, prefix, strlen(prefix)) == 0) {
                // Count comma-separated variable names
                char rest[MAX_LINE_LEN];
                strcpy(rest, line + strlen(prefix));

                char *token = strtok(rest, " ,;=()");
                while (token) {
                    if (!is_keyword(token) && !is_builtin(token) &&
                        isalpha((unsigned char)token[0])) {
                        fprintf(out, "  Line %d [%d]: %s %s\n",
                            i + 1, original_line_nums[i], var_types[t], token);
                        type_counts[t]++;
                        variable_count++;
                        total_memory += type_size(var_types[t]);
                    }
                    token = strtok(NULL, " ,;=()");
                }
            }
        }
    }

    fprintf(out, "\n  Total int     : %d\n", type_counts[0]);
    fprintf(out, "  Total float   : %d\n", type_counts[1]);
    fprintf(out, "  Total char    : %d\n", type_counts[2]);
    fprintf(out, "  Total double  : %d\n", type_counts[3]);
    fprintf(out, "  Total long    : %d\n", type_counts[4]);
    fprintf(out, "  Total short   : %d\n", type_counts[5]);
    fprintf(out, "\n----- MEMORY CONSUMED BY VARIABLES -----\n");
    fprintf(out, "  int    : %d bytes\n", type_counts[0] * 4);
    fprintf(out, "  float  : %d bytes\n", type_counts[1] * 4);
    fprintf(out, "  char   : %d bytes\n", type_counts[2] * 1);
    fprintf(out, "  double : %d bytes\n", type_counts[3] * 8);
    fprintf(out, "  long   : %d bytes\n", type_counts[4] * 8);
    fprintf(out, "  short  : %d bytes\n", type_counts[5] * 2);
    fprintf(out, "\n  TOTAL MEMORY USED: %d bytes\n", total_memory);
}

// ── Step 7: Syntax check (printf, scanf, for, while) ─────────────────────────
void syntax_check(FILE *out) {
    fprintf(out, "\n----- SYNTAX CHECK -----\n");
    int errors = 0;

    for (int i = 0; i < line_count; i++) {
        char *line = cleaned_lines[i];

        // Check missing semicolon (simple heuristic)
        int len = strlen(line);
        char last = (len > 0) ? line[len - 1] : '\0';

        // Lines that should end with ;
        int needs_semi = (
            strstr(line, "printf") || strstr(line, "scanf")  ||
            strstr(line, "return") || strstr(line, "gets")   ||
            strstr(line, "puts")
        );
        if (needs_semi && last != ';' && last != '{' && last != '}') {
            fprintf(out, "  [ERROR] Line %d [%d]: Missing semicolon --> %s\n",
                i + 1, original_line_nums[i], line);
            errors++;
        }

        // Check for loop syntax
        if (strncmp(line, "for", 3) == 0) {
            int semi_count = 0;
            for (int j = 0; line[j]; j++)
                if (line[j] == ';') semi_count++;
            if (semi_count < 2) {
                fprintf(out, "  [ERROR] Line %d [%d]: for loop may be malformed --> %s\n",
                    i + 1, original_line_nums[i], line);
                errors++;
            }
        }
    }

    if (errors == 0)
        fprintf(out, "  [OK] No syntax errors detected.\n");

    fprintf(out, "\n----- TOTAL LINES IN PROGRAM: %d -----\n", line_count);
}

// ── Step 8: Print cleaned lines ──────────────────────────────────────────────
void print_lines(FILE *out) {
    fprintf(out, "----- CLEANED SOURCE LINES -----\n");
    for (int i = 0; i < line_count; i++)
        fprintf(out, "  Line %3d [%3d]: %s\n",
            i + 1, original_line_nums[i], cleaned_lines[i]);
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main() {
    char input_file[200];
    char output_file[200] = "output.txt";

    printf("======================================\n");
    printf("   DSA Syntax Checker - MUJ Project   \n");
    printf("       by Aayra Gupta | 2428010132     \n");
    printf("======================================\n\n");

    printf("Enter input file path (e.g. input.txt): ");
    scanf("%199s", input_file);

    printf("\nAnalyzing '%s'...\n\n", input_file);

    int n = read_and_clean(input_file);
    if (n < 0) return 1;

    FILE *out = fopen(output_file, "w");
    if (!out) {
        printf("ERROR: Cannot create output file.\n");
        return 1;
    }

    fprintf(out, "======================================\n");
    fprintf(out, "   DSA SYNTAX CHECKER - OUTPUT REPORT \n");
    fprintf(out, "   File Analyzed: %s\n", input_file);
    fprintf(out, "======================================\n");

    print_lines(out);
    check_brackets(out);
    extract_keywords(out);
    detect_builtins(out);
    detect_functions(out);
    detect_variables(out);
    syntax_check(out);

    fprintf(out, "\n====== SUMMARY ======\n");
    fprintf(out, "  Total Lines     : %d\n", line_count);
    fprintf(out, "  Keywords Found  : %d\n", keyword_count);
    fprintf(out, "  Variables Found : %d\n", variable_count);
    fprintf(out, "  Functions Found : %d\n", function_count);
    fprintf(out, "  Built-ins Used  : %d\n", builtin_count);
    fprintf(out, "====================\n");

    fclose(out);

    // Also print summary to terminal
    printf("Analysis complete!\n");
    printf("------------------\n");
    printf("  Lines     : %d\n", line_count);
    printf("  Keywords  : %d\n", keyword_count);
    printf("  Variables : %d\n", variable_count);
    printf("  Functions : %d\n", function_count);
    printf("  Built-ins : %d\n", builtin_count);
    printf("\nOutput saved to: %s\n", output_file);
    printf("\nPress any key to exit...");
    getchar(); getchar();

    return 0;
}
