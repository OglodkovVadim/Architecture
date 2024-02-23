#include <iostream>
#include <vector>

enum PassengerType {
	Child,
	Adult,
	Beneficiary
};

enum DriverType {
	Bus,
	Taxi
};

class Passenger {
private:
	PassengerType type;
public:
	Passenger(PassengerType type) { this->type = type; }

	void info() {
		if (type == Child) std::cout << "Child";
		else if (type == Adult) std::cout << "Adult";
		else if (type == Beneficiary) std::cout << "Beneficiary";
	}
};

class Driver {
private:
	DriverType type;
public:
	Driver(DriverType type) { this->type = type; }
};

class Board {
protected:
	Driver* driver;
	unsigned int limit;
	std::vector<Passenger> passengers;
public:
	virtual void boardPassenger(PassengerType type) {
		if (passengers.size() < limit)
			passengers.push_back(Passenger(type));
		else
			std::cout << "\tLimit passengers" << std::endl;
	}

	inline virtual void addChildSeat() {};

	virtual void info() = 0;
};

class BusBoard : public Board {
public:
	BusBoard() {
		driver = new Driver(Bus);
		limit = 30;
	}

	void info() {
		std::cout << "\nBus info:" << std::endl;
		for (auto var : passengers) {
			std::cout << "\tPassenger: "; var.info(); std::cout << std::endl;
		}
	}
};

class TaxiBoard : public Board {
private:
	unsigned int childSeat = 0;
public:
	TaxiBoard() {
		driver = new Driver(Taxi);
		limit = 4;
	}

	inline void addChildSeat() { childSeat++; };

	void info() {
		std::cout << "\nTaxi info:" << std::endl;
		for (auto var : passengers) {
			std::cout << "\tPassenger: "; var.info(); std::cout << std::endl;
		}
		std::cout << "Count child seats: " << childSeat << std::endl;
	}
};

class BoardBuilder {
protected:
	Board* ptr;
public:
	BoardBuilder() : ptr(0) {}

	virtual void createBoard() = 0;
	virtual void boardPassenger() = 0;
	inline virtual Board* getBoard() { return ptr; }
};

class BoardBusBuilder : public BoardBuilder {
public:
	void createBoard() override { ptr = new BusBoard(); }
	void boardPassenger() override {
		std::cout << "\nBoarding on bus:" << std::endl;
		for (int i = 0; i < 33; i++)
			ptr->boardPassenger(PassengerType(rand() % 3));
	}
};

class BoardTaxiBuilder : public BoardBuilder {
public:
	void createBoard() override { ptr = new TaxiBoard(); }
	void boardPassenger() override {
		std::cout << "\nBoarding on taxi:" << std::endl;
		for (int i = 0; i < 5; i++) {
			PassengerType type = PassengerType(rand() % 2);
			if (type == Child)
				ptr->addChildSeat();
			ptr->boardPassenger(type);
		}
	}
};

class Director {
public:
	Board* createBoard(BoardBuilder& builder) {
		builder.createBoard();
		builder.boardPassenger();

		return builder.getBoard();
	}
};

int main()
{
	Director dir;
	BoardBusBuilder bus_builder;
	BoardTaxiBuilder taxi_builder;

	Board* bus = dir.createBoard(bus_builder);
	Board* taxi = dir.createBoard(taxi_builder);

	bus->info();
	taxi->info();

	return 0;
}