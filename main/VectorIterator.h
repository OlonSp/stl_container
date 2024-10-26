#pragma once
#include "Exceptions.h"


template<typename DataType>
class Vector;

template<typename DataType>
class VectorIterator {
private:
	Vector<DataType>* vec;
	size_t cur;
public:
	friend class Vector<DataType>;
	using Vector = Vector<DataType>;
	
	//конструкторы
	VectorIterator(Vector* _vec, size_t _ind)
		: vec(_vec)
		, cur(_ind)
	{}	

	//перегрузка операторов
	VectorIterator& operator++() {
		if (cur + 1 > vec->capacity()) {
			throw IteratorException("operator ++iter");
		}
		cur++;
		return *this;
	}

	VectorIterator operator++(int) {
		if (cur + 1 > vec->capacity()) {
			throw IteratorException("operator iter++");
		}
		VectorIterator tmp(*this);
		cur++;
		return tmp;
	}

	VectorIterator& operator--() {
		if (cur - 1 < 0) {
			throw IteratorException("operator --iter");
		}
		cur--;
		return *this;
	}

	VectorIterator operator--(int) {
		if (cur - 1 < 0) {
			throw IteratorException("operator iter--");
		}
		VectorIterator tmp(*this);
		cur--;
		return tmp;
	}

	DataType& operator*() {
		return vec->vectorArray[cur];
	}

	VectorIterator operator+(int num)
	{
		if (cur + num > vec->capacity()) {
			throw IteratorException("VectorIterator::operator+");
		}
		VectorIterator tmp(*this);
		tmp.cur += num;
		return tmp;
	}

	VectorIterator operator-(int num)
	{
		if (cur - num < 0) {
			throw IteratorException("VectorIterator::operator-num");
		}
		VectorIterator tmp(*this);
		tmp.cur -= num;
		return tmp;
	}

	bool operator==(const VectorIterator& other) {
		return (vec == other.vec) && (cur == other.cur);
	}

	bool operator!=(const VectorIterator& other) {
		return (vec != other.vec) || (cur != other.cur);
	}

	bool operator<(const VectorIterator& other) {
		if (vec != other.vec)
			throw IteratorException("Not the same iterator in operator<");
		return cur < other.cur;
	}

	bool operator<=(const VectorIterator& other) {
		if (vec != other.vec)
			throw IteratorException("Not the same iterator in operator<=");
		return cur <= other.cur;
	}

	bool operator>(const VectorIterator& other) {
		if (vec != other.vec)
			throw IteratorException("Not the same iterator in operator>");
		return cur > other.cur;
	}

	bool operator>=(const VectorIterator& other) {
		if (vec != other.vec)
			throw IteratorException("Not the same iterator in operator>=");
		return cur >= other.cur;
	}

	VectorIterator& operator+=(int num) {
		if (cur + num > vec->capacity())
			throw IteratorException("Wrong num in operator+=");
		cur += num;
		return *this;
	}

	VectorIterator& operator-=(int num) {
		if (cur - num < 0)
			throw IteratorException("Wrong num in operator+=");
		cur -= num;
		return *this;
	}

	DataType* operator->() const
	{
		return vec;
	}

	DataType& operator[](size_t n) {
		if (n >= vec->size())
			throw IteratorException("Out Of range iter[n]");
		return vec->vectorArray[n];
	}

	
	operator DataType() const {
		return vec->vectorArray[cur];
	}
};
