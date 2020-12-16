#include <stdio.h>

char numsht[10] = {'0','1','2','3','4','5','6','7','8','9'};
typedef const char* ccharptr;
ccharptr prsht[3] = {"+-","*",nullptr};
char inbuf[128];
char outbuf[256];
int calcstk[64];

int tree(char* in, char* out);

int chr2num(char in);

int readnum(char** in);

int getlevel(char in);

int eval(char* in);

int main(){
    scanf_s("%s",inbuf,128);
    char* entry = outbuf;
    char* ptr = outbuf;
    tree(inbuf, outbuf);
    while(*ptr){
        if(*ptr == 'p'){
            printf("%d ", *((int*)(++ptr)));
            ptr += sizeof(int);
        } else {
            printf("%c ", *(ptr++));
        }
    }
    printf("\n");
    printf("%d", eval(outbuf));
}

char* _tree(char* input, char** out, int plvl){
    char* entry = *out;
    char curop = 0;
    while(*input && *input != ')'){
        if(*input == ' '){
            input++;
            continue;
        }
        int curnum = readnum(&input);
        if(curnum != -1){
            *(entry++) = 'p';
            *((int*)entry) = curnum;
            entry += sizeof(int);
        } else if (*input == '('){
            input = _tree(input + 1, &entry, 0);
            input++;
        } else {
            int nextlvl = getlevel(*input);
            if(nextlvl > plvl){
                input = _tree(input, &entry, nextlvl);
            }
            if(curop){
                *(entry++) = curop;
                curop = 0;
            }
            if(nextlvl < plvl){
                *out = entry;
                return input;
            }
            if(*input != ')')
                curop = *(input++);
        }
    }
    if(curop)
        *(entry++) = curop;
    *out = entry;
    return input;
}

int tree(char* in, char* out){
    _tree(in, &out, 0);
    return 0;
}

int eval(char* in){
    int stkptr = -1;
    while(*in){
        if(*in == 'p'){
            calcstk[++stkptr] = *((int*)(++in));
            in += sizeof(int);
        } else {
            if(*in == '+'){
                int rparam = calcstk[stkptr--];
                int lparam = 0;
                if(stkptr < 0)
                    stkptr = 0;
                else
                    lparam = calcstk[stkptr];
                calcstk[stkptr] = lparam + rparam;
            } else if (*in == '-'){
                int rparam = calcstk[stkptr--];
                int lparam = 0;
                if(stkptr < 0)
                    stkptr = 0;
                else
                    lparam = calcstk[stkptr];
                calcstk[stkptr] = lparam - rparam;
            } else if (*in == '*'){
                int rparam = calcstk[stkptr--];
                int lparam = calcstk[stkptr];
                calcstk[stkptr] = lparam * rparam;
            }
            in++;
        }
    }
    return *calcstk;
}

int chr2num(char in){
    for(int i = 0; i < 10; i++){
        if(numsht[i] == in)
            return i;
    }
    return -1;
}

int readnum(char** in){
    int curnum = chr2num(**in);
    if(curnum == -1)
        return -1;
    int num = 0;
    while(curnum != -1){
        num = num * 10 + curnum;
        curnum = chr2num(*(++(*in)));
    }
    return num;
}

int getlevel(char in){
    int lvl = 0;
    while(prsht[lvl]){
        int idx = 0;
        char chr = *(prsht[lvl] + idx);
        while(chr){
            if(chr == in)
                return lvl;
            idx++;
            chr = *(prsht[lvl] + idx);
        }
        lvl++;
    }
    return -1;
}
