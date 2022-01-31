
#define StackSize 50             // change stack size according 
/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                    --STACK TOOLS--
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
struct stack                   //create stack
{
    int stk[StackSize];
    int top;
};
typedef struct stack STACK;
//---------------------------------------------------------------------------------------------------

void push (STACK s,int val)                  // stack push function
{
    if (s.top == (StackSize - 1))
    {
        printf ("Stack is Full\n");
        return;
    }
    else
    {
        s.stk[s.top] = val;
         s.top = s.top + 1;
    }
    return;
}
//---------------------------------------------------------------------------------------------------
int pop (STACK s)                       // stack pop function
{

    if (s.top == - 1)
    {
        printf ("Stack is Empty\n");
        return (s.top);
    }

    else {
        s.top++;
        return(s.stk[s.top]);
    }
}
//---------------------------------------------------------------------------------------------------
void printStack (STACK s)
{

    if (s.top == -1)
    {
        printf ("Stack is empty\n");
        return;
    }
    else
    {
        printf ("\n The status of the stack is \n");
        for (int i = s.top; i >= 0; i--)
        {
            printf ("%d\n", s.stk[i]);
        }
    }
    printf ("\n");
}