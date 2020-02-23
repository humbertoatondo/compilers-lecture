#include <stdio.h>

#define SIZE 25000

struct Data
{
    int char_value;
    int line;
    int indentation;
};

FILE *fp;
int c, spaces, line;
int spaces_flag, comments_flag, multiline_comments_flag;
int counter = 0;

char reserved[8] = {'{', '}', '(', ')' , '[', ']', '\'', '\"'};

struct Data received[SIZE];

int lines[SIZE];

char get_missing_value(char val);
void find_missing();
void check_balance();
int cmp(char val1, char val2);
void search_for_value(char val, int s, int l);
int contains(char val, char *arr, int start, int end);
void print_array(struct Data *arr, int end);

    int main(int argc, char *argv[])
{
    // Check if the correct number of parameters are passed.
    if (argc != 2)
    {
        printf("Error: 1 argument is required, %d were passed.\n", argc - 1);
        return -1;
    }
    // Open file to read.
    fp = fopen(argv[1], "r");
    // Init variables
    line = 0;
    spaces_flag = 0;
    comments_flag = 0;
    multiline_comments_flag = 0;
    // Read file char by char.
    char l[250];
    while ((fgets(l, sizeof(l), fp)))
    {
        for (int i = 0; l[i] != '\0'; i++)
        {
            if (l[i] == '/' && l[i+1] == '/')
                break;
            else if (l[i] == '/' && l[i + 1] == '*')
                multiline_comments_flag = 1;
            else if (l[i] == '*' && l[i + 1] == '/')
                multiline_comments_flag = 0;
            else if (l[i] == ' ' && !spaces_flag)
                    spaces++;
            else
                spaces_flag = 1;

            if (l[i - 1] != '\\'  && !comments_flag && !multiline_comments_flag)
                search_for_value(l[i], line, spaces);
        }
        lines[line] = spaces;
        spaces = 0;
        spaces_flag = 0;
        comments_flag = 0;
        line++;
    }
    check_balance();
    find_missing(received, counter);
    
}

void find_missing()
{
    int errors = 0;
    for (int i = 0; i < counter; i++)
    {
        if (contains(received[i].char_value, reserved, 2, 8))
        {
            printf("There is a missing %c error in this line: %d\n", get_missing_value(received[i].char_value), received[i].line + 1);
            errors++;
        }
        else if (contains(received[i].char_value, reserved, 0, 1))
        {
            for (int j = received[i].line + 1; j <= line; j++)
            {
                if (received[i].indentation == lines[j])
                {
                    printf("There is a missing %c error in this line: %d\n", get_missing_value(received[i].char_value), j+1);
                    errors++;
                    break;
                }
            }
        }
        else if (contains(received[i].char_value, reserved, 1, 2))
        {
            for (int j = received[i].line - 1; j >= 0; j--)
            {
                if (received[i].indentation == lines[j])
                {
                    printf("There is a missing %c error in this line: %d\n", get_missing_value(received[i].char_value), j + 1);
                    errors++;
                    break;
                }
                    
            }
        }
    }
    if (!errors)
        printf("There is no errors.\n");
}

char get_missing_value(char val)
{
    switch (val)
    {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        case '\"': return '\"';
        case '\'': return '\'';
        default: return ' ';
    }
}

// Remove all balanced characters.
void check_balance()
{
    for (int i = 0; i < counter - 1; i++)
    {
        for (int j = i + 1; j < counter; j++)
        {
            if (cmp(received[i].char_value, received[j].char_value) && received[i].indentation == received[j].indentation)
            {
                if (received[i].char_value == '\"' || received[i].char_value == '\'')
                    while (i + 1 <= j - 1)
                    {
                        received[i].char_value = 0;
                        received[j].char_value = 0;
                        i++;
                        j--;
                    }
                // Remove balanced characters.
                received[i].char_value = 0;
                received[j].char_value = 0;
            }
        }
    }
}

// Compare (, [, {, " and ' with their corresponding counterparts.
int cmp(char val1, char val2)
{
    if (val1 == '(' && val2 == ')') return 1;
    else if (val1 == '[' && val2 == ']') return 1;
    else if (val1 == '{' && val2 == '}') return 1;
    else if (val1 == '\"' && val2 == '\"') return 1;
    else if (val1 == '\'' && val2 == '\'') return 1;
    return 0;
}

// Look for reserved values.
void search_for_value(char val, int l, int s)
{
    struct Data d = {val, l, s};
    if (contains(val, reserved,0, 8))
        received[counter++] = d;
}

// Check if a certain array contains some specific value.
int contains(char val, char *arr, int start, int end)
{
    for (int i = start; i < end; i++)
        if (arr[i] == val)
            return 1;
    return 0;
}