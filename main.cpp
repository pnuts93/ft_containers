#include <iostream>
#include <string>
#include <deque>
#define C_RESET "\033[0m"
#define C_GREEN "\033[32m"

#if 0 //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	#include <map.hpp>
	#include <stack.hpp>
	#include <vector.hpp>
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};


#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

int main(int argc, char** argv) {
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	ft::map<int, int> map_int;

	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}

	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(rand(), rand()));
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		ft::map<int, int> copy = map_int;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// My Tests
	std::cout	<< "####################\n"\
				<< "#     MY TESTS     #\n"\
				<< "####################\n" << std::endl;
	// Vector
	{
		std::cout << "VECTOR\n" << std::endl;
		ft::vector<char> vec1;
		for (char letter = 'a'; letter < 'h'; letter++)
			vec1.push_back(letter);
		ft::vector<char> vec2(vec1);
		vec1.push_back('z');
		vec2 = vec1;
		ft::vector<char>::iterator begin = vec2.begin() + 3;
		ft::vector<char>::iterator end = vec2.end() - 2;
		ft::vector<char> vec3(begin, end);

		std::cout << vec3.at(0) << "\t" << vec3.at(vec3.size() - 1) << std::endl;
		std::cout << vec3[0] << "\t" << vec3[vec3.size() - 1] << std::endl;
		std::cout << vec3.front() << "\t" << vec3.back() << std::endl;
		std::cout << "vector is " << (vec3.empty() ? "empty" : "not empty") << " of size " << vec3.size() << std::endl;
		vec3.pop_back();
		vec3.erase(vec3.begin());
		vec3.insert(vec3.begin() + 1, 'Q');
		vec3.resize(5);
		vec3.swap(vec1);
		ft::vector<char>::const_iterator limit = vec1.end();
		std::cout << (limit == vec1.begin()) << (limit >= vec1.begin()) << (limit <= vec1.begin()) << (limit < vec1.begin()) << (limit > vec1.begin()) << std::endl;
		for (ft::vector<char>::iterator it = vec1.begin(); it != limit; it++)
			std::cout << *it << std::endl;
		ft::vector<char>::const_reverse_iterator rlimit = vec1.rend();
		std::cout << vec1.size() << std::endl;
		for (ft::vector<char>::reverse_iterator it = vec1.rbegin(); it != rlimit; it++)
			std::cout << *it << std::endl;
		vec1.clear();
		std::cout << "vector is " << (vec1.empty() ? "empty" : "not empty") << " of size " << vec1.size() << std::endl;
	}

		// Map
	{
		std::cout << std::endl;
		std::cout << "MAP\n" << std::endl;
		ft::map<char, int> map1;
		for (struct {char c;int n;} s = {'a', 1}; s.c < 'h'; s.c++, s.n*=2)
			map1.insert(ft::make_pair(s.c, s.n));
		ft::map<char, int> map2(map1);
		map1['z'] = 42;
		map2 = map1;
		ft::map<char, int>::iterator begin = map2.begin();
		for (int i = 0; i < 3; i++)
			begin++;
		ft::map<char, int>::iterator end = map2.end();
		end--;
		end--;
		ft::map<char, int> map3(begin, end);
		std::cout << (map2 < map3) << (map2 > map3) << (map2 <= map3) << (map2 >= map3) << (map2 != map3) << std::endl;

		std::cout << map3.find('d')->second << "\t" << map3.find('f')->second << std::endl;
		std::cout << map3['d'] << "\t" << map3['f'] << std::endl;
		std::cout << "map is " << (map3.empty() ? "empty" : "not empty") << " of size " << map3.size() << std::endl;
		map3.erase('f');
		map3.erase(map3.begin());
		map3.insert(++map3.begin(), ft::make_pair('Q', 21));
		map3.swap(map1);
		std::cout << map1.upper_bound('Q')->first << "\t" << map1.lower_bound('Q')->first << "\t" << map1.equal_range('Q').first->first << std::endl;
		std::cout << map1.upper_bound(' ')->first << "\t" << map1.lower_bound(' ')->first << "\t" << map1.equal_range(' ').first->first << std::endl;
		ft::map<char, int>::const_iterator limit = map1.end();
		std::cout << (limit == map1.begin()) << std::endl;
		for (ft::map<char, int>::iterator it = map1.begin(); it != limit; it++)
			std::cout << it->first << std::endl;
		ft::map<char, int>::const_reverse_iterator rlimit = map1.rend();
		std::cout << map1.size() << std::endl;
		for (ft::map<char, int>::reverse_iterator it = map1.rbegin(); it != rlimit; it++)
			std::cout << it->first << std::endl;
		map1.clear();
		std::cout << "map is " << (map1.empty() ? "empty" : "not empty") << " of size " << map1.size() << std::endl;
	}

		// Stack
	{
		std::cout << std::endl;
		std::cout << "STACK\n" << std::endl;
		ft::stack<char> stack1;
		ft::stack<int> stack2;
		stack2.push(70);
		stack2.push(84);
		stack2.push(95);
		stack2.push(67);
		stack2.push(79);
		stack2.push(78);
		stack2.push(84);
		stack2.push(65);
		stack2.push(73);
		stack2.push(78);
		stack2.push(69);
		stack2.push(82);
		stack2.push(83);
		while (stack2.size() > 0)
		{
			stack1.push((char)stack2.top());
			stack2.pop();
		}
		ft::stack<char> stack3(stack1);
		ft::stack<char> stack4 = stack3;
		std::cout << "The copy assigned stack is " << (stack4 == stack3 ? "identical" : "not identical (how?)") << " to the copied one" << std::endl;
		std::cout << (stack4 < stack3) << (stack4 > stack3) << (stack4 <= stack3) << (stack4 >= stack3) << (stack4 != stack3) << std::endl;
		while (stack4.size() > 0)
		{
			std::cout << stack4.top();
			stack4.pop();
		}
		std::cout << std::endl;
	}
	return (0);
}
