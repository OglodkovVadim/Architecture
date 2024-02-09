#include <iostream>

class MySingleton {
public:
	static MySingleton& getInstance();

	MySingleton(const MySingleton&) = delete;
	MySingleton& operator = (const MySingleton&) = delete;

	inline void setValue(unsigned int value) { this->value = value; }
	inline unsigned int getValue() { return this->value; }

protected:
	unsigned int value;

private:
	MySingleton() : value(0) {};

};

MySingleton& MySingleton::getInstance() 
{
	static MySingleton instance;
	return instance;
}

int main() 
{
	MySingleton::getInstance().setValue(0x05);
	std::cout << MySingleton::getInstance().getValue() << std::endl;

	return 0;
}