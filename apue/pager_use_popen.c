#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGER "${PAGER:-more}"
#define MAXLINE 256

void err_quit(const char *err_msg, ...)
{
    va_list va;
    va_start(va, err_msg);
    vfprintf(stderr, err_msg, va);
    va_end(va);
    exit(-1);
}

int main(int argc, char* argv[])
{
    char line[MAXLINE];
    FILE *fpin, *fpout;
    if(argc < 2)
        err_quit("Usage: pager <filename>\n");
    
    fpin = fopen(argv[1], "r");
    if(fpin == NULL)
        err_quit("Can't open %s", argv[1]);
    
    fpout = popen(PAGER, "w");
    if(fpout == NULL)
        err_quit("popen error\n");

    while(fgets(line,MAXLINE,fpin))
        if(fputs(line, fpout) == EOF)
            err_quit("fput error to pipe\n");
    if(ferror(fpin) != 0)
        err_quit("fgets error\n");
    if(pclose(fpout) != -1)
        err_quit("pclose error\n");
    exit(0);
}


/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
struct hashTable {
    char* key;
    int index;
    int cnt;
    UT_hash_handle hh;
};
int cmp (const void* a, const void* b) {
    return *(char*)a > *(char*)b ? 1 : 0;
}
char *** groupAnagrams(char ** strs, int strsSize, int* returnSize, int** returnColumnSizes){
    if (strsSize == 0) return NULL;
    char ***res = (char ***)malloc(sizeof(char **) * strsSize);
    *returnColumnSizes = (int *)malloc(sizeof(int) * strsSize);
    *returnSize = 0;
    struct hashTable *set = NULL;
    for(int i=0; i<strsSize; i++){
        int len = strlen(strs[i]);
        char strtmp[len+1];
        memset(strtmp, 0, sizeof(char) * (len+1));
        memcpy(strtmp, strs[i], sizeof(char) * len);
        // qsort tmp
        qsort(strtmp, len, sizeof(char), cmp);
        struct hashTable *tmp;
        HASH_FIND_STR(set, strtmp, tmp);
        if(tmp == NULL) {
            // HASH表中没有
            tmp= (struct hashTable *)malloc(sizeof(struct hashTable));
            tmp->key = (char*)calloc(len + 1, sizeof(char));
            memset(tmp->key, 0, sizeof(char) * (len + 1));
            memcpy(tmp->key, strtmp, sizeof(char) * len);
            tmp->index = *returnSize;
            tmp->cnt = 1;
            //printf("%d",1);
            HASH_ADD_STR(set, key, tmp);
            //HASH_ADD_KEYPTR( hh, set, tmp->key, strlen(tmp->key), tmp);
            //HASH_ADD(hh, set, key[0], strlen(tmp->key), tmp);
            res[*returnSize] = (char **)malloc(sizeof(char*) * strsSize);
            //printf("%d",2);
            res[*returnSize][tmp->cnt-1] = (char *)malloc(sizeof(char) * (len + 1));
            
            memset(res[*returnSize][tmp->cnt-1], 0, sizeof(char) * (len + 1));
            memcpy(res[*returnSize][tmp->cnt-1], strs[i], sizeof(char) * len);
            (*returnColumnSizes)[*returnSize] = tmp->cnt;
            (*returnSize)++;
        }
        else {
            // HASH表中有记录
            res[tmp->index][tmp->cnt] = (char *)malloc(sizeof(char) * (len + 1));
            memset(res[tmp->index][tmp->cnt], 0, sizeof(char) * (len + 1));
            memcpy(res[tmp->index][tmp->cnt], strs[i], sizeof(char) * len);
            tmp->cnt += 1;
            (*returnColumnSizes)[tmp->index] = tmp->cnt;

        }
    }

    return res;

}
/*
char *** groupAnagrams(char ** strs, int strsSize, int* returnSize, int** returnColumnSizes){
    if (strsSize == 0) {
        return NULL;
    }
    char*** res = (char***)calloc(strsSize, sizeof(char**));
    *returnSize = 0;
    *returnColumnSizes = (int*)calloc(strsSize, sizeof(int));
    struct hashTable* set = NULL;
    for (int i = 0; i < strsSize; i++) {
        int len = strlen(strs[i]);
        char strtmp[len + 1];
        memset(strtmp, 0, sizeof(char) * (len+1));
        memcpy(strtmp, strs[i], sizeof(char) * strlen(strs[i]));
        qsort(strtmp, len, sizeof(char), cmp);
        struct hashTable* tmp;
        //HASH_FIND(hh, set, strtmp, sizeof(char), tmp);
        HASH_FIND_STR(set, strtmp, tmp);
        if (tmp == NULL) {
            tmp = (struct hashTable*)calloc(1, sizeof(struct hashTable));
            tmp->key = (char*)calloc(len + 1, sizeof(char));
            memset(tmp->key, 0, sizeof(char) * (len+1));
            memcpy(tmp->key, strtmp, sizeof(char) * len);
            tmp->index = *returnSize;
            tmp->cnt = 1;
            HASH_ADD(hh, set, key, sizeof(char), tmp);
            //HASH_ADD_STR(set, key, tmp);
            res[*returnSize] = (char**)calloc(strsSize, sizeof(char*));
            res[*returnSize][tmp->cnt - 1] = (char*)calloc(len + 1, sizeof(char));
            memset(res[*returnSize][tmp->cnt - 1], 0, sizeof(char) * (len+1));
            memcpy(res[*returnSize][tmp->cnt - 1], strs[i], sizeof(char) * len);
            (*returnColumnSizes)[*returnSize] = tmp->cnt;
            (*returnSize)++;
        }
        else {
            printf("%d",1);
            res[tmp->index][tmp->cnt] = (char*)calloc(len + 1, sizeof(char));
            memset(res[*returnSize][tmp->cnt - 1], 0, sizeof(char) * (len+1));
            memcpy(res[tmp->index][tmp->cnt], strs[i], strlen(strs[i]));
            tmp->cnt += 1;
            (*returnColumnSizes)[tmp->index] = tmp->cnt;
        }
    }
    return res;
}*/

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
#define STR_SIZE 100

typedef struct Node{
    char str[STR_SIZE]; // key为字符串
    int row;            // value为结果所在的行
    struct Node * next;
} HashNode;


int hash(char * str, int size)
{
    long h = 0;
    for(int i = 0; i < strlen(str); i++){
        h = (h * 26 % size + str[i] - 'a') % size; // 字符串的hashcode, 权为26是因为小写字母，不限制时为128，这样能够让结点尽可能分布均匀，减少地址冲突
        // 取模是为了防止int型溢出
    }
    return h % size;
}

bool contain(HashNode * hashtable, char * str, int size)
{
    HashNode *head = &hashtable[hash(str, size)];
    HashNode *tail = head->next;
    while(tail){
        if(strcmp(tail->str, str) == 0) return true;
        tail = tail->next;
    }
    return false;
}

void add(HashNode * hashtable, char * str, int size, int row)
{
    if(contain(hashtable, str, size)) return;
    HashNode *head = &hashtable[hash(str, size)];
    // 头插建表
    HashNode *q = malloc(sizeof(HashNode));
    strcpy(q->str, str);
    q->row = row;
    q->next = head->next;
    head->next = q;
}


int getRows(HashNode * hashtable, char * str, int size)
{
    HashNode *head = &hashtable[hash(str, size)];
    HashNode *tail = head->next;
    while(tail){
        if(strcmp(tail->str, str) == 0) return tail->row;
        tail = tail->next;
    }
    return -1;
}

int cmp(const void *a, const void * b)
{
    return *(char *)a - *(char *)b;
}

char *** groupAnagrams(char ** strs, int strsSize, int* retSize, int** columnSizes){
    if(strsSize == 0 || strs == NULL){
        *retSize = 0;
        return NULL;
    }
    HashNode * hashtable = malloc(sizeof(HashNode) * strsSize);
    memset(hashtable, 0, sizeof(HashNode) * strsSize);
    char *** ans = malloc(sizeof(char **) * strsSize);

    *retSize = 0;
    *columnSizes = malloc(sizeof(int) * strsSize);

    for(int i = 0; i < strsSize; i++){
        char curStr[STR_SIZE] = "";
        strcpy(curStr, strs[i]);
        int lenStr = strlen(curStr);
        qsort(curStr, lenStr, sizeof(char), cmp); // 排序

        if(contain(hashtable, curStr, strsSize)){ // key已经存在了
            int row = getRows(hashtable, curStr, strsSize); // 获得之前key的结果所在行
            int col = (*columnSizes)[row];
            ans[row][col] = malloc(sizeof(char) * (lenStr + 1));
            strcpy(ans[row][col], strs[i]);
            (*columnSizes)[row]++;
        } else{ // key 不存在
            add(hashtable, curStr, strsSize, *retSize); // 插入表中
            ans[*retSize] = malloc(sizeof(char *) * strsSize); // 申请一行
            ans[*retSize][0] = malloc(sizeof(char) * (lenStr + 1));
            strcpy(ans[*retSize][0], strs[i]);
            (*columnSizes)[*retSize] = 1;
            (*retSize)++;
        }
    }
    return ans;
}
