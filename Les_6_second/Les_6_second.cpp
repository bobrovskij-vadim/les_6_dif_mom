#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <iterator>
#include <algorithm>

std::mutex m;

class Things
{
private:
	std::vector <int> vec{ 10, 24, 23, 15, 78 };

public:

	void print_vec() const
	{
		std::cout << std::endl;
		copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}

	void add_things()
	{
		m.lock();
		for (int i = 0; i < 10; ++i)
		{
			vec.push_back(rand() % 100);
			print_vec();
		}
		//vec.push_back(rand() % 100);

		m.unlock();
	}

	void steal_things()
	{
		m.lock();
		for (int i = 0; i < 10; ++i)
		{
			vec.erase(std::max_element(vec.cbegin(), vec.cend()));
			print_vec();
		}
		//vec.erase(std::max_element(vec.cbegin(), vec.cend()));
		m.unlock();
	}




	std::vector<int> get_vec()
	{
		return vec;
	}

};

void pcout(std::string text)
{
	m.lock();
	for (int i = 0; i < 10; ++i)
	{
		std::cout << text << ": " << std::this_thread::get_id() << std::endl;
	}
	m.unlock();
}

int prime_number(int num)
{
	int res = 0;
	for (int j = 2; j <= num; ++j)
	{
		int i = 2;
		for (; i <= j - 1; i++)
		{
			if (j % i == 0)
				break;
		}

		if (i == j && i != 2)
		{
			std::cout << j << " ";
			res = j;
		}
	}
	std::cout << std::endl;


	return res;
}


void print_vec(std::vector<int>& vec)
{
	copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}

void add_things(std::vector<int>& vec)
{

	for (int i = 0; i < 10; ++i)
	{
		vec.push_back(rand() % 100);
		print_vec(vec);
	}

}

void steal_things(std::vector<int>& vec)
{

	for (int i = 0; i < 10; ++i)
	{
		vec.erase(std::max_element(vec.cbegin(), vec.cend()));
		print_vec(vec);
    }

}

int main()
{
	//-----------Ex_1-----------
	{
		std::cout << "-----------Ex_1-----------" << std::endl;
		std::thread t1(pcout, "Thread 1");
		std::thread t2(pcout, "Thread 2");

		t1.join();
		t2.join();
	}
	//-----------Ex_2-----------
	{
		std::cout << "\n\n-----------Ex_2-----------" << std::endl;
		int sim_num;
		std::thread t3([&sim_num]() {sim_num = prime_number(100); });
		t3.join();
		std::cout << "The largest prime number: " << sim_num << std::endl;
	}

	//-----------Ex_3-----------
	{
		std::cout << "\n-----------Ex_3-----------" << std::endl;

		//--------------Первый вариант (не самый правильный)-----------------
		{
			std::vector <int> vec{ 10, 15, 25, 35, 78 };

			print_vec(vec);

			std::thread owner(add_things, ref(vec));

			owner.join();

			print_vec(vec);

			std::thread thief(steal_things, ref(vec));

			thief.join();

			print_vec(vec);
		}

		//--------------Второй вариант (через класс)-----------------
		{
			Things things;

			things.print_vec();

			std::thread t4([&]() { things.add_things(); });
			std::thread t5([&]() {things.steal_things(); });

			t4.join();
			t5.join();

		

		}
	}
}