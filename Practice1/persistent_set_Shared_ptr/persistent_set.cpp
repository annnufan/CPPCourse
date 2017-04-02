//
// Created by Anna Kopeliovich(annnufan@gmail.com, nufan@list.ru) on 04.10.2016.
//

#include "persistent_set.h"

typedef int value_type;

#include <cassert>
#include <iostream>

// Создает пустой persistent_set.
persistent_set::persistent_set() {
	root = std::shared_ptr<node>(new node());
}

// Создает копию указанного persistent_set-а.
persistent_set::persistent_set(persistent_set const &rhs) {
	assert(rhs.root != std::shared_ptr<node>());
	root = std::shared_ptr<node>(new node());
	root->left = rhs.root->left;
}

// Изменяет this так, чтобы он содержал те же элементы, что и rhs.
// Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
persistent_set& persistent_set::operator=(persistent_set const& rhs) {
	root->valid = false;
	old_root.push_back(root);
	root = std::shared_ptr<node>(new node());
	root->left = rhs.root->left;
	return *this;
}

// Деструктор. Вызывается при удалении объектов persistent_set.
// Инвалидирует все итераторы ссылающиеся на элементы этого persistent_set
// (включая итераторы ссылающиеся на элементы следующие за последними).
persistent_set::~persistent_set() {
	assert(root);
	old_root.clear();
	root = std::shared_ptr<node>();
}


// Поиск элемента.
// Возвращает итератор на найденный элемент, либо end().
persistent_set::iterator persistent_set::find(value_type val) {
	assert(root);
	std::shared_ptr<node> now_node = root;
	while (now_node && now_node->get_value() != val) {
		if (now_node->get_value() < val) {
			now_node = now_node->right;
		}
		else {
			now_node = now_node->left;
		}
	}
	iterator ans;
	if (!now_node)
		ans = iterator(root, root);
	else
		ans = iterator(now_node, root);
	return ans;
}

// Вставка элемента.
// 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
//    на уже присутствующий элемент и false.
// 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
//    элемент и true.
// Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
std::pair<persistent_set::iterator, bool> persistent_set::insert(value_type val) {
	assert(root);
	iterator f = find(val);
	if (f.value != root) {
		return std::make_pair(f, false);
	}
	std::shared_ptr<node> add_node = std::shared_ptr<node>(new node(val));
	root->valid = false;
	old_root.push_back(root);
	root = insert_value(root, add_node);
	iterator ans(add_node, root);
	return std::make_pair(ans, true);
};

//Функция, добавляющая новый элемент в наш сет
std::shared_ptr<persistent_set::node> persistent_set::insert_value(std::shared_ptr<persistent_set::node> v, std::shared_ptr<persistent_set::node> add_node) {
	if (v->get_value() > add_node->get_value()) {
		if (!v->left) {
			return std::shared_ptr<node>(new node(v->get_value(), add_node, v->right));
		}
		return std::shared_ptr<node>(new node(v->get_value(), insert_value(v->left, add_node), v->right));
	}
	if (!v->right) {
		return std::shared_ptr<node>(new node(v->get_value(), v->left, add_node));
	}
	return std::shared_ptr<node>(new node(v->get_value(), v->left, insert_value(v->right, add_node)));
};

// Возващает итератор на элемент с минимальный ключом.
persistent_set::iterator persistent_set::begin() const {
	assert(root);
	std::shared_ptr<persistent_set::node> use = root;
	while (use->left) {
		use = use->left;
	}
	return iterator(use, root);
}

// Возващает итератор на элемент следующий за элементом с максимальным ключом.
persistent_set::iterator persistent_set::end() const {
	assert(root);
	return iterator(root, root);
}

// Удаление элемента.
// Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
void persistent_set::erase(iterator elem) {
	assert(elem.value);
	root->valid = false;
	old_root.push_back(root);
	root = erase_value(elem.version_root, elem.value);
}

std::shared_ptr<persistent_set::node> persistent_set::erase_value(std::shared_ptr<persistent_set::node> v, std::shared_ptr<persistent_set::node> erase_node) {
	if (v->get_value() > erase_node->get_value()) {
		return std::shared_ptr<node>(new node(v->get_value(), erase_value(v->left, erase_node), v->right));
	}
	if (v->get_value() < erase_node->get_value()) {
		return std::shared_ptr<node>(new node(v->get_value(), v->left, erase_value(v->right, erase_node)));
	}
	if (!v->right) {
		return (!v->left) ? std::shared_ptr<node>() : std::shared_ptr<node>(new node(v->left->get_value(), v->left->left, v->left->right));
	}
	return std::shared_ptr<node>(new node(v->right->get_min(v->right)->get_value(), v->left, simple_deleted(v->right)));
}

std::shared_ptr<persistent_set::node> persistent_set::simple_deleted(std::shared_ptr<persistent_set::node> v) {
	if (!v->left)
		return (!v->right) ? std::shared_ptr<node>() : std::shared_ptr<node>(new node(v->right->get_value(), v->right->left, v->right->right));
	return std::shared_ptr<node>(new node(v->get_value(), simple_deleted(v->left), v->right));
}


persistent_set::node::node() {
	value = INT_MAX;
	left = right = std::shared_ptr<node>();
	valid = true;
}

persistent_set::node::node(value_type v, std::shared_ptr<persistent_set::node> l, std::shared_ptr<persistent_set::node> r) {
	value = v;
	left = l;
	right = r;
	valid = true;
}

value_type& persistent_set::node::get_value() {
	return value;
}

//конструктор по умолчанию и конструктор от ссылки на определенную node
persistent_set::iterator::iterator() {
	value = version_root = std::shared_ptr<node>();
}

persistent_set::iterator::iterator(std::shared_ptr<persistent_set::node> x, std::shared_ptr<persistent_set::node> v) {
	value = x;
	version_root = v;
}

// Элемент на который сейчас ссылается итератор.
// Разыменование итератора end() неопределено.
// Разыменование невалидного итератора неопределено.
value_type const& persistent_set::iterator::operator*() const {
	assert(version_root->valid);
	return value->get_value();
}

// Сравнение. Итераторы считаются эквивалентными если одни ссылаются на один и тот же элемент.
// Сравнение с невалидным итератором не определено.
// Сравнение итераторов двух разных контейнеров не определено.
bool operator==(persistent_set::iterator u, persistent_set::iterator v) {
	return u.value == v.value && u.version_root == v.version_root;
}
bool operator!=(persistent_set::iterator u, persistent_set::iterator v) {
	return u.value != v.value || u.version_root != v.version_root;
}

std::shared_ptr<persistent_set::node> persistent_set::node::get_min(std::shared_ptr<node> our) {
	assert(this != nullptr);
	std::shared_ptr<node> use = our;
	while (use->left) {
		use = use->left;
	}
	return use;
}

std::shared_ptr<persistent_set::node> persistent_set::node::get_max(std::shared_ptr<node> our) {
	assert(this != nullptr);
	std::shared_ptr<node> use = our;
	while (use->right) {
		use = use->right;
	}
	return use;
}


// Переход к элементу со следующим по величине ключом.
// Инкремент итератора end() неопределен.
// Инкремент невалидного итератора неопределен.
persistent_set::iterator& persistent_set::iterator::operator++() {
	if (value->right) {
		value = value->right->get_min(value->right);
		return *this;
	}
	std::shared_ptr<node> r = version_root;
	std::vector<std::shared_ptr<node>> parent;
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
	assert(value);
	iterator ans(value, version_root);
	iterator me = *this;
	*this = ++me;
	return ans;
}

// Переход к элементу со следующим по величине ключом.
// Декремент итератора begin() неопределен.
// Декремент невалидного итератора неопределен.
persistent_set::iterator& persistent_set::iterator::operator--() {
	if (value->left) {
		value = value->left->get_max(value->left);
		return *this;
	}
	std::shared_ptr<node> r = version_root;
	std::vector<std::shared_ptr<node>> parent;
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
	node* v = value.get();
	while (parent.back()->right == value) {
		value = parent.back();
		parent.pop_back();
	}
	value = parent.back();
	return *this;
}
persistent_set::iterator persistent_set::iterator::operator--(int) {
	assert(value);
	iterator ans(value, version_root);
	iterator me = *this;
	*this = --me;
	return ans;
}

void persistent_set::print() {
	node::print_node(root);
	std::cout << std::endl;
}

void persistent_set::node::print_node(std::shared_ptr<persistent_set::node> v) {
	if (!v)
		return;
	std::cout << '(';
	print_node(v->left);
	std::cout << v->get_value();
	print_node(v->right);
	std::cout << ')';
}

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
	/*for (int i : a) {
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