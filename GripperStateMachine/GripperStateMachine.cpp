// GripperStateMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"

using namespace std;

#define GRIPPER_OPENING 0
#define GRIPPER_CLOSING 1
#define GRIPPER_OPEN	3
#define GRIPPER_CLOSED_OBJECT_NOT_PRESENT 4
#define GRIPPER_CLOSED_OBJECT_PRESENT 5
#define GRIPPER_BAD_DRIVE 666

int gripperState;

void resolveState();
inline bool handleBadDriveState();
inline bool handleOpeningState();
inline bool handleClosingState();
inline bool handleOpenedState();
inline bool handleClosedObjectPresentState();
inline bool handleClosedObjectNotPresentState();

bool gripperOpen, gripperClose, gripperStopped;
double analogPosition;

double openLimit = 245; // analog pos > openlimit -> gripper open
double closeLimit = 2; // analogpos < closelimit -> gripper closed

int main()
{

	// opening
	analogPosition = 150;
	gripperOpen = true;
	gripperClose = false;
	gripperStopped = false;
	resolveState();

	std::cout << "Opening state, should be 0 : " << gripperState <<	"\n";

	// opened
	analogPosition = 250;
	gripperOpen = false;
	gripperClose = false;
	gripperStopped = true;
	resolveState();

	std::cout << "Opening state, should be 3 : " << gripperState << "\n";


	// closing
	analogPosition = 150;
	gripperOpen = false;
	gripperClose = true;
	gripperStopped = false;
	resolveState();

	std::cout << "Closing state, should be 1 : " << gripperState << "\n";


	// closed without object
	analogPosition = 1;
	gripperOpen = false;
	gripperClose = false;
	gripperStopped = true;
	resolveState();

	std::cout << "Closed wo obj state, should be 4 : " << gripperState << "\n";


	// closed without object
	analogPosition = 150;
	gripperOpen = false;
	gripperClose = false;
	gripperStopped = true;
	resolveState();

	std::cout << "Closed with obj state, should be 5 : " << gripperState << "\n";


	char c;
	std::cin >> c;

    return 0;
}


void resolveState() {

	if (handleBadDriveState()) {
		gripperState = GRIPPER_BAD_DRIVE;
		return;
	}

	if (handleOpeningState()) {
		gripperState = GRIPPER_OPENING;
		return;
	}

	if (handleOpenedState()) {
		gripperState = GRIPPER_OPEN;
		return;
	}

	if (handleClosedObjectPresentState()) {
		gripperState = GRIPPER_CLOSED_OBJECT_PRESENT;
		return;
	}

	if (handleClosedObjectNotPresentState()) {
		gripperState = GRIPPER_CLOSED_OBJECT_NOT_PRESENT;
		return;
	}

	if (handleClosingState()) {
		gripperState = GRIPPER_CLOSING;
		return;
	}
}

inline bool handleBadDriveState() {
	return gripperClose == true && gripperOpen == true;
}


inline bool handleOpeningState() {
	return (gripperOpen == true && gripperStopped == false);
}

inline bool handleOpenedState() {
	return (gripperStopped == true && analogPosition > openLimit);
}

inline bool handleClosedObjectPresentState() {
	return (gripperStopped == true && analogPosition <= openLimit &&analogPosition >= closeLimit);
}

inline bool handleClosedObjectNotPresentState() {
	return (gripperStopped == true && analogPosition <= closeLimit);
}

inline bool handleClosingState() {
	return (gripperStopped == false && gripperClose == true);
}