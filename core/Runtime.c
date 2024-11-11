#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

#include <SOARE/SOARE.h>

char *Runtime(AST *_Tree)
{
    if (_Tree == NULL)
        return NULL;
    
    AST *root = _Tree;
    char *returned = NULL;

    for (AST *curr = root->child; curr; curr = curr->sibling)
    {
        switch (curr->type)
        {
        case NODE_NOP:
            break;
        
        case NODE_IMPORT:
            // TODO: implement import
            break;
        
        case NODE_TRY:

            ClearException();
            IgnoreException(0x1);
            returned = Runtime(curr->child);
            IgnoreException(0x0);
            
            if (ErrorLevel())
                returned = Runtime(curr->child->sibling);
            
            if (returned != NULL)
                return returned;
            
            break;
        
        case NODE_RAISE:
            return LeaveException(curr->value, "raise", curr->file);

        default:
            return LeaveException(InterpreterError, curr->value, curr->file);
        }
    }

    return NULL;
}
