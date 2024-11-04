#ifndef __SOARE__
#define __SOARE__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

/*  */
#define SOARE_MAJOR 1
/*  */
#define SOARE_MINOR 0
/*  */
#define SOARE_PATCH 0

#ifdef __cplusplus
namespace SOARE
{
    extern "C"
    {
#endif

        /**
         * @brief
         * @author Antoine LANDRIEUX
         *
         */
        typedef struct Document
        {

            char *file;

            unsigned long long ln;
            unsigned long long col;

        } Document;

#include "core/error.h"
#include "core/tokenizer.h"
#include "core/parser.h"

#ifdef __cplusplus
    }
}
#endif

#endif /* __SOARE__ */