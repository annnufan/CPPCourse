//
// Created by Anna Kopeliovich(annnufan@gmail.com, nufan@list.ru) on 04.10.2016.
//

#pragma once

#ifndef PERSISTENT_SET_PERSISTENT_SET_H
#define PERSISTENT_SET_PERSISTENT_SET_H

#include <utility>
#include <vector>

typedef int value_type;

struct persistent_set
{

	// Bidirectional iterator.
	struct iterator;

	// ������� ������ persistent_set.
	persistent_set();

	// ������� ����� ���������� persistent_set-�.
	persistent_set(persistent_set const&);

	// �������� this ���, ����� �� �������� �� �� ��������, ��� � rhs.
	// ������������ ��� ���������, ������������� persistent_set'� this, ������� end().
	persistent_set& operator=(persistent_set const& rhs);

	// ����������. ���������� ��� �������� �������� persistent_set.
	// ������������ ��� ��������� ����������� �� �������� ����� persistent_set
	// (������� ��������� ����������� �� �������� ��������� �� ����������).
	~persistent_set();

	// ����� ��������.
	// ���������� �������� �� ������� ��������� �������, ���� end().
	// ���� ������� ������, �� ���������� ��� �������� ������ ��������������.
	iterator find(value_type);

	// ������� ��������.
	// 1. ���� ����� ���� ��� ������������, ������� �� �������������, ������������ ��������
	//    �� ��� �������������� ������� � false.
	// 2. ���� ������ ����� ��� ���, ������������� �������, ������������ �������� �� ���������
	//    ������� � true.
	// ������������ ��� ���������, ������������� persistent_set'� this, ������� end().
	std::pair<iterator, bool> insert(value_type);

	// �������� ��������.
	// ������������ ��� ���������, ������������� persistent_set'� this, ������� end().
	void erase(iterator);

	// ��������� �������� �� ������� � ����������� ������.
	iterator begin() const;
	// ��������� �������� �� ������� ��������� �� ��������� � ������������ ������.
	iterator end() const;
	void print();

private:
	//���������, � ������� ����� ��������� ��������, � ������ �� ������� ����� ������� ���� ������
	struct node;
	//������ �� ��� ������, �� ������� �� ���������
	node* root;
	node* insert_value(node*, node*);
	node* erase_value(node*, node*);
	node* simple_deleted(node*);
};

struct persistent_set::node {
	node();
	node(value_type, node* = nullptr, node* = nullptr);
	//��������� ����� ������ �� ���
	//���� ������ 0, ������� ���������
	void dec_node();
	//����������� ����� ������ �� ���
	void add();
	//���������� �������� � this
	value_type& get_value();
	//����� � ������ ������ ��������
	node* left;
	node* right;

	//����������� � ������������ �������� � ����� ���������
	node* get_min();
	node* get_max();
	static void print_node(persistent_set::node*);
	
private:
	//�������� � this �������
	value_type value;

	//������� ������ �� �������
	int count = 0;
};

struct persistent_set::iterator {
	//����������� �� ��������� � ����������� �� ������ �� ������������ node
	iterator();
	iterator(node*, node*);

	// ������� �� ������� ������ ��������� ��������.
	// ������������� ��������� end() ������������.
	// ������������� ����������� ��������� ������������.
	value_type const& operator*() const;

	// ������� � �������� �� ��������� �� �������� ������.
	// ��������� ��������� end() �����������.
	// ��������� ����������� ��������� �����������.
	iterator& operator++();
	iterator operator++(int);

	// ������� � �������� �� ��������� �� �������� ������.
	// ��������� ��������� begin() �����������.
	// ��������� ����������� ��������� �����������.
	iterator& operator--();
	iterator operator--(int);


	node* value;
	node* version_root;
};

// ���������. ��������� ��������� �������������� ���� ���� ��������� �� ���� � ��� �� �������.
// ��������� � ���������� ���������� �� ����������.
// ��������� ���������� ���� ������ ����������� �� ����������.
bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);


#endif //PERSISTENT_SET_PERSISTENT_SET_H
