//
// Created by Anna Kopeliovich(annnufan@gmail.com, nufan@list.ru) on 04.10.2016.
//

#ifndef PERSISTENT_SET_PERSISTENT_SET_H
#define PERSISTENT_SET_PERSISTENT_SET_H

#include <utility>
#include <vector>

typedef int value_type;

struct persistent_set
{

    // Bidirectional iterator.
    struct iterator;

    // Создает пустой persistent_set.
    persistent_set();

    // Создает копию указанного persistent_set-а.
    persistent_set(persistent_set const&);

    // Изменяет this так, чтобы он содержал те же элементы, что и rhs.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    persistent_set& operator=(persistent_set const& rhs);

    // Деструктор. Вызывается при удалении объектов persistent_set.
    // Инвалидирует все итераторы ссылающиеся на элементы этого persistent_set
    // (включая итераторы ссылающиеся на элементы следующие за последними).
    ~persistent_set();

    // Поиск элемента.
    // Возвращает итератор на элемент найденный элемент, либо end().
    // Если элемент найден, он помечается как наиболее поздно использованный.
    iterator find(value_type);

    // Вставка элемента.
    // 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
    //    на уже присутствующий элемент и false.
    // 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
    //    элемент и true.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    std::pair<iterator, bool> insert(value_type);

    // Удаление элемента.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    void erase(iterator);

    // Возващает итератор на элемент с минимальный ключом.
    iterator begin() const;
    // Возващает итератор на элемент следующий за элементом с максимальным ключом.
    iterator end() const;

private:
    //структура, в которой будут храниться значения, и ссылки на которую будет хранить наше дерево
    struct node;
    //ссылка на наш корень, за который всё подвешено
    node* root;
    //список всех итераторов данного set
    std::vector<iterator*> list_of_iterator;
    node* insert_value(node*, node*);
    node* erase_value(node*, node*);
    //функция, инфалидирующая все итераторы this
    void invalidate_all_iterators();
};

struct persistent_set::node {
    node();
    node(value_type, node* = nullptr, node* = nullptr);
    //уменьшает число ссылок на нее
    //если ссылок 0, вершина удаляется
    void dec_node();
    //увеличивает число ссылок на нее
    void add();
    //посмотреть значение в this
    value_type get_value();
    //левый и правый соседи вершинки
    node* left;
    node* right;
    node* get_min();
    node* get_max();
private:
    //значение в this вершине
    value_type value;
    int count = 0;
};

struct persistent_set::iterator {
    //конструктор по умолчанию и конструктор от ссылки на определенную node
    iterator();
    iterator(node*, node*);

    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end() неопределено.
    // Разыменование невалидного итератора неопределено.
    value_type const& operator*() const;

    // Переход к элементу со следующим по величине ключом.
    // Инкремент итератора end() неопределен.
    // Инкремент невалидного итератора неопределен.
    iterator& operator++();
    iterator operator++(int);

    // Переход к элементу со следующим по величине ключом.
    // Декремент итератора begin() неопределен.
    // Декремент невалидного итератора неопределен.
    iterator& operator--();
    iterator operator--(int);
    node* value;
    node* version_root;
};

// Сравнение. Итераторы считаются эквивалентными если одни ссылаются на один и тот же элемент.
// Сравнение с невалидным итератором не определено.
// Сравнение итераторов двух разных контейнеров не определено.
bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);


#endif //PERSISTENT_SET_PERSISTENT_SET_H
