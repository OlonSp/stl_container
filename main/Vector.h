#pragma once

#include <iostream>
#include "Exceptions.h"
#include "VectorIterator.h"

template<typename DataType>
class Vector {
private:
	DataType* vectorArray;
	DataType* memoryEnd;
	DataType* vectorEnd;
public:
	friend VectorIterator<DataType>;
	using iterator = VectorIterator<DataType>;

	//������������
	//�� ���������
	Vector()
		: vectorArray(nullptr)
		, memoryEnd(nullptr)
		, vectorEnd(nullptr)
	{}

	// ����������� ���������� ���������
	// ����������� ���������� ��������� � ��������, 
	//���������������� �������� �������.
	Vector(size_t size, const DataType& initValue = DataType())
		: vectorArray(new DataType[size * 2])
		, memoryEnd(vectorArray + size * 2)
		, vectorEnd(vectorArray + size)
	{
		for (size_t i = 0; i < size; ++i) {
			vectorArray[i] = initValue;
		}
	}

	
	// ����������� �������� [beg,end), �������� �����������.
	template <class iter>
	Vector(iter beg, iter end)
		: vectorArray(new DataType[(end - beg) * 2])
		, vectorEnd(vectorArray + (end - beg))
		, memoryEnd(vectorArray + (end - beg) * 2)
	{
		size_t i = 0;
		for (iterator tmp = beg; tmp < end; ++tmp) {
			vectorArray[i] = *tmp;
			++i;
		}

	}


	//����������� ������ ������������� std::initializer_list.
	Vector(const std::initializer_list<DataType>& list) 
		: vectorArray(new DataType[list.size() * 2])
		, memoryEnd(vectorArray + list.size() * 2)
		, vectorEnd(vectorArray + list.size())
	{
		size_t i = 0;
		for (const auto& elem : list) {
			vectorArray[i] = elem;
			++i;
		}
	}


	//���������� �����������
	Vector(const Vector& other)
		: vectorArray(new DataType[other.capacity()])
		, vectorEnd(vectorArray + other.size())
		, memoryEnd(vectorArray + other.capacity())
	{
		for (size_t i = 0; i < other.size(); ++i) {
			vectorArray[i] = other.vectorArray[i];
		}
	}


	//����������� ��������.
	Vector(Vector&& other) 
		: vectorArray(other.vectorArray)
		, vectorEnd(other.vectorEnd)
		, memoryEnd(other.memoryEnd)
	{
		other.memoryEnd = nullptr;
		other.vectorEnd = nullptr;
		other.vectorArray = nullptr;
	}


	//����������
	~Vector() {
		delete[] vectorArray;
	}

	//�������-��������
	//������������ � ������������
	Vector& operator=(const Vector& other) {
		if (this == &other)
			return *this;

		if (size() != other.size()) {
			delete[] vectorArray;
			vectorArray = new DataType[other.capacity()];
		}
		vectorEnd = vectorArray + other.size();
		memoryEnd = vectorArray + other.capacity();
		for (size_t i = 0; i < other.size(); ++i) {
			vectorArray[i] = other.vectorArray[i];
		}

		return *this;
	}

	//������������ � ���������
	Vector& operator=(Vector&& other) {
		if (this == &other)
			return *this;
		
		delete[] vectorArray;
		vectorArray = other.vectorArray;
		vectorEnd = other.vectorEnd;
		memoryEnd = other.memoryEnd;
		other.memoryEnd = nullptr;
		other.vectorEnd = nullptr;
		other.vectorArray = nullptr;
		
		return *this;
	}

	//�������� �������������� (��� ������ � ������), ������������ ������ 
	//�� ��������������� �������(��� �������� ������ �� ������� �������).
	DataType& operator[](size_t num) {
		return vectorArray[num];
	}

	const DataType& operator[](size_t num) const {
		return vectorArray[num];
	}

	//������
	//at - ������� ������� � �������� 
	DataType& at(size_t ind) {
		if (ind >= size()) {
			throw OutOfRangeException("Wrong index in method Vector::at");
		}
		return vectorArray[ind];
	}

	const DataType& at(size_t ind) const {
		if (ind >= size()) {
			throw OutOfRangeException("Wrong index in method Vector::at");
		}
		return vectorArray[ind];
	}

	//begin() � ���������� �������� �� ������ �������.
	iterator begin() {
		return iterator(this, 0);
	}

	//end() � ���������� �������� �� �������, ��������� �� ���������.
	iterator end() {
		return iterator(this, size());
	}

	//��������� ����� �������� elem ����� 
	//�������� ��������� pos � ���������� ������� ������ ��������.
	//������������ ������� ������.
	iterator insert(iterator pos, const DataType& elem) {
		if (this != pos.vec) {
			throw IteratorException("method Vector::insert(pos, elem)");
		}
		if (size() == capacity()) {
			DataType* arr = new DataType[capacity() * 2];
			size_t siz = size();
			for (size_t i = 0; i < siz; ++i) {
				arr[i] = vectorArray[i];
			}
			delete[] vectorArray;
			vectorArray = arr;
			vectorEnd = vectorArray + siz;
			memoryEnd = vectorArray + siz * 2;
		}
		vectorEnd = vectorArray + size() + 1;
		int curPos = pos.cur;
		for (size_t i = size() - 1; i > curPos; --i) {
			vectorArray[i] = vectorArray[i - 1];
		}
		vectorArray[curPos] = elem;
		return pos;
	}

	//������������ ������ � ���������.
	iterator insert(iterator pos, DataType&& elem) {
		if (this != pos.vec) {
			throw IteratorException("method Vector::insert(pos, elem)");
		}
		if (size() == capacity()) {
			DataType* arr = new DataType[capacity() * 2];
			size_t siz = size();
			for (size_t i = 0; i < siz; ++i) {
				arr[i] = vectorArray[i];
			}
			delete[] vectorArray;
			vectorArray = arr;
			vectorEnd = vectorArray + siz;
			memoryEnd = vectorArray + siz * 2;
		}
		vectorEnd = vectorArray + size() + 1;
		int curPos = pos.cur;
		for (size_t i = size() - 1; i > curPos; --i) {
			vectorArray[i] = vectorArray[i - 1];
		}
		vectorArray[curPos] = std::move(elem);
		return pos;
	}

	/*
	insert(pos,�beg,�end) � ��������� ����� ���� ��������� ��������� [beg,�end) 
	����� �������� ��������� pos 
	� ���������� ������� ������� ������ �������� 
	(��� �������� pos, ���� ����� ��������� ���).
	*/
	template<class iter>
	iterator insert(iterator pos, iter beg, iter end) {
		if (this != pos.vec || end.vec != beg.vec || beg > end) {
			throw IteratorException("method Vector::insert(pos, beg, end)");
		}
		for (auto val = end - 1; val > beg; --val) {
			insert(pos, *val);
		}
		insert(pos, *beg);
		iterator newPos(pos);
		for (auto val = beg; val != end; ++val) {
			--newPos;
		}
		return newPos;
	}

	/*
	erase(pos) � ������� ������� � ������� ��������� pos 
	� ���������� ������� ���������� ��������.
	*/
	iterator erase(iterator pos) {
		for (auto val = pos; val < end()-1; ++val) {
			*val = *(val + 1);
		}
		*(end() - 1) = DataType();
		return pos;
	}

	//pushBack - �������� � ����� �������(������ � ������)
	void pushBack(const DataType& elem) {
		if (vectorArray == nullptr) {
			vectorArray = new DataType[2];
			vectorEnd = vectorArray + 1;
			memoryEnd = vectorArray + 2;
		}
		else if (size() == capacity()) {
			size_t siz = capacity();
			DataType* arr = new DataType[siz * 2];
			for (size_t i = 0; i < size(); ++i) {
				arr[i] = vectorArray[i];
			}
			delete[] vectorArray;
			vectorArray = arr;
			vectorEnd = vectorArray + siz;
			memoryEnd = vectorArray + siz * 2;
		}
		*vectorEnd = elem;
		vectorEnd += 1;
	}

	//pushBack - �������� � ����� �������(������ � ���������)
	void pushBack(DataType&& elem) {
		if (vectorArray == nullptr) {
			vectorArray = new DataType[2];
			vectorEnd = vectorArray + 1;
			memoryEnd = vectorArray + 2;
		}
		else if (size() == capacity()) {
			size_t mem = capacity();
			DataType* arr = new DataType[mem * 2];
			for (size_t i = 0; i < size(); ++i) {
				arr[i] = vectorArray[i];
			}
			delete[] vectorArray;
			vectorArray = arr;
			vectorEnd = vectorArray + mem;
			memoryEnd = vectorArray + mem * 2;
		}
		*vectorEnd = std::move(elem);
		vectorEnd += 1;
	}


	//popBack - �������� ���������� ��������
	void popBack()
	{
		if (size() != 0) {
			vectorArray[size() - 1] = DataType();
			vectorEnd = vectorArray + size() - 1;
		}
	}

	//reserve(num) - ����������� ���������� ���������� ������
	void reserve(size_t num) {
		if (num < 0) {
			throw string("Attempt to down capcity at method Vector::reserve");
		}
		size_t siz = size();
		size_t mem = capacity();
		DataType* arr = new DataType[mem + num];
		for (size_t i = 0; i < size(); ++i) {
			arr[i] = vectorArray[i];
		}
		delete[] vectorArray;
		vectorArray = arr;
		vectorEnd = vectorArray + siz;
		memoryEnd = vectorArray + mem + num;
	}

	//resize(num) - �������� ���������� ��������� �� num (���� 
	//������ size() �������������, ����� �������� ���������
	//�������������� �� ��������� ��� ���������).
	void resize(size_t num) {
		if (num < 0) {
			throw std::string("Wrong size for Vector::resize");
		}
		else if (num < size()) {
			for (size_t i = num; i < size(); ++i) {
				vectorArray[i] = DataType();
			}
			vectorEnd = vectorArray + num;
		}
		else if (num > size()) {
			while (num > capacity())
				if (capacity() != 0)
					this->reserve(capacity());
				else
					this->reserve(num);
			for (size_t i = size(); i < num; ++i) {
				vectorArray[i] = DataType();
			}
			vectorEnd = vectorArray + num;
		}
	}

	//������ �� ������
	bool empty() const{
		return vectorArray == vectorEnd;
	}

	//������ �������
	size_t size() const {
		return vectorEnd - vectorArray;
	}

	//������� ������ ��������� ��� ������
	size_t capacity() const {
		return memoryEnd - vectorArray;
	}
	
	//�������� ������
	void clear() {
		for (size_t i = 0; i < size(); ++i) {
			vectorArray[i] = DataType();
		}
		vectorEnd = vectorArray;
	}

	//operator<< ��� ������ ����� iostream
	friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
		os << "(";
		for (size_t i = 0; i < vec.size()-1; i++) {
			os << vec[i] << ", ";
		}
		os << vec[vec.size() - 1] << ")";
		return os;
	}
};