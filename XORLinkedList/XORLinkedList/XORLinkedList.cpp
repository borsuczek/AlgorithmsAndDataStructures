#include <iostream>
#include <string>

using namespace std;

#define ZERO_IN_ASCII 48
#define NINE_IN_ASCII 57

struct list {
    list* pxor = NULL;
    int val = NULL;
};

struct line {
    int N = 0;
    string f = "";
};


list* XOR(list* x, list* y) {
    return (list*)((uintptr_t)(x) ^ (uintptr_t)(y));
}


int find_N(string f, int i, int N) {
    if (f[i] <= NINE_IN_ASCII && f[i] >= ZERO_IN_ASCII)
        return find_N(f, i + 1, 10 * N + f[i] - ZERO_IN_ASCII);
    else
        return N;
}


line cin_line(string function) {
    line l;
    int i = 0;

    while (function[i] != ' ' && function[i] != NULL) {
        l.f += function[i];
        i++;
    }
    if (i < int(function.length())) {
        while (function[i] == ' ')
            i++;
        l.N = find_N(function, i + 1, function[i] - ZERO_IN_ASCII);
    }
    return l;
}


list* ADD(list* beg, list** end, int N, list** actual) {
    list* new_beg = new list();
    new_beg->pxor = beg;
    new_beg->val = N;

    if (beg != NULL) {
        beg->pxor = XOR(new_beg, beg->pxor);
    }
    else {
        *end = new_beg;
        *actual = new_beg;
    }
    return new_beg;
}


void ADD_ACT(list** beg, list** end, int N, list** actual) {
    if (*actual == NULL)
        return;
    if (*actual == *beg) {
        *beg = ADD(*beg, end, N, actual);
        return;
    }

    list* new_val = new list();
    list* current = *beg;
    list* prev = NULL;
    list* next = XOR(current->pxor, prev);

    while (next->pxor != NULL) {
        if (*actual == next) {
            new_val->val = N;
            new_val->pxor = XOR(current, next);
            current->pxor = XOR(prev, new_val);
            next->pxor = XOR(XOR(next->pxor, current), new_val);
            return;
        }
        prev = current;
        current = next;
        next = XOR(current->pxor, prev);
    }
}


void PRINT(list* beg) {
    list* prev = NULL;
    list* current = beg;
    list* next = NULL;

    if (current == NULL) {
        cout << "NULL";
    }
    while (current != NULL) {
        cout << current->val << " ";
        next = XOR(current->pxor, prev);
        prev = current;
        current = next;
    }
    cout << endl;
}


list* DEL_BEG(list* beg, list** actual, list** end) {
    if (beg == NULL)
        return NULL;
    if (beg->pxor == NULL) {
        *actual = NULL;
        *end = NULL;
        free(beg);
        return NULL;
    }
    if (*actual == beg)
        *actual = *end;

    list* new_beg = beg->pxor;
    new_beg->pxor = XOR(beg, new_beg->pxor);
    free(beg);
    return new_beg;
}


list* DEL_END(list* end, list** actual, list** beg) {
    if (end == NULL)
        return NULL;
    if (end->pxor == NULL) {
        *actual = NULL;
        *beg = NULL;
        free(end);
        return NULL;
    }
    if (*actual == end)
        *actual = end->pxor;

    list* new_end = end->pxor;
    new_end->pxor = XOR(end, new_end->pxor);
    free(end);
    return new_end;
}


void DEL_ACT(list** beg, list** end, list** actual) {
    if (*beg == NULL)
        return;
    else if (*actual == *beg) {
        *beg = DEL_BEG(*beg, actual, end);
    }
    else if (*actual == *end) {
        *end = DEL_END(*end, actual, beg);
    }
    else {
        list* current = (*beg)->pxor;
        list* prev = *beg;
        list* next = XOR(current->pxor, prev);

        while (next != NULL) {
            if (current == *actual) {
                next->pxor = XOR(XOR(current, next->pxor), prev);
                prev->pxor = XOR(XOR(current, prev->pxor), next);
                *actual = prev;
                free(current);
                return;
            }
            prev = current;
            current = next;
            next = XOR((current)->pxor, prev);
        }
    }
}


void DEL_VAL(list** beg, list** end, int N, list** actual) {
    list* prev = NULL;
    list* next = NULL;
    list* current = *beg;

    while (current != NULL) {
        if (N == (*end)->val) {
            *end = DEL_END(*end, actual, beg);
        }
        if (prev == NULL && current->val == N) {

            *beg = DEL_BEG(*beg, actual, end);
            current = *beg;
        }
        else if (*beg != NULL) {
            next = XOR(current->pxor, prev);
            if (next != NULL) {
                if (current->val == N) {
                    next->pxor = XOR(XOR(current, next->pxor), prev);
                    prev->pxor = XOR(XOR(current, prev->pxor), next);
                    if (*actual == current)
                        *actual = prev;
                    free(current);
                    current = prev;
                }
            }
            prev = current;
            current = next;
        }
        else
            return;
    }
}


list* NEXT(list* actual, list* beg) {
    list* prev = NULL;
    list* next = NULL;
    list* current = beg;

    if (current == NULL) {
        cout << "NULL" << endl;
    }
    while (current != NULL) {
        next = XOR(current->pxor, prev);
        if (actual == current) {
            if (next != NULL) {
                cout << next->val << endl;
                return next;
            }
            else {
                cout << beg->val << endl;
                return beg;
            }
        }
        prev = current;
        current = next;
    }
    return NULL;
}


void ACTUAL(list* actual) {
    if (actual != NULL) {
        cout << actual->val << endl;
    }
    else
        cout << "NULL" << endl;
}


int main()
{
    string function;
    list* beg = NULL;
    list* end = NULL;
    list* actual = NULL;

    while (getline(cin, function)) {
        line data = cin_line(function);
        int N = data.N;
        function = data.f;

        if (function == "ACTUAL") {
            ACTUAL(actual);
        }
        if (function == "NEXT") {
            actual = NEXT(actual, beg);
        }
        if (function == "PREV") {
            actual = NEXT(actual, end);
        }
        if (function == "ADD_BEG") {
            beg = ADD(beg, &end, N, &actual);
        }
        if (function == "ADD_END") {
            end = ADD(end, &beg, N, &actual);
        }
        if (function == "ADD_ACT") {
            ADD_ACT(&beg, &end, N, &actual);
        }
        if (function == "DEL_BEG") {
            beg = DEL_BEG(beg, &actual, &end);
        }
        if (function == "DEL_END") {
            end = DEL_END(end, &actual, &beg);
        }
        if (function == "DEL_VAL") {
            DEL_VAL(&beg, &end, N, &actual);
        }
        if (function == "DEL_ACT") {
            DEL_ACT(&beg, &end, &actual);
        }
        if (function == "PRINT_FORWARD") {
            PRINT(beg);
        }
        if (function == "PRINT_BACKWARD") {
            PRINT(end);
        }
    }
    return 0;
}