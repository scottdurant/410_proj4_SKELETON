/**
 * I (Scott Durant) worked with Todd Sandburg on this project.
 * We both worked on it at the same time, so the work was split 50/50.
 */

#include <mutex>
#include "../includes/baker.h"
#include "../includes/externs.h"

using namespace std;

Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	int numDonuts = anOrder.number_donuts;

	// number of full boxes
	int numBoxes = numDonuts / 12;

	// fill the full boxes
	for (int i = 0; i < numBoxes; i++) {
		Box box;
		for (int j = 0; j < 12; j++) {
			DONUT d;
			box.addDonut(d);
		}
		anOrder.boxes.push_back(box);
	}

	// check if we need one more box
	if (numDonuts % 12 > 0) {
		Box additionalBox;
		for (int k = 0; k < numDonuts % 12; k++) {
			DONUT d;
			additionalBox.addDonut(d);
		}
		anOrder.boxes.push_back(additionalBox);
	}
}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {
	while (true) {
		unique_lock<mutex> inLock(mutex_order_inQ);

		while (order_in_Q.empty() && !b_WaiterIsFinished) {
			cv_order_inQ.wait(inLock);
		}
		// if there's orders, bake and box them
		if (!order_in_Q.empty()) {
			ORDER o;
			o = order_in_Q.front();
			order_in_Q.pop();

			// we aren't accessing order_inQ anymore
			inLock.unlock();

			bake_and_box(o);

			lock_guard<mutex> outLock(mutex_order_outQ);
			// push the order to the order_out_vector
			order_out_Vector.push_back(o);
		}

		// if no more orders and waiter is done, baker is done
		if (order_in_Q.empty() && b_WaiterIsFinished) {
			break;
		}
	}
}

