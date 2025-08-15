
#include <STD/stdlib.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <memory.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

static char memory[__MEMORY_POOL_SIZE__];

static char *mfree = memory;

/**
 * @brief Memory allocation
 *
 * @param size
 * @return void*
 */
void *malloc(unsigned long long size)
{
    if ((mfree + size) > (memory + __MEMORY_POOL_SIZE__))
        return NULL;

    void *alloc = mfree;
    mfree += size;
    return alloc;
}

/**
 * @brief Free allocated memory
 *
 */
void free()
{
    for (unsigned long long i = 0; i < sizeof(memory); i++)
        memory[i] = 0;
    mfree = memory;
}

/**
 * @brief Compare string
 *
 * @param str1
 * @param str2
 * @return unsigned char
 */
unsigned char strcmp(char *str1, char *str2)
{
    for (unsigned long long i = 0; str1[i] || str2[i]; i++)
        if (str1[i] != str2[i])
            return 1;
    return 0;
}

/**
 * @brief Search character from string
 *
 * @param string
 * @param character
 * @return unsigned char
 */
unsigned char strchr(char *string, char character)
{
    for (; *string; (volatile char *)string++)
        if (*string == character)
            return 1;
    return 0;
}

/**
 * @brief String length
 *
 * @param string
 * @return long long
 */
long long strlen(const char *string)
{
    for (long long i = 0; 1; i++)
        if (!string[i])
            return i;
    return 0;
}

/**
 * @brief String to int
 *
 * @param string
 * @return int
 */
int atoi(const char *string)
{
    int result = 0;
    int sign = 1;

    if (*string == '-' || *string == '+')
    {
        sign = *string == '-' ? -1 : 1;
        string++;
    }

    for (; *string; string++)
        if (*string >= '0' && *string <= '9')
            result = result * 10 + (*string - '0');
        else
            break;

    return sign * result;
}

/**
 * @brief Int to string
 *
 * @param buff
 * @param size
 * @param value
 * @return char*
 */
char *itoa(char *buff, int size, int value)
{
    int i = 0;
    int sign = 0;

    if (!value)
    {
        buff[i++] = '0';
        buff[i] = 0;
        return buff;
    }

    if (value < 0)
    {
        sign = 1;
        value = -value;
    }

    for (; value > 0 && i < size - 1; value /= 10)
        buff[i++] = '0' + (value % 10);

    if (sign && i < size - 1)
        buff[i++] = '-';

    buff[i] = 0;

    int start = 0;
    int end = i - 1;

    while (start < end)
    {
        char temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }

    return buff;
}

/**
 * @brief Append string
 *
 * @param dest
 * @param string
 */
void strcat(char *dest, char *string)
{
    unsigned long long i = 0;
    for (; dest[i]; i++)
        ;
    for (; *string; (volatile char *)string++)
    {
        dest[i] = *string;
        i++;
    }
    dest[i] = 0;
}

/**
 * @brief Copy string
 *
 * @param dest
 * @param string
 */
void strcpy(char *dest, char *string)
{
    unsigned long long i = 0;
    for (; string[i]; i++)
        dest[i] = string[i];
    dest[i] = 0;
}
