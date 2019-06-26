/*
 * Copyright (c) 2019 Marc Espie <espie@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
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

