#include <iostream>
#include <string>

using namespace std;

#define ZERO_IN_ASCII 48
#define NINE_IN_ASCII 57

struct list {
    list* prev = NULL;
    list* next = NULL;
    int val = NULL;
};

struct line {
    int N = 0;
    string f = "";
};


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


int SIZE(list* beg) {
    if (beg == NULL)
        return 0;
    int amount = 1;
    list* tmp = beg->next;
    while (tmp != beg) {
        amount++;
        tmp = tmp->next;
    }
    return amount;
}


int COUNT(list* front, list* back) {
    if (front == NULL)
        return 0;
    int amount = 1;
    list* tmp = front;
    while (tmp != back) {
        amount++;
        tmp = tmp->prev;
    }
    return amount;
}


list* ADD_BEG(list* beg, list** end, int N) {
    list* new_beg = new list();
    new_beg->val = N;
    new_beg->next = beg;
    new_beg->prev = *end;
    if (beg != NULL) {
        (*end)->next = new_beg;
        (beg)->prev = new_beg;
    }
    else {
        *end = new_beg;
        new_beg->next = new_beg;
        new_beg->prev = new_beg;
    }
    return new_beg;
}


list* ADD_END(list* end, list** beg, int N) {
    list* new_beg = new list();
    new_beg->val = N;
    new_beg->prev = end;
    new_beg->next = *beg;
    if (end != NULL) {
        (*beg)->prev = new_beg;
        end->next = new_beg;
    }
    else {
        *beg = new_beg;
        new_beg->next = new_beg;
        new_beg->prev = new_beg;
    }
    return new_beg;
}


void PRINT(list* beg, list* end, const string& way) {
    list* current = beg;
    if (current == NULL) {
        cout << "NULL";
    }
    else {
        while (current != end) {
            cout << current->val << " ";
            if (way == "prev")
                current = current->prev;
            else
                current = current->next;
        }
        cout << end->val;
    }
    cout << endl;
}


list* DEL_BEG(list* beg, list** end, list** front, list** back) {
    if (beg == NULL)
        return NULL;
    if (beg == *end) {
        *end = NULL;
        if (*front != NULL) {
            *front = NULL;
            *back = NULL;
        }
        free(beg);
        return NULL;
    }
    list* new_beg = beg->next;
    new_beg->prev = *end;
    (*end)->next = new_beg;
    if (*front == beg && *front != *back)
        *front = (*front)->prev;
    else if (*back == beg && *front != *back)
        *back = (*back)->next;
    else if (*back == beg && *front == *back) {
        *back = NULL;
        *front = NULL;
    }
    free(beg);
    return new_beg;
}


list* DEL_END(list* end, list** beg, list** front, list** back) {
    if (end == NULL)
        return NULL;
    if (end == *beg) {
        *beg = NULL;
        if (*front != NULL) {
            *front = NULL;
            *back = NULL;
        }
        free(end);
        return NULL;
    }
    list* new_end = end->prev;
    new_end->next = *beg;
    (*beg)->prev = new_end;
    if (*front == end && *front != *back)
        *front = (*front)->prev;
    else if (*back == end && *front != *back)
        *back = (*back)->next;
    else if (*back == end && *front == *back) {
        *back = NULL;
        *front = NULL;
    }
    free(end);
    return new_end;
}


void PUSH(list** front, list** back, list** beg, list** end, int N) {
    if (*beg == NULL) {
        *beg = ADD_BEG(*beg, end, N);
        *front = *end;
        *back = *beg;
    }
    else if (*front == NULL) {
        (*end)->val = N;
        *front = *end;
        *back = *end;
    }
    else if ((*back)->prev == (*front)) {
        if (*back == *beg) {
            *beg = ADD_BEG(*beg, end, N);
            *back = *beg;
        }
        else {
            list* new_elem = new list();
            new_elem->val = N;
            new_elem->next = *back;
            new_elem->prev = (*back)->prev;
            (*back)->prev->next = new_elem;
            (*back)->prev = new_elem;
            *back = new_elem;
        }
    }
    else {
        (*back)->prev->val = N;
        *back = (*back)->prev;
    }
}


void POP(list** front, list** back) {
    if (*front == NULL)
        cout << "NULL" << endl;
    else {
        cout << (*front)->val << endl;
        if (*front == *back) {
            *front = NULL;
            *back = NULL;
        }
        else
            *front = (*front)->prev;
    }
}


void GARBAGE_SOFT(list* front, list* back, list* beg) {
    if (front == NULL) {
        if (beg != NULL) {
            beg->val = 0;
            list* tmp = beg->next;
            while (tmp != beg) {
                tmp->val = 0;
                tmp = tmp->next;
            }
        }
        return;
    }
    list* tmp = back;
    while (tmp->prev != front) {
        tmp->prev->val = 0;
        tmp = tmp->prev;
    }
}


void GARBAGE_HARD(list* front, list* back, list** beg, list** end) {
    if (front == NULL) {
        while (*beg != NULL) {
            *beg = DEL_BEG(*beg, end, &front, &back);
        }
        return;
    }
    list* tmp = back->prev;
    while (tmp != front) {
        if (tmp == *end)
            *end = DEL_END(*end, beg, &front, &back);
        else if (tmp == *beg)
            *beg = DEL_BEG(*beg, end, &front, &back);
        else {
            back->prev = tmp->prev;
            tmp->prev->next = back;
            free(tmp);
        }
        tmp = back->prev;
    }
}


int main()
{
    string function;
    list* beg = NULL;
    list* end = NULL;
    list* front = NULL;
    list* back = NULL;

    while (getline(cin, function)) {
        line data = cin_line(function);
        int N = data.N;
        function = data.f;

        if (function == "ADD_BEG") {
            beg = ADD_BEG(beg, &end, N);
        }
        if (function == "ADD_END") {
            end = ADD_END(end, &beg, N);
        }
        if (function == "DEL_BEG") {
            beg = DEL_BEG(beg, &end, &front, &back);
        }
        if (function == "DEL_END") {
            end = DEL_END(end, &beg, &front, &back);
        }
        if (function == "PRINT_FORWARD") {
            PRINT(beg, end, "next");
        }
        if (function == "PRINT_BACKWARD") {
            PRINT(end, beg, "prev");
        }
        if (function == "PUSH") {
            PUSH(&front, &back, &beg, &end, N);
        }
        if (function == "POP") {
            POP(&front, &back);
        }
        if (function == "PRINT_QUEUE") {
            PRINT(front, back, "prev");
        }
        if (function == "SIZE") {
            cout << SIZE(beg) << endl;
        }
        if (function == "COUNT") {
            cout << COUNT(front, back) << endl;
        }
        if (function == "GARBAGE_SOFT") {
            GARBAGE_SOFT(front, back, beg);
        }
        if (function == "GARBAGE_HARD") {
            GARBAGE_HARD(front, back, &beg, &end);
        }
    }
    return 0;
}