/* Run when the program is executed from the command line.
 * Takes options as an input and passes them to the SwarmApp object, then executes it.
 */

#include "SwarmApp.h"

int main(int argc, char **argv) {
	SwarmApp a(argc, argv);
	a.setAppName("swarm");
	return a.exec();
}
