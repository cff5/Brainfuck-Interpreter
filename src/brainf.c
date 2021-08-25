/* Simple implementation of Brainfuck interpreter */

#include <stdio.h>
#include <stdlib.h>

typedef struct stack_t
{
    int data;
    struct stack_t *next;

} stack;

stack* CreateStack(int data)
{
    stack *_ = (stack*)malloc(sizeof(stack));

    if (_ == NULL)
        return NULL;
    
    _->data = data;
    _->next = NULL;

    return _;
}

void PushStack(stack* insertingNode, stack **head)
{
    insertingNode->next = *head;
    *head = insertingNode;
}

stack* PopStack(stack **head)
{
    stack *cur = *head;

    if (*head == NULL)
        return NULL;
    
    *head = cur->next;
    cur->next = NULL;
    return cur;
}

stack* PeekStack(stack *head)
{
    return head;
}

int mod(int x, int y)
{
    return x - (x / y) * y;
}

int main(int argc, char const *argv[])
{
    FILE *fp = NULL;

    char *buffer = NULL;
    char c;

    int i, j;

    int numbytes;
    int memsize = 1024;
    int line_count, char_count;

    int *pairs = NULL;

    char *tape = NULL;

    stack *stk = NULL;
    stack *temp = NULL;

    if (argc != 2 && argc != 3)
    {
        printf("Usage: %s <file> <optional: memory-size, default: %d>\n", argv[0], memsize);
        return -1;
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("%s: error: %s: No such file\n", argv[0], argv[1]);
        return -1;
    }

    if (argc == 3)
        memsize = atoi(argv[2]);
    
    tape = (char*)calloc(memsize, sizeof(char));

    fseek(fp, 0L, SEEK_END);

    numbytes = ftell(fp);

    buffer = (char*)malloc(sizeof(char) * numbytes);

    rewind(fp);

    fread(buffer, 1, numbytes, fp);

    fclose(fp);

    pairs = (int*)malloc(sizeof(int) * numbytes);

    line_count = 1;
    char_count = 1;

    for (i = 0; (c = buffer[i]) != '\0'; i++)
    {
        if (c == '\n')
        {
            line_count++;
            char_count = 1;
        }
        else if (c == '[')
        {
            temp = CreateStack(i);

            if (temp == NULL)
            {
                printf("\n%s: error: overflow\n", argv[0]);
                return -1;
            }

            PushStack(temp, &stk);
        }
        else if (c == ']')
        {
            temp = PopStack(&stk);

            if (temp == NULL)
            {
                printf("\nSyntax error: Unexpected closing bracket in line %d char %d.\n", line_count, char_count);
                return -1;
            }

            pairs[temp->data] = i;
            pairs[i] = temp->data;

            free(temp);
        }

        char_count++;
    }

    if (PeekStack(stk) != NULL)
    {
        printf("\nSyntax error: Unexpected bracket in line %d char %d.\n", line_count, char_count);
        return -1;
    }

    for (i = 0, j = 0; (c = buffer[i]) != '\0'; i++)
    {
        switch (c)
        {
            case '+':
                tape[j]++;
                break;
            
            case '-':
                tape[j]--;
                break;
            
            case '[':
                if (tape[j] == 0)
                    i = pairs[i];
                
                break;
            
            case ']':
                if (tape[j] != 0)
                    i = pairs[i];
                
                break;
            
            case '<':
                j = mod(j - 1, memsize);
                break;
            
            case '>':
                j = (j + 1) % memsize;
                break;
            
            case '.':
                putchar((char)tape[j]);
                fflush(stdout);
                break;
            
            case ',':
                tape[j] = (char)getchar();
                break;
        }
    }

    free(buffer);
    free(tape);
    free(pairs);

    return 0;
}
