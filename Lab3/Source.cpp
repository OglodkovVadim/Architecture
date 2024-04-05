#include <iostream>
#include <vector>
#include <algorithm>

int total_weight = 0;
int actual_weight = 0;
std::vector<int> removed_baggage;

// Component
class Unit 
{
public:
	virtual void addUnit(Unit*) {};
	virtual int getWeight() = 0;
	virtual void info() {};
	virtual ~Unit() {}
};


class Plane : public Unit
{
public:
	Plane() { limit = 1000; };
	Plane(int limit_) { limit = limit_; };

	void addUnit(Unit* unit) override {
		units.push_back(unit);
	}
	int getWeight() {
		int total = 0;
		for (auto& var : units) total += var->getWeight();
		return total;
	}

	void info() {
		
		std::cout << "Total weight: " << total_weight << "\nOverweight: " << total_weight - Plane::limit << std::endl;
		std::cout << "Removed baggages: ";
		int sum = 0;
		for (auto& var : removed_baggage) {
			sum += var;
			std::cout << var << " ";
		}
		std::cout << "\nTotal removed: " << sum << std::endl;
		std::cout << "Actual weight : " << actual_weight << std::endl;
		std::cout << "\nFirst class:" << std::endl;
		units[0]->info();
		std::cout << "Business class:" << std::endl;
		units[1]->info();
		std::cout << "Economy class:" << std::endl;
		units[2]->info();
	}

	static int limit;
private:
	std::vector<Unit*> units;
};


class PersonUnit : public Unit
{
public:
	inline int getWeight() override { return weight; }
protected:
	int weight = 0;
};

class Passenger : public PersonUnit
{
public:
	Passenger() { weight = 0; }
	Passenger(int weight_) { weight = weight_; }

	void reduce() { weight = 0; }
};

class Stewardess : public PersonUnit
{};

class Pilot : public PersonUnit
{};


class ClassUnit : public Unit 
{
public:
	void addUnit(Unit* unit) override {
		if (units.size() < limit) {
			total_weight += unit->getWeight();
			units.push_back(unit);
		}
	}

	int getWeight() {
		int total = 0;
		for (auto& var : units) total += var->getWeight();
		return total;
	}

	void info() override {
		std::cout << "Num of passengers: " << units.size() << std::endl;
		std::cout << "Passengers: ";
		for (auto& var : units) {
		std::cout << var->getWeight() << " ";
		}
		std::cout << std::endl << std::endl;
	}

	~ClassUnit() {
		for (auto& var : units) delete var;
	}
protected:
	std::vector<Unit*> units;
	int limit;
};

class EconomyClass : public ClassUnit 
{
public:
	EconomyClass() { limit = 150; }

	void addUnit(Unit* unit) override {
		if (units.size() < limit) {
			total_weight += unit->getWeight();
			if (actual_weight + unit->getWeight() > Plane::limit) {
				removed_baggage.push_back(unit->getWeight());
				dynamic_cast<Passenger*>(unit)->reduce();
			}
			units.push_back(unit);
			actual_weight += unit->getWeight();
		}
	}
};

class BusinessClass : public ClassUnit 
{
public:
	BusinessClass() { limit = 20; }
};

class FirstClass blic ClassUnit 
{
public:
	FirstClass() { limit = 10; }
};


int Plane::limit;

int main()
{
	
	Plane* plane = new Plane(4000);

	FirstClass* first_class = new FirstClass();
	BusinessClass* business_class = new BusinessClass();
	EconomyClass* economy_class = new EconomyClass();

	for (int i = 0; i < 12; i++) first_class->addUnit(new Passenger(rand() % 56 + 5));
	for (int i = 0; i < 30; i++) business_class->addUnit(new Passenger(rand() % 56 + 5));
	for (int i = 0; i < 160; i++) economy_class->addUnit(new Passenger(rand() % 56 + 5));

	plane->addUnit(first_class);
	plane->addUnit(business_class);
	plane->addUnit(economy_class);

	plane->info();

	return 0;
}