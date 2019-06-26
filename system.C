#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>

#include "system.h"
using std::cerr;

void
system_error(const char* msg)
{
	auto e = strerror(errno);
	cerr << msg << ": " << e << "\n";
	exit(1);
}

void
deal_with_child(int pid)
{
	int r;
	auto e = waitpid(pid, &r, 0);
	if (e == -1)
		system_error("waitpid");
	if (e != pid) {
		cerr << "waitpid exited with " << e << 
		    "(shouldn't happen)\n";
		exit(1);
	}
		
	if (WIFEXITED(r)) {
		auto s = WEXITSTATUS(r);
		if (s != 0) {
			cerr << "Command exited with "<< s << "\n";
		}
	} else {
		auto s = WTERMSIG(r);
		cerr << "Command exited on signal #"<< s << "\n";
		kill(getpid(), s);
		// in case we didn't die
		exit(1);
	}
}

