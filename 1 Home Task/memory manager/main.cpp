#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>
#include <map>
#include <algorithm>

struct pair_vertex;

struct vertex {
    //left > right
    vertex* left;
    vertex* right;
    long long key;
    long long priority;
    pair_vertex* ver;
    vertex(long long k, long long p, pair_vertex* a) : left(nullptr), right(nullptr), key(k), priority(p), ver(a) {}
};

void split(vertex* my_vertex, double input_i, vertex*& l, vertex*& r) {
    if (!my_vertex) {
        l = nullptr;
        r = nullptr;
        return;
    } else if (my_vertex->key < input_i) {
        split(my_vertex->right, input_i, my_vertex->right, r);
        l = my_vertex;
    } else {
        split(my_vertex->left, input_i, l, my_vertex->left);
        r = my_vertex;
    }
}

//merge l and r to vertex with root in my
void merge(vertex*& my, vertex* l, vertex* r) {
    if (!l || !r) {
        my = l ? l : r;
    }
    else if (l->priority < r->priority) {
        merge(r->left, l, r->left);
        my = r;
    } else{
        merge(l->right, l->right, r);
        my = l;
    }
}

//insert a to my
void insert(vertex*& my, vertex* a) {
    if (my == nullptr)
        my = a;
    else if (a->priority > my->priority) {
        split(my, a->key, a->left, a->right);
        my = a;
    } else {
        insert(my->key > a->key ? my->left : my->right, a);
    }
}
void erase(vertex*& my, long long a) {
    if (my->key == a) {
        vertex* x = my;
        merge(my, my->left, my->right);
        delete x;
        x = nullptr;
    }
    else {
        erase(my->key < a ? my->right : my->left, a);
    }
}
/*long long find(long long index, vertex* a) {
    if (index == count_left)
        return a->key;
    if (index < count_left)
        return find(index, a->left);
    return find(index - count_left, a->right);
}*/
void delete_vertex(vertex*& my) {
    if (!my)
        return;
    if (my->left)
        delete_vertex(my->left);
    if (my->right)
        delete_vertex(my->right);
    delete my;
    my = nullptr;
}

void print_vertex(vertex* a) {
    if(! a) {
        return;
    } if(a->left) {
        print_vertex(a->left);
    }
    std::cout << a->key << ' ';
    if (a->right) {
        print_vertex(a->right);
    }
}


struct pair_vertex {
    //left > right
    pair_vertex* left;
    pair_vertex* right;
    std::pair<long long, long long> key;
    long long priority;
    pair_vertex(std::pair<long long, long long> k, long long p) : left(nullptr), right(nullptr), key(k), priority(p) {}
};

bool more(std::pair<long long, long long> a, std::pair<long long, long long> b) {
    if (a.first > b.first)
        return true;
    if (a.first < b.first)
        return false;
    return a.second < b.second;
}
bool less(std::pair<long long, long long> a, std::pair<long long, long long> b) {
    return more(b,a);
}


void split(pair_vertex* my_vertex, std::pair<long long, long long> input_key, pair_vertex*& l, pair_vertex*& r) {
    if (! my_vertex) {
        l = nullptr;
        r = nullptr;
        return;
    } else if (less(my_vertex->key, input_key)) {
        split(my_vertex->right, input_key, my_vertex->right, r);
        l = my_vertex;
    } else {
        split(my_vertex->left, input_key, l, my_vertex->left);
        r = my_vertex;
    }
}

//merge l and r to pair_vertex with root in my
void merge(pair_vertex*& my, pair_vertex* l, pair_vertex* r) {
    if (!l || !r) {
        my = l ? l : r;
    }
    else if (l->priority < r->priority) {
        merge(r->left, l, r->left);
        my = r;
    } else{
        merge(l->right, l->right, r);
        my = l;
    }
}

//insert a to my
void insert(pair_vertex*& my, pair_vertex* a) {
    if (! my)
        my = a;
    else if (a->priority > my->priority) {
        split(my, a->key, a->left, a->right);
        my = a;
    } else {
        insert(more(my->key, a->key) ? my->left : my->right, a);
    }
}
void erase(pair_vertex*& my, std::pair<long long, long long> a) {
    if (my->key == a) {
        pair_vertex* x = my;
        merge(my, my->left, my->right);
        delete x;
        x = nullptr;
    }
    else {
        erase(less(my->key, a) ? my->right : my->left, a);
    }
}
pair_vertex* find(long long index, pair_vertex* a) {
    pair_vertex* answer = nullptr;
    if (!a)
        return a;
    answer = find(index, a->right);
    if (! answer)
        return a;
    return answer;
}

void delete_vertex(pair_vertex*& my) {
    if (!my)
        return;
    if (my->left)
        delete_vertex(my->left);
    if (my->right)
        delete_vertex(my->right);
    delete my;
    my = nullptr;
}
vertex* min(vertex* my) {
    if (!my || ! my->left)
        return my;
    return min(my->left);
}

vertex* max(vertex* my) {
    if (!my || ! my->right)
        return my;
    return max(my->right);
}

void needMerge(vertex*& my, vertex* a, pair_vertex*& root) {
    vertex* left = nullptr;
    vertex* right = nullptr;
    vertex* center = nullptr;
    split(my, a->key - 0.5, left, right);
    split(right, a->key + 0.5, center, right);
    vertex* ll = max(left);
    vertex* rr = min(right);
    bool first_state = false, second_state = false;
    if (ll && ll->key + ll->ver->key.first == a->key)
        first_state = true;
    if (rr && a->key + a->ver->key.first == rr->key)
        second_state = true;
    if (first_state && second_state) {
        long long af = a->ver->key.first + rr->ver->key.first + ll->ver->key.first;
        long long bf = ll->key;
        pair_vertex* new_ver = new pair_vertex(std::make_pair(af, bf), std::rand());
        erase(root, a->ver->key);
        erase(root, ll->ver->key);
        erase(root, rr->ver->key);
        insert(root, new_ver);
        vertex* ve = new vertex(new_ver->key.second, std::rand(), new_ver);
        erase(center, a->key);
        erase(left, ll->key);
        erase(right, rr->key);
        insert(center, ve);
    } else if (first_state) {
        long long af = a->ver->key.first + ll->ver->key.first;
        long long bf = ll->key;
        pair_vertex* new_ver = new pair_vertex(std::make_pair(af, bf), std::rand());
        erase(root, a->ver->key);
        erase(root, ll->ver->key);
        insert(root, new_ver);
        vertex* ve = new vertex(new_ver->key.second, std::rand(), new_ver);
        erase(center, a->key);
        erase(left, ll->key);
        insert(center, ve);
    } else if (second_state) {
        long long af = a->ver->key.first + rr->ver->key.first;
        long long bf = a->key;
        pair_vertex* new_ver = new pair_vertex(std::make_pair(af, bf), std::rand());
        erase(root, a->ver->key);
        erase(root, rr->ver->key);
        insert(root, new_ver);
        vertex* ve = new vertex(new_ver->key.second, std::rand(), new_ver);
        erase(center, a->key);
        erase(right, rr->key);
        insert(center, ve);
    }
    merge(center, left, center);
    merge(my, center, right);
}


int main() {
    long long n, m;
    std::cin >> n >> m;
    long long query[m + 5][2];
    for (int j = 0; j < m+5; ++j) {
        query[j][0] = 0;
        query[j][1] = 0;
    }
    pair_vertex* root = new pair_vertex(std::make_pair<long long, long long>(std::move(n), 1), std::rand());
    vertex* merge_root = new vertex(root->key.second, std::rand(), root);
    for (int i = 1; i < m + 1; ++i) {
        long long k;
        std::cin >> k;
        if (k > 0) {
            pair_vertex* find_v = find(k, root);
            if (!find_v || find_v->key.first < k) {
                std::cout << "-1" << '\n';
                continue;
            }
            query[i][0] = k;
            query[i][1] = find_v->key.second;
            std::cout << find_v->key.second << '\n';
            pair_vertex* new_2 = new pair_vertex(std::make_pair(find_v->key.first - k, find_v->key.second + k), std::rand());
            if (new_2->key.first == 0) {
                delete new_2;
                new_2 = nullptr;
                erase(merge_root, find_v->key.second);
                erase(root, find_v->key);
                continue;
            }
            insert(root, new_2);

            vertex* n_2 = new vertex(new_2->key.second, std::rand(), new_2);
            insert(merge_root, n_2);

            erase(merge_root, find_v->key.second);
            erase(root, find_v->key);
        } else {
            k *= -1;
            long long a = query[k][0], b = query[k][1];
            if (a == 0)
                continue;
            pair_vertex* new_ver = new pair_vertex(std::make_pair(a, b), std::rand());
            vertex* ve = new vertex(b, std::rand(), new_ver);
            if (! root) {
                root = new_ver;
                merge_root = ve;
                continue;
            }
            insert(merge_root, ve);
            insert(root, new_ver);

            needMerge(merge_root, ve, root);
        }
    }
    delete_vertex(root);
    delete_vertex(merge_root);
}