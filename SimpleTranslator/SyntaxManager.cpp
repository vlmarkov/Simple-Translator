#include "header.h"

using namespace std;

// Class LexemManager
SyntaxManager::SyntaxManager() 
{
    syntaxMatrix_ = new vector<SyntaxNode>;
    syntaxStack_ = new stack<pair<States, string>>;
}

SyntaxManager::~SyntaxManager() 
{

}

/*
 * SYNTAX STATE TABLE
 * 0  - datatype
 * 1  - del
 * 2  - endstr 
 * 3  - com
 * 4  - var
 * 5  - exit
 * 6  - oper
 * 7  - math
 * 8  - asgn
 * 9  - loop
 * 10 - endloop
 * 11 - if
 * 12 - endif
 * 13 - else
 * 14 - endelse
 * 15 - start/end - point
*/

/* Manual filling the syntax relationship table */
void SyntaxManager::fillSyntaxMatrix()
{
    /*
     * int addSyntaxMatrix(int i, int j, int s);
     * First arg  (i) - new add in syntax stack
     * Second arg (j) - already store in syntax stack
     * Third arg  (s) - relation betwen 'j' & 'i' (1-push/2-pop_all)
     */
    addSyntaxMatrix(0, 15, 1);
    addSyntaxMatrix(1, 4, 1);
    addSyntaxMatrix(2, 4, 2);
    addSyntaxMatrix(2, 5, 2);
    addSyntaxMatrix(2, 10, 2);
    addSyntaxMatrix(2, 12, 2);
    addSyntaxMatrix(3, 2, 1);
    addSyntaxMatrix(3, 13, 1);
    addSyntaxMatrix(4, 1, 1);
    addSyntaxMatrix(4, 2, 1);
    addSyntaxMatrix(4, 6, 1);
    addSyntaxMatrix(4, 7, 1);
    addSyntaxMatrix(4, 8, 1);
    addSyntaxMatrix(4, 9, 1);
    addSyntaxMatrix(4, 11, 1);
    addSyntaxMatrix(5, 2, 1);
    addSyntaxMatrix(5, 15, 1);
    addSyntaxMatrix(6, 4, 1);
    addSyntaxMatrix(7, 4, 1);
    addSyntaxMatrix(8, 0, 1);
    addSyntaxMatrix(8, 3, 1);
    addSyntaxMatrix(9, 2, 1);
    addSyntaxMatrix(9, 9, 1);
    addSyntaxMatrix(10, 2, 2);
    addSyntaxMatrix(10, 4, 2);
    addSyntaxMatrix(10, 9, 1);
    addSyntaxMatrix(10, 10, 2);
    addSyntaxMatrix(11, 2, 1);
    addSyntaxMatrix(11, 15, 1);
    addSyntaxMatrix(12, 2, 1); 
    addSyntaxMatrix(12, 4, 1); 
    addSyntaxMatrix(12, 14, 1);
    addSyntaxMatrix(13, 15, 1);
    addSyntaxMatrix(14, 4, 1);
    addSyntaxMatrix(15, 2, 1);
    addSyntaxMatrix(15, -1, 2);
}

// Add the new node
int SyntaxManager::addSyntaxMatrix(int i, int j, int s)
{
    SyntaxNode newNode(i, j, s);
    
    syntaxMatrix_->push_back(newNode);

    return 0;
}

// Simple search in syntax relationship table
int SyntaxManager::searchSyntaxMatrix(int j, int i)
{
    for (auto ptr = syntaxMatrix_->begin(); ptr != syntaxMatrix_->end(); ptr++) {
        if ((j == ptr->getJ()) && (i == ptr->getI())) {
            return ptr->getS();
        }
    }
    return 0;
}

// Push in stack
int SyntaxManager::stack_push(enum States S, string V)
{
    pair<States, string> newPair;
    
    newPair = std::make_pair(S, V);

    syntaxStack_->push(newPair);

    return 0;
}

// Pop from stack
int SyntaxManager::stack_pop()
{
    struct syntax_stack *next;

    if (stack_empty()) {
        //fprintf(stderr, "error: stack is empty\n");
        return -1;
    }
    //printf("%s\n",head_stack->value);

    syntaxStack_->pop();

    return 0; 
}

bool SyntaxManager::stack_empty()
{
    return syntaxStack_->empty();
}

enum States SyntaxManager::get_head_stack_state()
{
    auto top = syntaxStack_->top();
    return top.first;
}

string SyntaxManager::get_head_stack_value()
{
    auto top = syntaxStack_->top();
    return top.second;
}

// Compare state between lexems
void SyntaxManager::compare_states(int input, enum States S, string V)
{
    int stack = -1;

    if (stack_empty()) {
        stack_push(S, V);
        return;
    }

    switch(get_head_stack_state())
    {
        case DATATYPE:
            stack = 0;
            break;

        case DELIMETR:
            stack = 1;
            break;

        case ENDSTR:
            stack = 2;
            break;

        case COMMAND:
            stack = 3;
            break;                

        case VARIABLE:
            stack = 4;
            break;

        case EXIT:           
            stack = 5;
            break;

        case OPERATION:
            stack = 6;
            break;

        case MATH:
            stack = 7;
            break;

        case ASIGN:
            stack = 8;
            break;
        
        case LOOP:
            stack = 9;
            break;
        
        case ENDLOOP:
            stack = 10;
            break;
        
        case IF:
            stack = 11;
            break;

        case ENDIF:
            stack = 12;
            break;

        case ELSE:
            stack = 13;
            break;

        case ENDELSE:
            stack = 14;
            break;
        case START:
            stack = 15;
            break;
    }

    //printf("%d %d -- %s\n", stack, input, V);

    if (searchSyntaxMatrix(stack, input) == 2) {
        
        do {
            // ... 
        } while(stack_pop() != -1);

    } else if (searchSyntaxMatrix(stack, input) == 1) {
        
        stack_push(S, V);
        //printf("stack push %s\n", V);

    } else if (searchSyntaxMatrix(stack, input) == 0) {
        
        //printf("in magazine: %s\n", head_stack->value);
        //do {} while(stack_pop() != -1);
        //printf("%d-%d\n", stack, input);
        if (stack_empty()) {

            switch(get_head_stack_state())
            {
                case DATATYPE:
                    if (S == VARIABLE) {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout <<  get_head_stack_value();
                        cout << " [missing ':'] ";
                        cout << V << endl;
                    }
                    break;

                case COMMAND:
                    if (S == VARIABLE) {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout <<  get_head_stack_value();
                        cout << " [missing ':'] ";
                        cout << V << endl;
                    }
                    break;

                case VARIABLE:
                    if (S == VARIABLE) {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout <<  get_head_stack_value();
                        cout << " [missing delimetr] ";
                        cout << V << endl;
                    } else {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout <<  get_head_stack_value();
                        cout << " [missing '.'] ";
                        cout << V << endl; 
                    }
                    break;

                case EXIT:
                    if (S == END) {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout << get_head_stack_value();
                        cout << " [missing '.'] ";
                        cout << V << endl;
                    }
                    break;

                case ENDELSE:
                    cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                    cout << get_head_stack_value();
                    cout << " [missing 'endif.'] ";
                    cout << V << endl;
                    break;

                case ENDIF:
                    if (S == ELSE) {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout << get_head_stack_value();
                        cout << " [missing '.'] ";
                        cout << V << endl;
                    } else {
                        cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                        cout << get_head_stack_value();
                        cout << " [missing '.'] ";
                        cout << V << endl;
                    }
                    break;

                default:
                    cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
                    cout << get_head_stack_value();
                    cout << " [missing something] ";
                    cout << V << endl;
                    break;
            }
        } else {
            cout << COLOR_RED << "-->SYNTAX ERROR " << COLOR_OFF;
            cout << V << endl;
        }
        do {
            // ....
        } while(stack_pop() != -1);
        
    } else {
        fprintf(stderr, "error: panic, panic\n");
        exit(0);
    }
}

// Syntax part
int SyntaxManager::syntaxAnalysis(vector<LexemNode> table)
{
    // Manual filling
    fillSyntaxMatrix();
    
    // First insertion by START POINT
    stack_push(START, "str"); 

    /*
     * Going by lexem linked list, check relationship between two lexems,
     * we can push in stack if that statisfied condtion or
     * we can pop from stack in two way:
     * if no relation and it is error (syntax error)
     * or we reach end of condition or expression (allright in syntax way)
     */
    for (auto ptr = table.begin(); ptr != table.end(); ptr++) {
        switch(ptr->getState())
        {
            case DATATYPE:
                compare_states(0, ptr->getState(), ptr->getValue());
                break;

            case DELIMETR:
                compare_states(1, ptr->getState(), ptr->getValue());    
                break;

            case ENDSTR:
                compare_states(2, ptr->getState(), ptr->getValue());    
                break;

            case COMMAND:
                compare_states(3, ptr->getState(), ptr->getValue());    
                break;    

            case VARIABLE:
                compare_states(4, ptr->getState(), ptr->getValue());    
                break;

            case EXIT:
                compare_states(5, ptr->getState(), ptr->getValue());    
                break;

            case OPERATION:
                compare_states(6, ptr->getState(), ptr->getValue());    
                break;

            case MATH:
                compare_states(7, ptr->getState(), ptr->getValue());    
                break;

            case ASIGN:
                compare_states(8, ptr->getState(), ptr->getValue());    
                break;

            case LOOP:
                compare_states(9, ptr->getState(), ptr->getValue());    
                break;

            case ENDLOOP:
                compare_states(10, ptr->getState(), ptr->getValue());    
                break;

            case IF:
                compare_states(11, ptr->getState(), ptr->getValue());    
                break;

            case ENDIF:
                compare_states(12, ptr->getState(), ptr->getValue());    
                break;

            case ELSE:
                compare_states(13, ptr->getState(), ptr->getValue());    
                break;

            case ENDELSE:
                compare_states(14, ptr->getState(), ptr->getValue());    
                break;

            case ERROR:
                //printf("\033[0;31m-->UNKNOWN CHARACTER\033[0m : %s\n", ptr->value);
                break;
        }
    }
    // The last insertion by END POINT
    compare_states(15, END, "eof");
}

/* 
 * Simple checking function 
 * for relation between:
 * 'if/endif', 'else/endelse', 'loop/endloop'
 */
void SyntaxManager::checkLoopIf(vector<LexemNode> table)
{   
    int loops = 0, endloops = 0;
    int ifs = 0, endifs = 0, elses = 0, endelses = 0;

    for (auto ptr = table.begin(); ptr != table.end(); ptr++) {
        switch (ptr->getState())
        {
            case LOOP:
                loops++;
                break;
            case ENDLOOP:
                endloops++;
                break;

            case IF:
                ifs++;
                break;

            case ENDIF:
                endifs++;
                break;

            case ELSE:
                ifs++;
                elses++;
                break;

            case ENDELSE:
                endelses++;
                break;
        }
    }

    if (loops != endloops) {
        cout << COLOR_RED << "-->SYNTAX ERROR" << COLOR_OFF;
        cout << ":[missing loop or endloop]\n" << endl;    
    }
    if (ifs != endifs) {
        cout << COLOR_RED << "-->SYNTAX ERROR" << COLOR_OFF;
        cout << ":[missing if or endif]\n" << endl;    
    } 
    if (elses != endelses) {
        cout << COLOR_RED << "-->SYNTAX ERROR" << COLOR_OFF;
        cout << ":[missing else or endelse]\n" << endl;    
    }  
}



void SyntaxManager::doSyntaxRoutine(LexemTable* table)
{
    AssemblyGen asmGen;

    cout << "LEXSICAL ANALYSIS - start" << endl;
    table->printTable();
    cout << "LEXSICAL ANALYSIS - complete" << endl;
    
    cout << "SYNTAX ANALYSIS: start" << endl;
    checkLoopIf(table->getTable());
    syntaxAnalysis(table->getTable());
    cout << "SYNTAX ANALYSIS: complete" << endl;

    cout << "ASSEMBLY GENERATE - start" << endl;
    asmGen.generateAssembly(table->getTable());
    cout << "ASSEMBLY GENERATE - complete" << endl;
}

