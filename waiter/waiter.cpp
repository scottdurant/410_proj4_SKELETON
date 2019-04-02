#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/file_IO.h"
#include "../includes/externs.h"


using namespace std;

//ID is just a number used to identify this particular baker
//(used with PRINT statements)
//filename is what waiter reads in orders from
Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order from file_IO
//if return == SUCCESS then anOrder
//contains new order
//otherwise return contains fileIO error

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	int result = myIO.getNext(anOrder);

	// if there's no more orders, waiter is finished
	if (result != SUCCESS) {
		b_WaiterIsFinished = true;
		return NO_ORDERS;
	}

	return SUCCESS;
}

//contains a loop that will get orders from filename one at a time
//then puts them in order_in_Q then signals baker(s) using cv_order_inQ
//so they can be consumed by baker(s)
//when finished exits loop and signals baker(s) using cv_order_inQ that
//it is done using b_WaiterIsFinished
void Waiter::beWaiter() {
	ORDER someOrder;

	// get the next order
	getNext(someOrder);
	while (!b_WaiterIsFinished) {
		{
			lock_guard<mutex> lck(mutex_order_inQ);
			order_in_Q.push(someOrder);
		}

		// tell baker that there are orders ready
		cv_order_inQ.notify_all();
		getNext(someOrder);
	}
}

