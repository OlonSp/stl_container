#pragma once
#include <iterator>

// Потоковый итератор вывода
template< class T,
	class CharT = char,
	class Traits = std::char_traits<CharT>>
class OutputIterator {
public:
	using iterator_category = output_iterator_tag;
	using value_type = void;
	using difference_type = ptrdiff_t;
	using pointer = void;
	using reference = void;
	using char_type = CharT;
	using traits_type = Traits;
	using ostream_type = basic_ostream<CharT, Traits>;

	OutputIterator(ostream_type& stream, const CharT* delim = nullptr)
		: _delim(delim), _ostr(&stream) {}

	// Перегруженная операция присваивания
	OutputIterator& operator=(const T& product) {
		*_ostr << product;
		if (_delim) {
			*_ostr << _delim;
		}
		return *this;
	}

	OutputIterator& operator*() {
		return *this;
	}

	OutputIterator& operator++() {
		return *this;
	}

	OutputIterator& operator++(int) {
		return *this;
	}

private:
	const CharT* _delim;
	ostream_type* _ostr;
};