#ifndef __SOARE__
#define __SOARE__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (WTFPL) <SOARE.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#ifdef __cplusplus
/**
 * SOARE namespace
 */
namespace SOARE
{
    extern "C"
    {
#endif

/* SOARE version majeur */
#define SOARE_MAJOR 1
/* SOARE version mineur */
#define SOARE_MINOR 0
/* SOARE version patch */
#define SOARE_PATCH 0

        /**
         * @brief Structure d'un docuement
         * @author Antoine LANDRIEUX
         */
        typedef struct Document
        {

            // Le fichier
            char *file;

            // La ligne
            unsigned long long ln;
            // La colone
            unsigned long long col;

        } Document;

/* Fichier d’entrée pour le mode console */
#define __SOARE_FILE__ "_input"

#include "core/error.h"
#include "core/tokenizer.h"
#include "core/parser.h"
#include "core/memory.h"
#include "core/math.h"
#include "core/runtime.h"

#ifdef __cplusplus
    }
}
#endif

#endif /* __SOARE__ */