#include<iostream>
#include <string>
using namespace std;

#define ZERO_IN_ASCII 48
#define NINE_IN_ASCII 57

struct btree {
    int* keys;
    btree** sons;
    int T;
    int n;
    bool isLeaf;
};


btree* create(int T, bool isLeaf) {
    btree* node = new btree;
    node->keys = new int[2 * T - 1];
    node->sons = new btree * [2 * T];
    node->isLeaf = isLeaf;
    node->n = 0;
    node->T = T;
    return node;
}


void print(btree* node) {
    for (int i = 0; i < node->n + 1; i++) {
        if (node->isLeaf == false) {
            print(node->sons[i]);
        }
        if (i < node->n)
            cout << node->keys[i] << " ";
    }
}


void save(btree* node) {
    cout << "( ";
    for (int i = 0; i < node->n + 1; i++) {
        if (node->isLeaf == false) {
            save(node->sons[i]);
        }
        if (i < node->n)
            cout << node->keys[i] << " ";
    }
    cout << ") ";
}


void split(btree* p, int i, btree* w) {
    int T = p->T;
    btree* z = create(T, w->isLeaf);

    z->n = T - 1;
    w->n = T - 1;

    for (int j = p->n; j > i; j--) {
        p->keys[j] = p->keys[j - 1];
    }
    for (int j = p->n + 1; j > i; j--) {
        p->sons[j] = p->sons[j - 1];
    }
    p->keys[i] = w->keys[T - 1];
    p->n = p->n + 1;

    for (int j = 0; j < T; j++) {
        z->sons[j] = w->sons[T + j];
    }
    for (int j = 0; j < T - 1; j++) {
        z->keys[j] = w->keys[T + j];
    }

    p->sons[i] = w;
    p->sons[i + 1] = z;
}


int newNodePlace(btree* parent, int x) {
    int i = 0;
    while (i < parent->n) {
        if (parent->keys[i] > x)
            break;
        i++;
    }
    return i;
}


void add(btree* node, int i, int x) {
    int j = node->n;
    node->n = node->n + 1;
    while (j > i) {
        node->keys[j] = node->keys[j - 1];
        j--;
    }
    node->keys[i] = x;
}


void insertWithNotFullRoot(btree* parent, int x) {
    int index = newNodePlace(parent, x);

    if (parent->isLeaf == true) {
        add(parent, index, x);
    }
    else {
        if (parent->sons[index]->n == 2 * parent->T - 1)
        {
            split(parent, index, parent->sons[index]);
            index = newNodePlace(parent, x);
        }
        insertWithNotFullRoot(parent->sons[index], x);
    }
}


btree* insert(btree* root, int x) {
    if (root->n == 0) {
        root->keys[0] = x;
        root->n = root->n + 1;
    }
    else
    {
        if (root->n == 2 * root->T - 1) {
            btree* w = create(root->T, false);
            split(w, 0, root);
            root = w;
        }
        insertWithNotFullRoot(root, x);
    }
    return root;
}


void check(btree* node, bool* found, int x) {
    int index = newNodePlace(node, x);
    if (node->keys[index - 1] == x) {
        *found = true;
    }
    if (*found == false && node->isLeaf == false) {
        check(node->sons[index], found, x);
    }
}


int findKey(string f, int i, int key, int* index) {
    if (f[i] <= NINE_IN_ASCII && f[i] >= ZERO_IN_ASCII)
        return findKey(f, i + 1, 10 * key + f[i] - ZERO_IN_ASCII, index);
    else {
        *index = i;
        return key;
    }
}


btree* load(btree* root) {
    string tree;
    getline(cin, tree);
    getline(cin, tree);

    for (int i = 0; i < (int)tree.length(); i++) {
        if (tree[i] >= 48 && tree[i] <= 57) {
            int key = findKey(tree, i + 1, tree[i] - ZERO_IN_ASCII, &i);
            root = insert(root, key);
        }
    }

    return root;
}


void findDepth(btree* node, int x, int* depth) {
    int index = newNodePlace(node, x);
    *depth = *depth + 1;
    if (node->keys[index - 1] != x && node->isLeaf == false) {
        findDepth(node->sons[index], x, depth);
    }
}


bool isInCache(int* cache, int size, int key) {
    for (int i = 0; i < size; i++) {
        if (cache[i] == key) {
            return true;
        }
    }
    return false;
}


void calculateCache(btree* root) {
    int size, key, noCache = 0, Cache = 0;
    cin >> size;
    int* cache = new int[size]();

    while (cin >> key) {
        int depth = 0;
        findDepth(root, key, &depth);
        noCache += depth;

        if (isInCache(cache, size, key) == false) {
            Cache += depth;
            for (int i = 1; i < size; i++) {
                cache[i] = cache[i - 1];
            }
            cache[0] = key;
        }
    }
    cout << "NO CACHE: " << noCache << " CACHE: " << Cache;
    delete[] cache;
}


int main() {
    char letter;
    btree* root = create(1, true);
    while (cin >> letter) {
        int x;
        if (letter == 'I') {
            cin >> x;
            root = create(x, true);
        }
        if (letter == 'A') {
            cin >> x;
            root = insert(root, x);
        }
        if (letter == '?') {
            cin >> x;
            bool found = false;
            check(root, &found, x);
            if (found == false)
                cout << x << " -" << endl;
            else
                cout << x << " +" << endl;
        }
        if (letter == 'P') {
            print(root);
            cout << endl;
        }
        if (letter == 'S') {
            cout << root->T << endl;
            save(root);
            cout << endl;
        }
        if (letter == 'L') {
            cin >> x;
            root = create(x, true);
            root = load(root);
        }
        if (letter == 'C') {
            calculateCache(root);
        }
        if (letter == 'X') {
            return 0;
        }
    }
}