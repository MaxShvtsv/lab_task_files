#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CONSONANTS "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ"
#define CONSONANTS_COUNT 52
#define PATH_LEN 256
#define STR_LEN 100
#define FILE_LEN 1024
#define MAX_VIEW_COUNT 5
#define INFINITE -1

// Prints a line.
void print_line()
{
    printf("\n-------------------------------------\n");
}

// Prints a title.
void print_front_page(){
    system("cls||clear");
    print_line();
    printf("\nLaboratory work №8");
    printf("\nTheme: 'Working with binary and text files'");
    printf("\nDone by student of group КМ-01 - Nimchenko Max\n");
    print_line();
}

int input_number(int min, int max)
{
    int number;
    char last;
    if (max != INFINITE)
        while (!scanf("%d%c", &number, &last) || last != '\n' || number < min || number > max)
        {
            rewind(stdin);
            printf("Wrong number. Try again: ");
            fflush(stdin);
        } 
    else
        while (!scanf("%d%c", &number, &last) || last != '\n' || number < min)
        {
            rewind(stdin);
            printf("Wrong number. Try again: ");
            fflush(stdin);
        }
    
    return number;
}

/* Splits string.
 * str - string to be splitted.
 * delim - a deliminator.
 * splitted_words - array to which add splitted words.
 */
void split(char str[], char *delim, char *splitted_words[])
{
    // pos: position of word to be splitted.
    int pos = 0;
    // word: char array, a word as a result after splitting.
    char *word = strtok(str, delim);
    while (word != NULL)
    {
        // While there is no what to split.
        splitted_words[pos] = word;
        word = strtok(NULL, delim);
        pos = pos + 1;
    }
}

int is_consonant(char letter)
{
    for(int i = 0; i < CONSONANTS_COUNT; i++)
        if (letter == CONSONANTS[i])
            return 1;
    return 0;
}

int count_consonants(char *string)
{
    int count = 0;
    for(int i = 0; i < strlen(string); i++)
        if (is_consonant(string[i]) == 1)
            count++;
    return count;
}

int find_max_in_array(int array[], int arr_len)
{
    int max_index = 0;
    int max = array[0];
    for(int i = 1; i < arr_len; i++)
        if (array[i] > max)
        {
            max = array[i];
            max_index = i;
        }
    return max_index;
}

void create_new_file()
{
    FILE* new_file;

    char path_to_file[PATH_LEN];
    char file_name[PATH_LEN];

    while (1)
    {
        // Input.
        printf("\nInput path where to create new file");
        printf("\nNOTE: add '\\' in the end");
        printf("\nNOTE: no more than 255 symbols");
        printf("\nNOTE: input 'x' to return to menu");
        printf("\nNOTE: input 'here' to create file in current directory");
        printf("\nPath: ");

        // Input path to file.
        fgets(path_to_file, PATH_LEN, stdin);
        path_to_file[strlen(path_to_file) - 1] = '\0';

        // Stop creating a file.
        if (strcmp(path_to_file, "x") == 0)
            return;

        // Input file name.
        printf("\nInput file name:");

        fgets(file_name, PATH_LEN, stdin);
        file_name[strlen(file_name) - 1] = '\0';

        // Combine path and file name.
        if (strcmp(path_to_file, "here") != 0)
            strcat(path_to_file, file_name);
        else
            strcpy(path_to_file, file_name);

        // Check if file exists.
        if (access(path_to_file, F_OK) == 0)
        {
            printf("\nSuch file exists. Input another path or file name.");
            continue;
        }

        new_file = fopen(path_to_file, "w");
        if (new_file == NULL)
        {
            // Invalid path.
            printf("\nInvalid path. Try again.");
            continue;
        }
        fclose(new_file);
        break;
    }
}

int open_file(char path_to_file_ptr[], int *lines_count_ptr, char file_str_loc[FILE_LEN])
{
    FILE* opened_file;

    while (1)
    {
        printf("\nInput path to file:\n");
        printf("\nNOTE: no more than 255 symbols,");
        printf("\nNOTE: input 'x' to return to menu,");
        printf("\nNOTE: to open file in current cwd just input name of file.");
        printf("\nYour path: ");

        fgets(path_to_file_ptr, PATH_LEN, stdin);
        path_to_file_ptr[strlen(path_to_file_ptr) - 1] = '\0';

        if (strcmp(path_to_file_ptr, "x") == 0)
        {
            // Back to menu.
            return 1;
        }

        opened_file = fopen(path_to_file_ptr, "a+");
        if (opened_file != NULL)
        {
            // Path and file exist.
            break;
        }
        printf("\nInvalid path or file exist. Try again.");
    }
    print_line();

    // Reading opened file for further operations //

    // Variables for reading the file.
    char current_char;
    int pos = 0;

    // Reading whole file.
    while((current_char = getc(opened_file)) != EOF)
    {
        if (current_char == '\n')
            *lines_count_ptr += 1;
        file_str_loc[pos] = current_char;
        pos++;
    }
    *lines_count_ptr += 1;

    file_str_loc[pos] = '\0';

    fclose(opened_file);

    return 0;
}

void print_view(int min, int max, char** file_lines_loc)
{
    for(int i = min; i < max; i++)
    {
        if (file_lines_loc[i] == NULL)
            file_lines_loc[i] = "";
        printf("\n%d.\t\t    '%s'", i + 1, file_lines_loc[i]);
    }
}

void view_file(int lines_count_loc, char** file_lines_loc)
{
    // Variables to organise reviwing a file.
    int view_choose = -1;
    int current_line_begin = 0;
    int current_line_end = MAX_VIEW_COUNT;
    int line_jump = MAX_VIEW_COUNT;

    // Firstly, view first five lines(if exist).
    printf("Number of line      Line");
    if (lines_count_loc < line_jump)
    {
        // There is less then MAX_VIEW_COUNT lines.
        line_jump = lines_count_loc;
        current_line_end = lines_count_loc;
    }

    print_view(0, line_jump, file_lines_loc);
    print_line();

    while (1)
    {
        printf("\nWhat to view?");
        printf("\n0.Stop viewing,");
        printf("\n1.Beginning of the current file,");
        printf("\n2.Ending of the current file,");
        printf("\n3.Scroll up(by 5 lines),");
        printf("\n4.Scroll down(by 5 lines),");
        printf("\nYour input: ");
        view_choose = input_number(0, 4);
        if (view_choose == 0)
        {
            print_line();
            break;
        }
        print_line();

        // --- Viewing ---
        printf("Number of line      Line");

        if (view_choose == 1)
        {
            current_line_begin = 0;
            current_line_end = line_jump;

            print_view(current_line_begin, current_line_end, file_lines_loc);
        } else if (view_choose == 2)
        {
            current_line_begin = lines_count_loc - line_jump;
            current_line_end = lines_count_loc;

            print_view(current_line_begin, current_line_end, file_lines_loc);
        } else if (view_choose == 3)
        {
            if (current_line_begin - line_jump < 0)
            {
                current_line_begin = 0;
                current_line_end = line_jump;
            } else
            {
                current_line_begin -= line_jump;
                current_line_end -= line_jump;
            }

            print_view(current_line_begin, current_line_end, file_lines_loc);
        } else if (view_choose == 4)
        {
            if (current_line_end + line_jump > lines_count_loc)
            {
                current_line_end = lines_count_loc;
                current_line_begin = lines_count_loc - line_jump;
            } else
            {
                current_line_begin += line_jump;
                current_line_end += line_jump;
            }

            print_view(current_line_begin, current_line_end, file_lines_loc);
        }
        view_choose = -1;
        print_line();
    }
}

void save_current_file(char **file_lines_loc, char path[PATH_LEN], int lines_count_loc)
{
    FILE *new_file = fopen(path, "w");
    for(int i = 0; i < lines_count_loc; i++)
    {
        fputs(file_lines_loc[i], new_file);
        fputs("\n", new_file);
    }
    fclose(new_file);
}

void free_file_lines(char **file_lines_loc, int lines_count_loc)
{
    for (int i = 0; i < lines_count_loc; i++)
        free(file_lines_loc[i]);
}

int main(void)
{
    FILE* current_file = NULL;
    char **file_lines = (char**)malloc(sizeof(char*));
    char path_to_file[PATH_LEN];
    int lines_count = 0;
    int is_file_open = 0;

    // print_front_page();
    int choose = -1;
    while (choose != 0)
    {
        // Choosing option exit/work.
        printf("\nChoose option:");
        printf("\n0.Exit from program,");
        printf("\n1.Create new file,");
        printf("\n2.Open file,");
        printf("\n3.View file,");
        printf("\n4.Edit file,");
        printf("\n5.Save file,");
        printf("\n6.Save file as,");
        printf("\n7.Close file,");
        printf("\n8.Copy to one file from another file rows which begins with letter 'A'");
        printf("\nand situated between given lines.");
        printf("\n9.Search a line in file, where is more consonants.");
        printf("\nYour input: ");

        choose = input_number(0, 9);
        print_line();

        if (choose == 0)
        {
            // Exit //
            if (current_file != NULL)
                fclose(current_file);

            break;
        } else if (choose == 1)
        {
            // Create new file //
            create_new_file();
        } else if (choose == 2)
        {
            // Open file //

            // Close previous file.
            if (is_file_open == 1)
            {
                char to_save;
                printf("\nDo you want to save and close current file(y/n)?");
                to_save = getchar();
                getchar();
                if (to_save == 'y')
                {
                    save_current_file(file_lines, path_to_file, lines_count);
                }
                free_file_lines(file_lines, lines_count);
                lines_count = 0;
                fclose(current_file);
            }

            // Opening new file.
            char file_str[FILE_LEN];
            int to_menu = open_file(path_to_file, &lines_count, file_str);

            if (to_menu == 1)
                continue;

            file_lines = (char**)realloc(file_lines, lines_count * sizeof(char*));

            for(int i = 0; i < lines_count; i++)
                file_lines[i] = (char*)malloc(STR_LEN * sizeof(char));

            split(file_str, "\n", file_lines);

            is_file_open = 1;
        } else if (choose == 3)
        {
            // View file.
            view_file(lines_count, file_lines);
        } else if (choose == 4)
        {
            int edit_choose = -1;
            while (1)
            {
                // Edit file.
                printf("\nInput how to edit or exit:");
                printf("\n0. Return to menu,");
                printf("\n1. Insert a line to the beginning of the file,");
                printf("\n2. Insert a line to the end of the file,");
                printf("\n3. Insert a line after the given number of line in the file,");
                printf("\n4. Edit a line with given position,");
                printf("\n5. Remove a line with given position,");
                printf("\nYour input: ");
                edit_choose = input_number(0, 5);
                if (edit_choose == 0)
                {
                    print_line();
                    break;
                }
                print_line();

                // Editing.
                if (edit_choose == 1)
                {
                    // Moving array elements by one in front.
                    lines_count++;
                    file_lines = (char**)realloc(file_lines, lines_count * sizeof(char*));
                    file_lines[lines_count - 1] = (char*)malloc(STR_LEN * sizeof(char));
                
                    for(int i = lines_count - 2; i >= 0; i--)
                    {
                        char *temp_line = file_lines[i];
                        file_lines[i + 1] = temp_line;
                    }

                    // Input a string which to add to array
                    printf("\nInput string to add: ");
                    fgets(file_lines[0], STR_LEN, stdin);
                    file_lines[0][strlen(file_lines[0]) - 1] = '\0';

                    // Output resulting list.
                    print_line();
                    for (int i = 0; i < lines_count; i++)
                    {
                        printf("\n%d.'%s'", i+1, file_lines[i]);
                    }
                    print_line();
                } else if (edit_choose == 2)
                {
                    char *string_to_add;
                    printf("\nInput string to add: ");

                    fgets(string_to_add, STR_LEN, stdin);
                    string_to_add[strlen(string_to_add) - 1] = '\0';

                    lines_count++;
                    file_lines = (char**)realloc(file_lines, lines_count * sizeof(char*));
                    file_lines[lines_count - 1] = (char*)malloc(STR_LEN * sizeof(char));
                    file_lines[lines_count - 1] = string_to_add;
                } else if (edit_choose == 3)
                {
                    int line_after_to_insert;
                    char *string_to_add;

                    printf("\nInput number of line after which to insert: ");
                    line_after_to_insert = input_number(0, INFINITE);
                    printf("\nInput string to add: ");
                    fgets(string_to_add, STR_LEN, stdin);
                    string_to_add[strlen(string_to_add) - 1] = '\0';
                    lines_count++;
                    file_lines = (char**)realloc(file_lines, lines_count * sizeof(char*));
                    file_lines[lines_count - 1] = (char*)malloc(STR_LEN * sizeof(char));

                    for(int i = lines_count - 2; i >= line_after_to_insert; i--)
                    {
                        char *temp_line = file_lines[i];
                        file_lines[i + 1] = temp_line;
                    }

                    file_lines[line_after_to_insert] = string_to_add;
                } else if (edit_choose == 4)
                {
                    int line_to_edit;
                    char *string_to_replace;

                    printf("\nInput number of line which to edit: ");
                    line_to_edit = input_number(0, INFINITE);
                    printf("\nInput string to replace: ");
                    fgets(string_to_replace, STR_LEN, stdin);
                    string_to_replace[strlen(string_to_replace) - 1] = '\0';

                    file_lines[line_to_edit - 1] = string_to_replace;
                } else if (edit_choose == 5)
                {
                    int line_to_remove;

                    printf("\nInput number of line which to remove: ");
                    line_to_remove = input_number(0, INFINITE);

                    for (int i = line_to_remove - 1; i < lines_count - 1; i++)
                    {
                        char *temp_str = file_lines[i];
                        file_lines[i] = file_lines[i + 1];
                        file_lines[i + 1] = temp_str;
                    }
                    
                    lines_count--;
                    free(file_lines[lines_count - 1]);
                }
                edit_choose = -1;
            }
        } else if (choose == 5)
        {
            // Save file //
            save_current_file(file_lines, path_to_file, lines_count);
            printf("\nFile is saved.");
            fclose(current_file);
            print_line();
        } else if (choose == 6)
        {
            // Save file as //
            char save_as_path[PATH_LEN];
            char file_name[STR_LEN];

            // Input file name.
            printf("\nInput new file name: ");

            fgets(file_name, STR_LEN, stdin);
            for (int i = 0; i < lines_count; i++)
                printf("\n'%s'", file_lines[i]);
            file_name[strlen(file_name) - 1] = '\0';

            // // Input path where to save.
            // printf("\nInput path where to save current file:");
            // printf("\nNOTE: put '\\' in the end.");
            // printf("\nNOTE: input 'here' to save file in current directory.");
            // printf("\nYour path: ");

            // fgets(save_as_path, STR_LEN, stdin);
            // save_as_path[strlen(save_as_path) - 1] = '\0';

            // // for (int i = 0; i < lines_count; i++)
            // //     printf("\n'%s'", file_lines[i]);

            // if(strcmp(save_as_path, "here") != 0)
            //     strcat(save_as_path, file_name);
            // else
            //     strcpy(save_as_path, file_name);

            save_current_file(file_lines, file_name, lines_count);
            printf("\nFile is save here: %s", file_name);
            print_line();
        } else if (choose == 7)
        {
            // Close file //
            fclose(current_file);
            printf("\nCurrent file(%s) is closed.", path_to_file);
            print_line();
        }
        else if (choose == 8)
        {
            // Copy lines from one file to another //
            char path_1[PATH_LEN];
            char path_2[PATH_LEN];
            int min_line;
            int max_line;
            int to_menu = 0;

            int lines_count_1 = 0;
            char file_str_1[FILE_LEN];
            to_menu = open_file(path_1, &lines_count_1, file_str_1);

            if (to_menu == 1)
                continue;

            char **file_lines_1 = (char**)malloc(lines_count_1 * sizeof(char*));

            for(int i = 0; i < lines_count_1; i++)
                file_lines_1[i] = (char*)malloc(STR_LEN * sizeof(char));
                
            split(file_str_1, "\n", file_lines_1);
            printf("\nWatch on this file and decide which lines to choose.\n");
            view_file(lines_count_1, file_lines_1);

            printf("\nInput first number of line:");
            printf("\nNOTE: numbers must be between 1 and %d,", lines_count_1);
            printf("\nYour input: ");
            min_line = input_number(1, lines_count_1);
            print_line();

            printf("\nInput second number of line:");
            printf("\nNOTE: numbers must be between %d and %d,", min_line, lines_count_1);
            printf("\nYour input: ");
            max_line = input_number(min_line, lines_count_1);
            print_line();

            FILE* file_2;

            while (1)
            {
                printf("\nInput path to file:\n");
                printf("\nNOTE: no more than 255 symbols,");
                printf("\nNOTE: input 'x' to return to menu,");
                printf("\nNOTE: to open file in current cwd just input name of file.");
                printf("\nYour path: ");

                fgets(path_2, PATH_LEN, stdin);
                path_2[strlen(path_2) - 1] = '\0';

                if (strcmp(path_2, "x") == 0)
                {
                    // Back to menu.
                    to_menu = 1;
                    break;
                }

                file_2 = fopen(path_2, "a+");
                if (file_2 != NULL)
                {
                    // Path and file exist.
                    break;
                }
                printf("\nInvalid path or file exist. Try again.");
            }

            if (to_menu == 1)
                continue;
            print_line();
            printf("\nFrom first file(%s)", path_1);
            printf("\nto second file(%s)", path_2);
            printf("\nsuch lines copied:");
            printf("\nNumber of line      Line");
            for (int i = min_line - 1; i < max_line; i++)
            {
                if (file_lines_1[i][0] == 'A')
                {   
                    fputs("\n", file_2);
                    fputs(file_lines_1[i], file_2);
                }
            }
            fclose(file_2);
            print_line();
        }
        else if (choose == 9)
        {
            char path_to_file_consonants[PATH_LEN];
            int lines_count_consonants = 0;
            char file_str_consonants[FILE_LEN];

            int to_menu = open_file(path_to_file_consonants, &lines_count_consonants, file_str_consonants);

            if (to_menu == 1)
                continue;

            char **file_lines_consonants = (char**)malloc(lines_count_consonants * sizeof(char*));

            for(int i = 0; i < lines_count_consonants; i++)
                file_lines_consonants[i] = (char*)malloc(STR_LEN * sizeof(char));

            split(file_str_consonants, "\n", file_lines_consonants);

            int array_consonants_count[lines_count_consonants];
            for (int i = 0; i < lines_count_consonants; i++)
            {
                array_consonants_count[i] = count_consonants(file_lines_consonants[i]);
                printf("\n%d.%d", i+1, array_consonants_count[i]);
            }
            int line_to_print = find_max_in_array(array_consonants_count, lines_count_consonants);

            printf("\nLine with more consonants in file(%s):", path_to_file_consonants);
            printf("'%s'", file_lines_consonants[line_to_print]);

            print_line();
        }
        choose = -1;
    }
    return 0;
}