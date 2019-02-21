/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                     IBI v1.2                                    ;
;---------------------------------------------------------------------------------;
;                         Interactive Brainfuck Interpreter                       ;
;---------------------------------------------------------------------------------;
;                             - by Code Monkey King -                             ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;                          Implementation is based on the                         ;
;                          "simple brainfuck interpreter"                         ;
;                           - by Daniel B. Cristofani -                           ;
;                 http://www.hevanet.com/cristofd/brainfuck/sbi.c                 ;
;---------------------------------------------------------------------------------;
;                       "Extending the brainfuck language is                      ;
;                        like bolting parts to a skateboard                       ;
;                       in an attempt to build a pickup truck"                    ;
;---------------------------------------------------------------------------------;
;                             - Daniel B. Cristofani -                            ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;         Copyright © 2019 Maksim Korzh  <freesoft.for.people@gmail.com>          ;
;---------------------------------------------------------------------------------;
'       THIS PROGRAM IS FREE SOFTWARE. IT COMES WITHOUT ANY WARRANTY, TO          ;
;        THE EXTENT PERMITTED BY APPLICABLE LAW. YOU CAN REDISTRIBUTE IT          ;
;       AND/OR MODIFY IT UNDER THE TERMS OF THE DO WHAT THE FUCK YOU WANT         ;
;          TO PUBLIC LICENCE, VERSION 2, AS PUBLISHED BY SAM HOCEVAR.             ;
;                SEE http://www.wtfpl.net/ FOR MORE DETAILS.                      ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define VERSION "v1.2"

#define TAPESIZE 65536
#define SOURCESIZE 65536

int stack[SOURCESIZE], stack_ptr;
int loops[SOURCESIZE], source_ptr, source_length, user_input;

short int tape[TAPESIZE], cell;
char source[SOURCESIZE];

void InitLoops()
{
    for(source_ptr = 0; source_ptr < source_length; source_ptr++)
    {
        if(source[source_ptr] == '[') stack[stack_ptr++] = source_ptr;
        
        if (source[source_ptr] == ']')
        {
            if(!stack_ptr)
            {
                fprintf(stderr,"\n error: couldn't find matching '[' for ']' at byte %d\n\n", source_ptr);
                exit(1);
            }

            else
            {
                --stack_ptr;
                loops[source_ptr] = stack[stack_ptr];
                loops[stack[stack_ptr]] = source_ptr;
            }
        }
    }
    
    if(stack_ptr > 0)
    {
        fprintf(stderr,"\n error: couldn't find matching ']' for '[' at byte %d\n\n", stack[--stack_ptr]);
        exit(1);
    }
}

void ExecuteSource()
{
    InitLoops();

    for(source_ptr = 0; source_ptr < source_length; source_ptr++)
    {
        switch(source[source_ptr])
        {
            case '+': tape[cell]++; break;
            case '-': tape[cell]--; break;
            case '<': cell--; break;
            case '>': cell++; break;
            case ',':
                if((user_input = getchar()) != EOF)
                    tape[cell] = (user_input == '\n') ? 10 : user_input;

                break;
                
            case '.':
                putchar(tape[cell] == 10? '\n' : tape[cell]);
                break;
                
            case '[': if(!tape[cell]) source_ptr=loops[source_ptr]; break;
            case ']': if(tape[cell]) source_ptr=loops[source_ptr]; break;
            case '#': printf(" cell #%d: '%d'\n", cell, tape[cell]); break;
            case '@':
                printf("\n");

                for(int i = 0; i < 20; i++)
                {
                    if((cell + i) < 10)
                        printf("#%d  ", cell + i);
                    
                    else if((cell + i) >= 10)
                        printf("#%d ", cell + i);
                }

                printf("\n");

                for(int i = 0; i < 20; i++)
                {
                    if(tape[cell + i] < 10)
                        printf(" %d  ", tape[cell + i]);
                    
                    if(tape[cell + i] >= 10)
                        printf(" %d ", tape[cell + i]);
                }

                printf("\n");

                break; 
        }
    }
}

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("\n    Interactive Brainfuck Interpreter %s \n", VERSION);
        printf("          - by Code Monkey King -\n\n");
        printf("\n    STDIN read mode(current):\n\n");
        printf(" 1. type in the code or paste it from clipboard\n");
        printf(" 2. press Ctrl-D(Linux) or Ctrl-Z(Windows) to execute program\n\n");
        printf("\n    FILE read mode:\n\n");
        printf(" 1. ./ibi [filename]   (Linux)\n");
        printf(" 2. ibi.exe [filename] (Windows)\n\n");
        printf("\n    Happy brainfucking!\n\n");

        source_length = fread(source, 1, SOURCESIZE - 1, stdin);
        InitLoops();
        ExecuteSource();
    }

    else if(argc == 2)
    {
        FILE *source_file;
    
        if(!(source_file = fopen(argv[1], "r")))
        {
            fprintf(stderr,"\n error: couldn't open the file '%s'\n\n", argv[1]);
            exit(1);
        }
        
        source_length = fread(source, 1, SOURCESIZE, source_file);        
        fclose(source_file);
        ExecuteSource();
    }

    else if(argc > 2)
    {
        fprintf(stderr, "\n error: too many arguments\n\n");
        fprintf(stderr, " usage: ./ibi [filename]   (Linux)\n");
        fprintf(stderr, "        ibi.exe [filename] (Windows)\n\n");
        exit(1);
    }

    return 0;
}

