//
// Created by Anna Kopeliovich(annnufan@gmail.com, nufan@list.ru) on 04.10.2016.
//

#ifndef PERSISTENT_SET_PERSISTENT_SET_H
#define PERSISTENT_SET_PERSISTENT_SET_H

#include <utility>
#include <vector>
#include <memory>

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
	std::shared_ptr<node> root;
	//������ ���� ���������� ������� set

	std::shared_ptr<node> insert_value(std::shared_ptr<node>, std::shared_ptr<node>);
	std::shared_ptr<node> erase_value(std::shared_ptr<node>, std::shared_ptr<node>);
	std::shared_ptr<node> simple_deleted(std::shared_ptr<node>);
	std::vector<std::shared_ptr<node>> old_root;
};

struct persistent_set::node : std::enable_shared_from_this<node> {
	node();
	node(value_type, std::shared_ptr<node> = std::shared_ptr<node>(), std::shared_ptr<node> = std::shared_ptr<node>());

	//���������� �������� � this
	value_type& get_value();
	//����� � ������ ������ ��������
	std::shared_ptr<node> left;
	std::shared_ptr<node> right;

	//����������� � ������������ �������� � ����� ���������
	std::shared_ptr<node> get_min(std::shared_ptr<node>);
	std::shared_ptr<node> get_max(std::shared_ptr<node>);
	static void print_node(std::shared_ptr<node>);
	bool valid;
private:
	//�������� � this �������
	value_type value;
};

struct persistent_set::iterator {
	//����������� �� ��������� � ����������� �� ������ �� ������������ node
	iterator();
	iterator(std::shared_ptr<node>, std::shared_ptr<node>);

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


	std::shared_ptr<node> value;
	std::shared_ptr<node> version_root;
};

// ���������. ��������� ��������� �������������� ���� ���� ��������� �� ���� � ��� �� �������.
// ��������� � ���������� ���������� �� ����������.
// ��������� ���������� ���� ������ ����������� �� ����������.
bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);


#endif //PERSISTENT_SET_PERSISTENT_SET_H
