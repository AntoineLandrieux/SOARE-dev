
#include <DRIVER/memory.h>

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
 * @brief 
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
 * @brief 
 * 
 */
void free()
{
    for (unsigned long long i = 0; i < sizeof(memory); i++)
        memory[i] = 0;
    mfree = memory;
}

/**
 * @brief 
 * 
 * @param str1 
 * @param str2 
 * @return unsigned char 
 */
unsigned char strcmp(char *str1, char *str2)
{
    for (; *str1 || *str2; (volatile char *)str1++)
        if (*str1 != *str2)
            return 1;
        else
            (volatile char *)str2++;
    return 0;
}

/**
 * @brief 
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
 * @brief 
 * 
 * @param string 
 * @return long long int 
 */
long long int atoll(const char *string)
{
    long long int result = 0;
    int sign = 1;
    if (*string == '-' || *string == '+')
    {
        sign = *string == '-' ? -1 : 1;
        (volatile char *)string++;
    }
    for (; *string; (volatile char *)string++)
        if (*string == '.')
            break;
        else
            result = result * 10 + (*string - '0');
    return sign * result;
}

/**
 * @brief 
 * 
 * @param buff 
 * @param size 
 * @param value 
 * @return char* 
 */
char *lltoa(char *buff, int size, long long value)
{
    int i = 0;
    int sign = 0;

    if (value == 0)
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
 * @brief 
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
 * @brief 
 * 
 * @param dest 
 * @param string 
 * @param size 
 * @return unsigned char 
 */
unsigned char stradd(char *dest, char *string, unsigned long long size)
{
    unsigned long long index = 0;
    for (; dest[index]; index++)
        ;
    for (; *string; (volatile char *)string++)
    {
        if (index >= size)
            return 0;
        dest[index] = *string;
        index++;
    }
    return 1;
}
