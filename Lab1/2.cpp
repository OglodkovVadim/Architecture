#include <iostream>
#include <vector>

class Passenger {
public:
	Passenger() {};
};

class Driver {
public:
	Driver() {};
};

class BusDriver : public Driver {
private:
	BusDriver() {}
public:
	static BusDriver& getDriver() {
		static BusDriver driver;
		return driver;
	}

	BusDriver(const BusDriver&) = delete;
	BusDriver& operator=(const BusDriver&) = delete;
};

class TaxiDriver : public Driver {
private:
	TaxiDriver() {}
public:
	static TaxiDriver& getDriver() {
		static TaxiDriver driver;
		return driver;
	}
	TaxiDriver(const TaxiDriver&) = delete;
	TaxiDriver& operator=(const TaxiDriver&) = delete;
};

class BoardAnyCar {
public:
	virtual void BoardDriver() = 0;
	virtual void BoardPassenger() = 0;

	unsigned int limit;
	Driver* driver;
	std::vector<Passenger> passengers;
};

class BoardBus : public BoardAnyCar {
public:
	BoardBus() { limit = 30; }

	void BoardDriver() override;
	void BoardPassenger() override;
};

void BoardBus::BoardDriver()
{
	driver = &BusDriver::getDriver();
}

void BoardBus::BoardPassenger()
{
	if (passengers.size() < 30)
		passengers.push_back(Passenger());
	else
		std::cout << "Limit bus passengers" << std::endl;
}

class BoardTaxi : public BoardAnyCar {
public:
	BoardTaxi() { limit = 30; }

	void BoardDriver() override;
	void BoardPassenger() override;
};

void BoardTaxi::BoardDriver()
{
	driver = &TaxiDriver::getDriver();
}

void BoardTaxi::BoardPassenger()
{
	if(passengers.size() < 4)
		passengers.push_back(Passenger());
	else
		std::cout << "Limit taxi passengers" << std::endl;
}

int main()
{
	BoardBus bb;
	BoardTaxi bt;

	bb.BoardDriver();
	bb.BoardDriver();

	bt.BoardDriver();
	bt.BoardDriver();

	for (int i = 0; i < 33; i++)
		bb.BoardPassenger();
	for (int i = 0; i < 7; i++)
		bt.BoardPassenger();

	return 0;
}