#include "header.h"

using namespace std;

// Regular jump
char jump[6][3] = { {">"}, {">="}, {"<"}, {"<="}, {"!="}, {"=="} };
// Assembly jump
char assem_jump[6][4] = { {"jg"}, {"jge"}, {"jl"}, {"jle"}, {"jne"}, {"je"} };

AssemblyGen::AssemblyGen() {}
AssemblyGen::~AssemblyGen() {}

void AssemblyGen::generateAssembly(vector<LexemNode> table)
{
    FILE *output = fopen("output.s", "w");
    if (output == NULL) {
        fprintf(stderr, "\033[0;31m-->error\033[0m: can't open file.asm\n");
        exit(0);
    }

    // Start of section .data
    fprintf(output, "%s", ".data\n\tscanf_int_format:\n\t\t.string \"%d%*c\"\n\t"
        "printf_int_format:\n\t\t.string \"%d\\n\"\n\tscanf_char_format:\n"
        "\t\t.string \"%c%*c\"\n\tprintf_char_format:\n\t\t.string \"%c\\n\"");

    // Fill section .data (for int date type)
    for (auto ptr = table.begin(); ptr != table.end(); ptr++) {
        if (ptr->getState() == DATATYPE) {
            if (ptr->getValue().compare("int") == 0) {
                ptr++; // skip asign
                ptr++; // move to variable(s)
                while (ptr->getValue().compare(".") != 0 && ptr != table.end()) {   // write until end of string
                    if (ptr->getValue().compare(",") != 0) {                 // skip delimetr
                        fprintf(output, "\n\t%s", ptr->getValue().c_str());
                        fprintf(output, "%s", ":\n\t\t.long 0");
                    }
                    ptr++;
                }
            }
        }
    }

    // Fill section .data (for char date type)
    for (auto ptr = table.begin(); ptr != table.end(); ptr++) {
        if (ptr->getState() == DATATYPE) {
            if (ptr->getValue().compare("char") == 0) {
                ptr++; // skip asign
                ptr++; // move to variable(s)
                while (ptr->getValue().compare(".") != 0 && ptr != table.end()) { // write until end of string
                    if (ptr->getValue().compare(",") != 0) {                 // skip delimetr
                        fprintf(output, "\n\t%s", ptr->getValue().c_str());
                        fprintf(output, "%s", ":\n\t\t.byte 0");
                    }
                    ptr++;
                }
            }
        }
    }
    // End of section .data

     // Start of section .text
    fprintf(output, "%s", "\n.text\n.globl main\n.type main, @function\nmain:\n\n");

    int ifs = 0, endifs = 0;
    int loops = 0, endloops = 0;
    
    auto prev = table.begin();
    auto curr = table.begin();
    auto next = table.begin();

    string eq;
    
    for (auto ptr = table.begin(); ptr != table.end(); ptr++) {
        int  i;
        char *buff, *buff1;
        auto action = table.begin();
        
        switch(ptr->getState())
        {
            case DATATYPE:
                while (ptr->getState() != ENDSTR) {
                    ptr++;            // skip data type fields
                }
                break;

            case EXIT:
                fprintf(output, "%s", "ret\n");
                break;

            case ENDSTR:
                fprintf(output, "%s", "\n");
                break;

            case COMMAND:
                if (ptr->getValue().compare("read") == 0) {
                    ptr++; // skip asign
                    ptr++; // move to variable
                    fprintf(output, "%s%s\n", "pushl $", ptr->getValue().c_str());
                    
                    if (ptr->getType().compare("int") == 0) {
                        fprintf(output, "%s", "pushl $scanf_int_format\n");
                    } else {
                        fprintf(output, "%s", "pushl $scanf_char_format\n");
                    }
                    fprintf(output, "%s\n", "call scanf\naddl $8, %esp");
                } else if (ptr->getValue().compare("write") == 0) {
                    
                    ptr++; // skip asign
                    ptr++; // move to variable
                    
                    fprintf(output, "%s%s\n", "pushl ", ptr->getValue().c_str());
                    
                    if (ptr->getType().compare("int") == 0) {
                        fprintf(output, "%s\n", "pushl $printf_int_format");
                    } else {
                        fprintf(output, "%s\n", "pushl $printf_char_format");
                    }
                    
                    fprintf(output, "%s\n", "call printf\naddl $8, %esp");
                }
                break;

            case MATH:
                if (ptr->getValue().compare("= ") == 0) {
                    cout << "=" << endl;
                    cout << ptr->getValue() << endl;
                    ptr++; // move to next variable;
                    curr = ptr;
                    ptr++; // move to next variable;
                    cout << curr->getValue() << endl;
                    cout << ptr->getValue() << endl;                    
                    /*if (curr->type == 'c') {
                        buff = "$";
                    } else {
                        buff = "";
                    }*/
                    buff = "";
                    
                    if (ptr->getValue().compare("+") == 0) {
                        next = ptr+1;
                        if (next->getType().compare("char") == 0) {
                            cout << "char" << endl;
                            buff1 = "$";
                        } else {
                            cout << "int" << endl;
                            buff1 = "";
                        }
                        cout << "++" << endl;
                        cout << curr->getValue() << endl;
                        cout << next->getValue()<< endl;
                        cout << prev->getValue()<< endl;

                        fprintf(output, "movl %s%s, %%eax\naddl %s%s, %%eax\nmovl %%eax, %s\n", 
                    buff, curr->getValue().c_str(), buff1, next->getValue().c_str(), prev->getValue().c_str());
                    } else if (ptr->getValue().compare("- ") == 0) {
                        next = ptr++;
                        if (next->getType().compare("char") == 0) {
                            buff1 = "$";
                        } else {
                            buff1 = "";
                        }
                        fprintf(output, "movl %s%s, %%eax\nsubl %s%s, %%eax\nmovl %%eax, %s\n", 
                    buff, curr->getValue().c_str(), buff1, next->getValue().c_str(), prev->getValue().c_str());
                    } else if (ptr->getValue().compare("* ") == 0) {
                        next = ptr++;
                        if ((next->getType(), "char") == 0) {
                            buff1 = "$";
                        } else {
                            buff1 = "";
                        }
                        fprintf(output, "movl %s%s, %%eax\nmovl %s%s, %%ebx\nmull %%ebx\nmovl %%eax, %s\n", 
                    buff, curr->getValue().c_str(), buff1, next->getValue().c_str(), prev->getValue().c_str());
                    } else if (ptr->getValue().compare("/ ") == 0) {
                        next = ptr++;
                        if ((next->getType(), "char") == 0) {
                            buff1 = "$";
                        } else {
                            buff1 = "";
                        }
                        fprintf(output, "movl %s%s, %%eax\nimull %s%s\nmovl %%eax, %s\n", 
                    buff, curr->getValue().c_str(), buff1, next->getValue().c_str(), prev->getValue().c_str());
                    }
                }
                break;

            case IF:
                //printf("!!!\n");
                ptr = ptr++;
                fprintf(output, "movl %s, %%eax\n", ptr->getValue().c_str());
                ptr = ptr++;
                // find needed jump
                for (i = 0; i < 6; i++) {
                    if (ptr->getValue().compare(&jump[i][0]) == 0) {
                        break;
                    }
                }
                //printf("dd%d\n", i);
                ptr++;
                fprintf(output, "cmp %s, %%eax\n", ptr->getValue().c_str());
                fprintf(output, "%s .IF%d\n", &assem_jump[i][0], ifs);
                fprintf(output, "jmp .ENDIF%d\n", ifs);
                fprintf(output, ".IF%d:\n", ifs);

                ifs++;
                if ((ifs - 1) == endifs) {
                    endifs++;
                } else {
                    endifs = ifs;
                }
                break;

            case ENDIF:
                //printf("eee\n");
                action = ptr;
                action = action + 2;
                if (action->getValue().compare("else") == 0) {
                    fprintf(output, "jmp .ENDELSE%d\n.ENDIF%d:\n", endifs - 1, endifs - 1);
                    ptr = action;
                } else {
                    //fprintf(output, ".ENDIF%d:\n", endifs - 1);
                    //endifs--;
                    ptr++;
                }
                break;   

            case ENDELSE:
                fprintf(output, ".ENDELSE%d:\n", endifs - 1);
                endifs--;
                break;

            case LOOP:
                ptr++;
                fprintf(output, ".TLOOP%d:\n", loops);
                fprintf(output, "movl %s, %%eax\n", ptr->getValue().c_str());
                ptr++;
                for (i = 0; i < 6; i++) {
                    if (ptr->getValue().compare(&jump[i][0]) == 0) {
                        break;
                    }
                }
                ptr++;
                fprintf(output, "cmp %s, %%eax\n", ptr->getValue().c_str());
                fprintf(output, "%s .LOOP%d\n", &assem_jump[i][0], loops);
                fprintf(output, "jmp .ENDLOOP%d\n", loops);
                fprintf(output, ".LOOP%d:\n", loops);

                loops++;
                if ((loops - 1) == endloops) {
                    endloops++;
                } else {
                    endloops = loops;
                }
                break;

            case ENDLOOP:
                endloops--;
                fprintf(output, "jmp .TLOOP%d\n.ENDLOOP%d:\n", endloops, endloops);
                break;
        }
        prev = ptr;
    }
    // End of section .text

    fclose(output);
}