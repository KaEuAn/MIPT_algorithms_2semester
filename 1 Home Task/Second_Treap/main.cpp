#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>


struct vertex {
    //left > right
    vertex* left;
    vertex* right;
    long long key;
    long long priority;
    long long sum_behind;
    vertex(long long k, long long p) : left(nullptr), right(nullptr), key(k), priority(p), sum_behind(1) {}
    void make_sum();
};

long long sum_behind_function(vertex *a) {
    return a ? a->sum_behind : 0;
}
void vertex::make_sum() {
    sum_behind = sum_behind_function(left) + sum_behind_function(right) + 1;
}

void split(vertex* my_vertex, double input_i, vertex*& l, vertex*& r) {
    if (my_vertex == nullptr) {
        l = nullptr;
        r = nullptr;
        return;
    } else if (sum_behind_function(my_vertex->left) < input_i ) {
        split(my_vertex->right, input_i - sum_behind_function(my_vertex->left) - 1, my_vertex->right, r);
        l = my_vertex;
        l->make_sum();
    } else {
        split(my_vertex->left, input_i, l, my_vertex->left);
        r = my_vertex;
        r->make_sum();
    }
    my_vertex->make_sum();
}

//merge l and r to vertex with root in my
void merge(vertex*& my, vertex* l, vertex* r) {
    if (!l || !r) {
        my = l ? l : r;
    }
    else if (l->priority < r->priority) {
        merge(r->left, l, r->left);
        my = r;
        my->make_sum();
    } else{
        merge(l->right, l->right, r);
        my = l;
        my->make_sum();
    }
}

//insert a to my
void insert(vertex*& my, vertex* a) {
    if (my == nullptr)
        my = a;
    else if (a->priority > my->priority) {
        split(my, a->key, a->left, a->right);
        my = a;
        my->make_sum();
    } else {
        insert(sum_behind_function(my) < sum_behind_function(a) ? my->left : my->right, a);
        my->make_sum();
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
        erase(my->key > a ? my->right : my->left, a);
        my->make_sum();
    }
}
long long find(long long index, vertex* a) {
    long long count_left = sum_behind_function(a->left) + 1;
    if (index == count_left)
        return a->key;
    if (index < count_left)
        return find(index, a->left);
    return find(index - count_left, a->right);
}
void delete_vertex(vertex*& my) {
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

int main() {
    int n, m;
    std::cin >> n >> m;
    vertex* root = new vertex(1, std::rand());
    for (int i = 2; i < n + 1; ++i) {
        vertex* ver = new vertex(i, std::rand());
        insert(root, ver);
    }
    for (int i = 0; i < m; ++i) {
        int l, r;
        std::cin >> l >> r;
        vertex* left = nullptr;
        vertex* right = nullptr;
        vertex* center = nullptr;
        split(root, l - 1.5, left, center);
        split(center, r - l + 0.5, center, right);
        merge(center, center, left);
        merge(center, center, right);
    }
    print_vertex(root);
    delete_vertex(root);
    return 0;
}