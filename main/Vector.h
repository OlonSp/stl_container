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

	//Конструкторы
	//по умолчанию
	Vector()
		: vectorArray(nullptr)
		, memoryEnd(nullptr)
		, vectorEnd(nullptr)
	{}

	// принимающий количество элементов
	// принимающий количество элементов и значение, 
	//инициализирующее элементы вектора.
	Vector(size_t size, const DataType& initValue = DataType())
		: vectorArray(new DataType[size * 2])
		, memoryEnd(vectorArray + size * 2)
		, vectorEnd(vectorArray + size)
	{
		for (size_t i = 0; i < size; ++i) {
			vectorArray[i] = initValue;
		}
	}

	
	// принимающий интервал [beg,end), заданный итераторами.
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


	//принимающий список инициализации std::initializer_list.
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


	//Копирующий конструктор
	Vector(const Vector& other)
		: vectorArray(new DataType[other.capacity()])
		, vectorEnd(vectorArray + other.size())
		, memoryEnd(vectorArray + other.capacity())
	{
		for (size_t i = 0; i < other.size(); ++i) {
			vectorArray[i] = other.vectorArray[i];
		}
	}


	//Конструктор переноса.
	Vector(Vector&& other) 
		: vectorArray(other.vectorArray)
		, vectorEnd(other.vectorEnd)
		, memoryEnd(other.memoryEnd)
	{
		other.memoryEnd = nullptr;
		other.vectorEnd = nullptr;
		other.vectorArray = nullptr;
	}


	//Деструктор
	~Vector() {
		delete[] vectorArray;
	}

	//функции-операции
	//присваивание с копированием
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

	//присваивание с переносом
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

	//Операции индексирования (для чтения и записи), возвращающие ссылку 
	//на соответствующий элемент(без проверки выхода за пределы массива).
	DataType& operator[](size_t num) {
		return vectorArray[num];
	}

	const DataType& operator[](size_t num) const {
		return vectorArray[num];
	}

	//методы
	//at - вернуть элемент с индексом 
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

	//begin() – возвращает итератор на начало вектора.
	iterator begin() {
		return iterator(this, 0);
	}

	//end() – возвращает итератор на элемент, следующий за последним.
	iterator end() {
		return iterator(this, size());
	}

	//вставляет копию элемента elem перед 
	//позицией итератора pos и возвращает позицию нового элемента.
	//Предоставить обычную версию.
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

	//Предоставить версию с переносом.
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
	insert(pos, beg, end) – вставляет копии всех элементов интервала [beg, end) 
	перед позицией итератора pos 
	и возвращает позицию первого нового элемента 
	(или итератор pos, если новых элементов нет).
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
	erase(pos) – удаляет элемент в позиции итератора pos 
	и возвращает позицию следующего элемента.
	*/
	iterator erase(iterator pos) {
		for (auto val = pos; val < end()-1; ++val) {
			*val = *(val + 1);
		}
		*(end() - 1) = DataType();
		return pos;
	}

	//pushBack - вставить в конец вектора(версия с копией)
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

	//pushBack - вставить в конец вектора(версия с переносом)
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


	//popBack - удаление последнего элемента
	void popBack()
	{
		if (size() != 0) {
			vectorArray[size() - 1] = DataType();
			vectorEnd = vectorArray + size() - 1;
		}
	}

	//reserve(num) - увеличивает количество выделенной памяти
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

	//resize(num) - изменяет количество элементов до num (если 
	//размер size() увеличивается, новые элементы создаются
	//конструкторами по умолчанию для элементов).
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

	//пустой ли вектор
	bool empty() const{
		return vectorArray == vectorEnd;
	}

	//размер вектора
	size_t size() const {
		return vectorEnd - vectorArray;
	}

	//сколько памяти выделенно под вектор
	size_t capacity() const {
		return memoryEnd - vectorArray;
	}
	
	//очистить массив
	void clear() {
		for (size_t i = 0; i < size(); ++i) {
			vectorArray[i] = DataType();
		}
		vectorEnd = vectorArray;
	}

	//operator<< для вывода через iostream
	friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
		os << "(";
		for (size_t i = 0; i < vec.size()-1; i++) {
			os << vec[i] << ", ";
		}
		os << vec[vec.size() - 1] << ")";
		return os;
	}
};