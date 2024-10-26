#pragma once
#include <iostream>
#include <algorithm>





template <typename Lambda, typename ItererType>
concept ConceptLambda = requires(Lambda   func, ItererType i) {
	func(i);
};

template <typename ItererType, typename PredType>
ItererType findIf(ItererType beg, ItererType end, PredType pred) {
	while (beg != end) {
		if (pred(*beg)) {
			break;
		}
		++beg;
	}
	ItererType result(beg);
	return result;
}

template<typename ItererType>
ItererType minElement(ItererType beg, ItererType end) {
	auto minElem = beg;
	while (beg != end) {
		if (*beg < *minElem) {
			minElem = beg;
		}
		++beg;
	}
	return minElem;
}

template<typename ItererType>
ItererType maxElement(ItererType beg, ItererType end) {
	auto maxElem = beg;
	while (beg != end) {
		if (*beg > *maxElem) {
			maxElem = beg;
		}
		++beg;
	}
	return maxElem;
}

template <typename ItererType, typename _OperationType>
void forEach(ItererType beg, ItererType end, _OperationType op) {
	while (beg != end) {
		op(*beg);
		beg++;
	}
}

template <typename ItererType>
void Sort(ItererType beg, ItererType end) {                  
	for (auto iter = beg; iter != end; ++iter) {
		std::iter_swap(iter, minElement(iter, end));
	}
}


template <typename ItererTypeA, typename ItererTypeB, typename PredType>
void copyIf(ItererTypeA sourceBeg, ItererTypeA sourceEnd,
	ItererTypeB destBeg, PredType		pred) {
	while (sourceBeg != sourceEnd) {
		if (pred(*sourceBeg)) {
			*destBeg = *sourceBeg;
			++destBeg;
		}
		++sourceBeg;
	}
}


