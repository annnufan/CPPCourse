//
// Created by Anna Kopeliovich(annnufan@gmail.com, nufan@list.ru) on 04.10.2016.
//

#include "persistent_set.h"

typedef int value_type;

#include <cassert>
#include <iostream>

// Ñîçäàåò ïóñòîé persistent_set.
persistent_set::persistent_set() {
    root = new node();
}

// Ñîçäàåò êîïèþ óêàçàííîãî persistent_set-à.
persistent_set::persistent_set(persistent_set const &rhs) {
    assert(rhs.root != nullptr);
    root = new node();
    root->left = rhs.root->left;
}

// Èçìåíÿåò this òàê, ÷òîáû îí ñîäåðæàë òå æå ýëåìåíòû, ÷òî è rhs.
// Èíâàëèäèðóåò âñå èòåðàòîðû, ïðèíàäëåæàùèå persistent_set'ó this, âêëþ÷àÿ end().
persistent_set& persistent_set::operator=(persistent_set const& rhs) {
    if (root != nullptr) {
        root->dec_node();
        root->valid = false;
    }
    root = new node();
    root->left = rhs.root->left;
    return *this;
}

// Äåñòðóêòîð. Âûçûâàåòñÿ ïðè óäàëåíèè îáúåêòîâ persistent_set.
// Èíâàëèäèðóåò âñå èòåðàòîðû ññûëàþùèåñÿ íà ýëåìåíòû ýòîãî persistent_set
// (âêëþ÷àÿ èòåðàòîðû ññûëàþùèåñÿ íà ýëåìåíòû ñëåäóþùèå çà ïîñëåäíèìè).
persistent_set::~persistent_set() {
    assert(root != nullptr);
    root->dec_node();
    root->valid = false;
    root = nullptr;
}


// Ïîèñê ýëåìåíòà.
// Âîçâðàùàåò èòåðàòîð íà íàéäåííûé ýëåìåíò, ëèáî end().
persistent_set::iterator persistent_set::find(value_type val) {
    assert(root != nullptr);
    node* now_node = root;
    while (now_node != nullptr && now_node->get_value() != val) {
        if (now_node->get_value() < val) {
            now_node = now_node->right;
        }
        else {
            now_node = now_node->left;
        }
    }
    iterator ans;
    if (now_node == nullptr)
        ans = iterator(root, root);
    else
        ans = iterator(now_node, root);
    return ans;
}


// Âñòàâêà ýëåìåíòà.
// 1. Åñëè òàêîé êëþ÷ óæå ïðèñóòñòâóåò, âñòàâêà íå ïðîèçâîäèòüñÿ, âîçâðàùàåòñÿ èòåðàòîð
//    íà óæå ïðèñóòñòâóþùèé ýëåìåíò è false.
// 2. Åñëè òàêîãî êëþ÷à åù¸ íåò, ïðîèçâîäèòüñÿ âñòàâêà, âîçâðàùàåòñÿ èòåðàòîð íà ñîçäàííûé
//    ýëåìåíò è true.
// Èíâàëèäèðóåò âñå èòåðàòîðû, ïðèíàäëåæàùèå persistent_set'ó this, âêëþ÷àÿ end().
std::pair<persistent_set::iterator, bool> persistent_set::insert(value_type val) {
    assert(root != nullptr);
    iterator f = find(val);
    if (f.value != root) {
        return std::make_pair(f, false);
    }
    node* add_node = new node(val);
    root->valid = false;
    root = insert_value(root, add_node);
    return std::make_pair(iterator(add_node, root), true);
};

//Ôóíêöèÿ, äîáàâëÿþùàÿ íîâûé ýëåìåíò â íàø ñåò
persistent_set::node* persistent_set::insert_value(node* v, node* add_node) {
    if (v->get_value() > add_node->get_value()) {
        if (v->left == nullptr) {
            return new node(v->get_value(), add_node, v->right);
        }
        return new node(v->get_value(), insert_value(v->left, add_node), v->right);
    }
    //    std::cout << "WARNING " << v->get_value() << std::endl;
    if (v->right == nullptr) {
        return new node(v->get_value(), v->left, add_node);
    }
    return new node(v->get_value(), v->left, insert_value(v->right, add_node));
};

// Âîçâàùàåò èòåðàòîð íà ýëåìåíò ñ ìèíèìàëüíûé êëþ÷îì.
persistent_set::iterator persistent_set::begin() const {
    assert(root != nullptr);
    node* use = root;
    while (use->left != nullptr) {
        use = use->left;
    }
    return iterator(use, root);
}

// Âîçâàùàåò èòåðàòîð íà ýëåìåíò ñëåäóþùèé çà ýëåìåíòîì ñ ìàêñèìàëüíûì êëþ÷îì.
persistent_set::iterator persistent_set::end() const {
    assert(root != nullptr);
    return iterator(root, root);
}

// Óäàëåíèå ýëåìåíòà.
// Èíâàëèäèðóåò âñå èòåðàòîðû, ïðèíàäëåæàùèå persistent_set'ó this, âêëþ÷àÿ end().
void persistent_set::erase(iterator elem) {
    assert(elem.value != nullptr);
    root->valid = false;
    root = erase_value(elem.version_root, elem.value);
}

persistent_set::node* persistent_set::erase_value(node* v, node* erase_node) {
    if (v->get_value() > erase_node->get_value()) {
        return new node(v->get_value(), erase_value(v->left, erase_node), v->right);
    }
    if (v->get_value() < erase_node->get_value()) {
        return new node(v->get_value(), v->left, erase_value(v->right, erase_node));
    }
    if (v->right == nullptr) {
        return (v->left == nullptr) ? nullptr : new node(v->left->get_value(), v->left->left, v->left->right);
    }
    return new node(v->right->get_min()->get_value(), v->left, simple_deleted(v->right));
}

persistent_set::node* persistent_set::simple_deleted(node* v) {
    if (v->left == nullptr)
        return (v->right == nullptr) ? nullptr : new node(v->right->get_value(), v->right->left, v->right->right);
    return new node(v->get_value(), simple_deleted(v->left), v->right);
}


persistent_set::node::node() {
    value = INT_MAX;
    count = 1;
    left = right = nullptr;
    valid = true;
}

persistent_set::node::node(value_type v, node* l, node* r) {
    value = v;
    count = 1;
    left = l;
    right = r;
    valid = true;
}

//óìåíüøàåò ÷èñëî ññûëîê íà íåå
//åñëè ññûëîê 0, âåðøèíà óäàëÿåòñÿ
void persistent_set::node::dec_node() {
    count--;
    if (count == 0) {
        delete this;
    }
}

value_type& persistent_set::node::get_value() {
    return value;
}

//êîíñòðóêòîð ïî óìîë÷àíèþ è êîíñòðóêòîð îò ññûëêè íà îïðåäåëåííóþ node
persistent_set::iterator::iterator() {
    value = version_root = nullptr;
}

persistent_set::iterator::iterator(node* x, node* v) {
    value = x;
    version_root = v;
}

// Ýëåìåíò íà êîòîðûé ñåé÷àñ ññûëàåòñÿ èòåðàòîð.
// Ðàçûìåíîâàíèå èòåðàòîðà end() íåîïðåäåëåíî.
// Ðàçûìåíîâàíèå íåâàëèäíîãî èòåðàòîðà íåîïðåäåëåíî.
value_type const& persistent_set::iterator::operator*() const {
    assert(version_root && version_root->valid);
    return value->get_value();
}

// Ñðàâíåíèå. Èòåðàòîðû ñ÷èòàþòñÿ ýêâèâàëåíòíûìè åñëè îäíè ññûëàþòñÿ íà îäèí è òîò æå ýëåìåíò.
// Ñðàâíåíèå ñ íåâàëèäíûì èòåðàòîðîì íå îïðåäåëåíî.
// Ñðàâíåíèå èòåðàòîðîâ äâóõ ðàçíûõ êîíòåéíåðîâ íå îïðåäåëåíî.
bool operator==(persistent_set::iterator u, persistent_set::iterator v) {
    return u.value == v.value && u.version_root == v.version_root;
}
bool operator!=(persistent_set::iterator u, persistent_set::iterator v) {
    return u.value != v.value || u.version_root != v.version_root;
}

persistent_set::node* persistent_set::node::get_min() {
    assert(this != nullptr);
    node* use = this;
    while (use->left != nullptr) {
        use = use->left;
    }
    return use;
}

persistent_set::node* persistent_set::node::get_max() {
    assert(this != nullptr);
    node* use = this;
    while (use->right != nullptr) {
        use = use->right;
    }
    return use;
}


// Ïåðåõîä ê ýëåìåíòó ñî ñëåäóþùèì ïî âåëè÷èíå êëþ÷îì.
// Èíêðåìåíò èòåðàòîðà end() íåîïðåäåëåí.
// Èíêðåìåíò íåâàëèäíîãî èòåðàòîðà íåîïðåäåëåí.
persistent_set::iterator& persistent_set::iterator::operator++() {
    if (value->right != nullptr) {
        value = value->right->get_min();
        return *this;
    }
    node* r = version_root;
    std::vector<node*> parent;
    while (r->right != value && r->left != value) {
        parent.push_back(r);
        if (r->get_value() < value->get_value()) {
            r = r->right;
        }
        else {
            r = r->left;
        }
    }
    parent.push_back(r);
    while (parent.back()->right == value) {
        value = parent.back();
        parent.pop_back();
    }
    value = parent.back();
    return *this;
}
persistent_set::iterator persistent_set::iterator::operator++(int) {
    assert(value != nullptr);
    iterator ans(value, version_root);
    iterator me = *this;
    *this = ++me;
    return ans;
}

// Ïåðåõîä ê ýëåìåíòó ñî ñëåäóþùèì ïî âåëè÷èíå êëþ÷îì.
// Äåêðåìåíò èòåðàòîðà begin() íåîïðåäåëåí.
// Äåêðåìåíò íåâàëèäíîãî èòåðàòîðà íåîïðåäåëåí.
persistent_set::iterator& persistent_set::iterator::operator--() {
    if (value->left != nullptr) {
        value = value->left->get_max();
        return *this;
    }
    node* r = version_root;
    std::vector<node*> parent;
    while (r->right != value && r->left != value) {
        parent.push_back(r);
        if (r->get_value() < value->get_value()) {
            r = r->right;
        }
        else {
            r = r->left;
        }
    }
    parent.push_back(r);
    while (parent.back()->left == value) {
        value = parent.back();
        parent.pop_back();
    }
    value = parent.back();
    return *this;
}
persistent_set::iterator persistent_set::iterator::operator--(int) {
    assert(value != nullptr);
    iterator ans(value, version_root);
    iterator me = *this;
    *this = --me;
    return ans;
}

void persistent_set::print() {
    node::print_node(root);
    std::cout << std::endl;
}

void persistent_set::node::print_node(node* v) {
    if (v == nullptr)
        return;
    std::cout << '(';
    print_node(v->left);
    std::cout << v->get_value();
    print_node(v->right);
    std::cout << ')';
}

// Testing part
int main() {
    int a[6] = { 2, 5, 3, 7, 1, 9 };
    persistent_set me;
    for (int i : a) {
        me.insert(i);
    }
    me.print();
    persistent_set::iterator ex = me.end();
    std::cout << *(ex) << std::endl;
    while (ex != me.begin()) {
        std::cout << *(ex--) << std::endl;
    }
    for (int i : a) {
        me.erase(me.find(i));
        me.print();
    }
    /*    for (int i : a) {
    me.erase(me.find(i));
    ex = me.begin();
    std::cout << *ex << std::endl;
    }
    */  return 0;
}