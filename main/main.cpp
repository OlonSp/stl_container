#include <iostream>
#include "Vector.h"
#include "Algorithm.h"
#include <fstream>
#include "Outputiterator.h"



struct Goods
{
	uint32_t m_id;                // Код товара
	string   m_name;              // Название
	string   m_manufacturer;      // Производитель
	string   m_warehouse_address; // Адрес склада
	double   m_weight;            // Вес

	Goods(uint32_t p_id = 0, const string& p_name = " ",
		const string& p_manufacturer = " ",
		const string& p_warehouse_address = " ",
		double p_weight = .0)
		: m_id(p_id), m_name(p_name),
		m_manufacturer(p_manufacturer),
		m_warehouse_address(p_warehouse_address),
		m_weight(p_weight)
	{}

	friend ostream& operator<<(ostream& os, const Goods& goods)
	{
		os << "ID:                " << goods.m_id << endl
			<< "Name:              " << goods.m_name << endl
			<< "Manufacturer:      " << goods.m_manufacturer << endl
			<< "Warehouse address: " << goods.m_warehouse_address << endl
			<< "Weight:            " << goods.m_weight << endl;
		return os;
	}
};

class functor {
public:
	auto operator()(const Goods& obj) {
		return obj.m_name == "Jeans";
	}
};

int main() {
	std::srand(std::time(nullptr));
	Vector<int> a;
	//Vector<int> b(5, 4);
	Vector<int> c{1, 2, 3, 4, 5};
	//Vector<int> d(b);
	//Vector<int> d(std::move(b));
	
	//a = std::move(b);
	//a = b;
	std::cout << "-----------Test class Vector-----------" << std::endl;
	std::cout << "Vector c: " << c << std::endl;

	for(int i = 6; i < 12; ++i)
		c.pushBack(i);
	c.pushBack(std::move(12));
	std::cout << "Vector c after some pushBack: " << c << endl << std::endl;

	c.popBack();
	std::cout << "c after popBack(): " << c << std::endl << std::endl;

	std::cout << "c.at(3) = " << c.at(3) << std::endl << std::endl;

	std::cout << "old capacity of c = " << c.capacity() << std::endl;
	c.reserve(5);
	std::cout << "new capacity of c after reserve = " << c.capacity() << std::endl << std::endl;


	std::cout << "old size of c = " << c.size() << std::endl;
	std::cout << "old capacity of c after resize = " << c.capacity() << std::endl;
	std::cout << "old vector c: " << c << std::endl;
	c.resize(27);
	std::cout << "new size of c = " << c.size() << std::endl;
	std::cout << "new capacity of c after resize = " << c.capacity() << std::endl;
	std::cout << "new vector c: " << c << std::endl << std::endl;

	std::cout << "is c empty? : " << c.empty() << std::endl << std::endl;
	
	std::cout << "-----------Test class VectorIterator-----------" << std::endl;
	VectorIterator<int> iter(&c, 3);
	std::cout << "test *iter: " << *iter << std::endl;
	std::cout << "test ++iter: " << *(++iter) << std::endl;
	std::cout << "test iter++: in process " << *(iter++) << ", after process " << *iter << std::endl;
	std::cout << "test --iter: " << *(--iter) << std::endl;
	std::cout << "test iter--: in process " << *(iter--) << ", after process " << *iter << std::endl;
	std::cout << "\nIter2 is copy of iter" << std::endl;
	VectorIterator<int> iter2(iter);
	std::cout << "test copy constract: " << *iter2 << std::endl;
	std::cout << "iter == iter2 : " << (iter == iter2) << std::endl;
	std::cout << "iter != iter2 : " << (iter != iter2) << std::endl;
	iter2 += 4;
	std::cout << "iter2 += 4 : " << *iter2 << std::endl;
	iter -= 1;
	std::cout << "iter -= 1 : " << *iter << std::endl;
	std::cout << "iter < iter2 : " << (iter < iter2) << std::endl;
	std::cout << "iter <= iter2 : " << (iter <= iter2) << std::endl;
	std::cout << "iter > iter2 : " << (iter > iter2) << std::endl;
	std::cout << "iter >= iter2 : " << (iter >= iter2) << std::endl;
	iter = iter2;
	std::cout << "iter = iter2 -> " << *iter << std::endl;
	std::cout << "iter + 2 : " << *(iter + 2) << std::endl;
	std::cout << "iter - 2 : " << *(iter - 2) << std::endl;
	std::cout << "iter[5] : " << iter[5] << std::endl << std::endl;

	std::cout << "-----------Test method of Vector, that works with iterator-----------" << std::endl;
	c.resize(10);
	std::cout << "Vector c: " << c << std::endl;
	std::cout << "Vector c begin: " << *c.begin() << std::endl;
	std::cout << "Vector c end: " << *(c.end()) << std::endl;
	Vector<int> genWithIter(c.begin(), iter);
	std::cout << "Vector generate with Vecor[beg, end): " << genWithIter << std::endl;
	c.insert(iter - 2, 20);
	std::cout << "test insert on vector c: " << c << std::endl;
	c.insert(iter + 2, c.begin(), iter);
	std::cout << "test insert[beg,end) : " << c << std::endl;
	c.resize(10);
	std::cout << "Vector c: " << c << std::endl;
 	c.erase(iter-2);
	std::cout << "test erase on vector c: " << c << std::endl;

	std::cout << "\n--------------Test algoritms-----------" << std::endl;
	Vector<int> forAlgoritm(10);
	
	auto random = [](auto& s) {
		s = std::rand() % 10 + 1;
	};

	forEach(forAlgoritm.begin(), forAlgoritm.end(), random);
	std::cout << "Vector created with forEach(random): " << forAlgoritm << std::endl;

	auto isEven = [](const auto& a) {
		return (a % 2 == 0);
	};

	auto it = findIf(forAlgoritm.begin(), forAlgoritm.end(), isEven);
	std::cout << "First even element: " << *it << std::endl;
	it = minElement(forAlgoritm.begin(), forAlgoritm.end());
	std::cout << "minElememt: " << *it << std::endl;
	it = maxElement(forAlgoritm.begin(), forAlgoritm.end());
	std::cout << "maxElememt: " << *it << std::endl;

	Sort(forAlgoritm.begin(), forAlgoritm.end());
	std::cout << "Sort vector" << forAlgoritm << std::endl;

	Vector<int> newVec(10);
	std::cout << "New vector: " << newVec << std::endl;
	copyIf(forAlgoritm.begin(), forAlgoritm.end(), newVec.begin(), isEven);
	std::cout << "New vector after copy from old vector: " << newVec << std::endl;

	//=======
	auto range = [](const auto& s) {
		return (25 < s.m_weight) && (s.m_weight < 200);
		};

	auto print = [](const auto& obj) {
		cout << "ID:                " << obj.m_id << endl
			<< "Name:              " << obj.m_name << endl
			<< "Manufacturer:      " << obj.m_manufacturer << endl
			<< "Warehouse address: " << obj.m_warehouse_address << endl
			<< "Weight:            " << obj.m_weight << endl << endl;
		};

	Vector<Goods> ContainerA{ {1, "Jeans", "H&M", "Berlin", 500},
							  {2, "Smartphones", "Xiaomi", "Pekin", 10},
							  {3, "Tabi",  "Skeleton", "Terraria", 50} };
	cout << "First batch of goods: " << endl;
	cout << "1) " << endl << ContainerA[0] << endl
		<< "2) " << endl << ContainerA[1] << endl
		<< "3) " << endl << ContainerA[2] << endl;

	Vector<Goods> ContainerB;
	Goods b0{ 4, "Cars",  "Hyundai", "Japan",  1000 };
	Goods b1{ 5, "Sushi", "Osaka", "Rostov",  10 };
	Goods b2{ 6, "Jeans", "Mech", "Moscow",    30 };
	Goods b3{ 7, "Pizza", "Uncle Jhon", "Italia", 20 };
	ContainerB.pushBack(b0);
	ContainerB.pushBack(b1);
	ContainerB.pushBack(b2);
	ContainerB.insert(ContainerB.end() - 2, b3);

	cout << "Second batch of goods: " << endl << endl;

	forEach(ContainerB.begin(), ContainerB.end(), print);

	cout << "Range insert: " << endl;
	ContainerA.insert(ContainerA.begin(), ContainerB.begin(), ContainerB.end());
	forEach(ContainerA.begin(), ContainerA.end(), print);

	cout << "Test findIf 25 < weight < 200: " << endl;
	auto itt = findIf(ContainerA.begin(), ContainerA.end(), range);
	cout << *itt << endl;

	cout << "Test copyIf to get all manufactures, that produce jeans: " << endl;
	Vector<Goods> ContainerC(2);
	copyIf(ContainerA.begin(), ContainerA.end(), ContainerC.begin(), functor());
	unsigned int i = 0;
	for (const auto& elem : ContainerC) {
		cout << i + 1 << ") " << endl << elem << endl;
		++i;
	}

	cout << "Change the address to new, using ->" << endl;
	ContainerA[1].m_warehouse_address = "Russia";
	forEach(ContainerA.begin(), ContainerA.end(), print);

	int weight = 50;
	
	// Открытие файлового потока
	std::ofstream outfile("Goods.txt");

	auto func = [weight](const Goods& goods) {
		return goods.m_weight < weight;
	};

	// Использование алгоритма copyIf для вывода товаров в файл
	copyIf(ContainerA.begin(), ContainerA.end(), OutputIterator<Goods>(outfile), func);

	outfile.close();

	std::cout << "Goods with weight < " << weight << " write in file goods.txt." << std::endl;


	return 0;
}
