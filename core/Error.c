#include <stdio.h>
#include <stdlib.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (WTFPL) <Error.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>

/* Activation/Désactivation de l'affichage des erreurs */
static u8 Exception = 1;

/* niveau d'erreur */
static i8 ErrorLvl = 0;

/**
 * @brief Ignore les erreurs
 * @author Antoine LANDRIEUX
 *
 * @param _Ignore 0 si non
 */
void IgnoreException(unsigned char _Ignore)
{
    Exception = !_Ignore;
}

/**
 * @brief Efface les erreurs
 * @author Antoine LANDRIEUX
 *
 */
void ClearException(void)
{
    ErrorLvl = 0;
}

/**
 * @brief Retourne le niveau d'erreur
 * @author Antoine LANDRIEUX
 *
 * @return char
 */
char ErrorLevel(void)
{
    return ErrorLvl;
}

/**
 * @brief Créer une nouvelle erreur, et l'affiche
 * @author Antoine LANDRIEUX
 *
 * @param _Error
 * @param _String
 * @param _File
 * @return void* (retourne toujours NULL)
 */
void *LeaveException(char *_Error, char *_String, Document _File)
{
    // Si les erreurs sont désactivés, on affiche rien
    if (Exception)
    {
#ifndef __SOARE_NO_COLORED_OUTPUT
        // Red
        fprintf(stderr, "\033[31m");
#endif /* __SOARE_NO_COLORED_OUTPUT */
        fprintf(
            //
            stderr,
            "Except: %s\n\t\"%.10s\"\n\t ^~~~\n\tAt file %s:%lld:%lld\n",
            _Error,
            _String,
            _File.file,
            _File.ln,
            _File.col
            //
        );
#ifndef __SOARE_NO_COLORED_OUTPUT
        // Normal
        fprintf(stderr, "\033[0;39m");
#endif /* __SOARE_NO_COLORED_OUTPUT */
    }
    // On mais l'erreur au niveau 1
    ErrorLvl = 1;
    return NULL;
}
