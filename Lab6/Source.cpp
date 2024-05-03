#include <iostream>
#include <vector>
#include <string>


bool random()
{
	if (rand() % 2 == 1)
		return true;
	return false;
}


class IObserver
{
public:
	virtual void update() = 0;
};

class ISubject
{
protected:
	std::vector<IObserver*> observers;
public:
	virtual void attach(IObserver*) = 0;
	virtual void detach(IObserver*) = 0;
	virtual void notify() = 0;
};



class Teacher : public ISubject
{
public:
	void attach(IObserver* observer) {
		observers.push_back(observer);
	}

	void detach(IObserver* observer) {
		observers.erase(
			std::find(
				observers.begin(),
				observers.end(),
				observer
			)
		);
	}

	void notify() {
		for (auto& var : observers)
			var->update();
	}

	void create_info() {
		mark = rand() % 100 + 1;
		notify();
	}

private:
	int mark = 0;
};

class Decanat : public IObserver, public ISubject
{
public:
	void update() {
		if (random())
			notify();
	}

	void attach(IObserver* observer) {
		observers.push_back(observer);
	}

	void detach(IObserver* observer) {
		observers.erase(
			std::find(
				observers.begin(),
				observers.end(),
				observer
			)
		);
	}

	void notify() {
		for (auto& var : observers)
			var->update();
	}
};

class Kafedra : public IObserver
{
public:
	void update() {
		std::cout << "didn't pass it on time";
	}
};



int main()
{
	Kafedra* kafedra = new Kafedra();
	Decanat* decanat = new Decanat();
	Teacher* teacher1 = new Teacher();
	Teacher* teacher2 = new Teacher();
	Teacher* teacher3 = new Teacher();

	teacher1->attach(decanat);
	teacher2->attach(decanat);
	teacher3->attach(decanat);

	decanat->attach(kafedra);

	std::cout << "teacher1: ";
	teacher1->create_info();
	std::cout << std::endl;

	std::cout << "teacher2: ";
	teacher2->create_info();
	std::cout << std::endl;

	std::cout << "teacher3: ";
	teacher3->create_info();
	std::cout << std::endl;

	return 0;
}