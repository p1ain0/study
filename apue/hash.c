#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _hash_table_t
{
    char *key;
    int index;
    int count;
    struct _hash_table_t* next;
}hash_table_t;


hash_table_t* add_ht(hash_table_t **ht,const char* key)
{
    if(*ht == NULL)
    {
        (*ht) = calloc(1, sizeof(hash_table_t));
        (*ht)->key = strdup(key);
        (*ht)->index = 0;
        (*ht)->count = 0;
        return NULL;
    }
    else
    {
        hash_table_t *tmp = *ht;
        hash_table_t *last = tmp;
        int index = -1;
        while(tmp)
        {
            if(strcmp(tmp->key, key) == 0)
            {
                tmp->count++;
                return tmp;
            }
            if(tmp->next == NULL)
                last = tmp;
            index = tmp->index;
            tmp = tmp->next;
            
        }
        if(tmp == NULL)
        {
            tmp = calloc(1, sizeof(hash_table_t));
            tmp->key = strdup(key);
            tmp->index = index + 1;
            last->next = tmp;
            return NULL;
        }
    }
    return NULL;
}

int cmp(char *a, char *b)
{
    return *a > *b?1:0;
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
char *** groupAnagrams(char ** strs, int strsSize, int* returnSize, int** returnColumnSizes){
    if(strs == NULL)
        return NULL;
    
    char ***retstr = calloc(1, sizeof(char**) * strsSize);
    *returnColumnSizes = calloc(1, sizeof(int*) * strsSize);
    *returnSize = 0;
    hash_table_t *ht = NULL;
    for(int i = 0; i < strsSize; i++)
    {
        char strtmp[101];
        strcpy(strtmp, strs[i]);
        qsort(strtmp, strlen(strtmp), sizeof(char), cmp);
        hash_table_t* tmp;
        tmp = add_ht(&ht, strtmp);
        if(tmp == NULL)
        {
            //Hash表没有
            retstr[*returnSize] = calloc(1, sizeof(char*) * strsSize);
            retstr[*returnSize][(*returnColumnSizes)[*returnSize]] = strdup(strs[i]);
            ((*returnColumnSizes)[*returnSize])++;
            (*returnSize)++;
        }
        else
        {
            //Hash表有
            retstr[tmp->index][tmp->count] = strdup(strs[i]);
            ((*returnColumnSizes)[tmp->index])++;
        }
    }
    return retstr;
}

int main()
{
    // char *strs[6] = {"eat","tea","tan","ate","nat","bat"};
    char *strs[6];
    strs[0] = "eat";
    strs[1] = "tea";
    strs[2] = "tan";
    strs[3] = "ate";
    strs[4] = "nat";
    strs[5] = "bat";
    int retsize = 0;
    int* retcs = 0;
    char *** c = groupAnagrams(strs, 6, &retsize, &retcs);
    return 0;
}