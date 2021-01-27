/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <nanvix/runtime/runtime.h>
#include <nanvix/runtime/barrier.h>
#include <nanvix/limits.h>
#include <nanvix/ulib.h>

#define NCLUSTERS 2
#define PORT_NUM 10

static barrier_t barrier;
static int nodes[NCLUSTERS];

static void do_leader(void)
{
	int inbox, outbox;
	char msg[KMAILBOX_MESSAGE_SIZE];

	uassert((inbox = kmailbox_create(nodes[0], PORT_NUM)) >= 0);
	uassert((outbox = kmailbox_open(nodes[1], PORT_NUM)) >= 0);

	uassert(barrier_wait(barrier) == 0);

	/* send process name */
	ustrcpy(msg, nanvix_getpname());
	uassert(
		kmailbox_write(
			outbox,
			msg,
			KMAILBOX_MESSAGE_SIZE
		) == KMAILBOX_MESSAGE_SIZE
	);

	/* receive worker process name */
	uassert(
		kmailbox_read(
			inbox,
			msg,
			KMAILBOX_MESSAGE_SIZE
		) == KMAILBOX_MESSAGE_SIZE
	);

	uprintf("%s found %s at node %d",
		nanvix_getpname(), msg, nanvix_name_lookup(msg)
	);

	uassert(barrier_wait(barrier) == 0);

	uassert(kmailbox_close(outbox) == 0);
	uassert(kmailbox_unlink(inbox) == 0);
}

static void do_worker(void)
{
	int inbox, outbox;
	char msg[KMAILBOX_MESSAGE_SIZE];

	uassert((inbox = kmailbox_create(nodes[1], PORT_NUM)) >= 0);
	uassert((outbox = kmailbox_open(nodes[0], PORT_NUM)) >= 0);

	uassert(barrier_wait(barrier) == 0);

	/* send process name */
	ustrcpy(msg, nanvix_getpname());
	uassert(
		kmailbox_write(
			outbox,
			msg,
			KMAILBOX_MESSAGE_SIZE
		) == KMAILBOX_MESSAGE_SIZE
	);

	/* receive leader process name*/
	uassert(
		kmailbox_read(
			inbox,
			msg,
			KMAILBOX_MESSAGE_SIZE
		) == KMAILBOX_MESSAGE_SIZE
	);

	uprintf("%s found %s at node %d",
		nanvix_getpname(), msg, nanvix_name_lookup(msg)
	);

	uassert(barrier_wait(barrier) == 0);

	uassert(kmailbox_close(outbox) == 0);
	uassert(kmailbox_unlink(inbox) == 0);
}

static void name_lookup(void)
{
	void (*fn)(void);

	fn = (knode_get_num() == PROCESSOR_NODENUM_LEADER) ? do_leader : do_worker;

	for (int i = 0; i < NCLUSTERS; i++)
		nodes[i] = PROCESSOR_NODENUM_LEADER + i;

	barrier = barrier_create(nodes, NCLUSTERS);
	uassert(BARRIER_IS_VALID(barrier));

	fn();

	uassert(barrier_destroy(barrier) == 0);
}

int __main3(int argc, const char *argv[])
{
	((void) argc);
	((void) argv);

	name_lookup();

	return (0);
}
