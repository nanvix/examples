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
#include <nanvix/sys/perf.h>
#include <nanvix/limits.h>
#include <nanvix/ulib.h>

/**
* @brief Port number and total of clusters used.
*/
#define NCLUSTERS 2
#define PORT_NUM 10

static barrier_t barrier;
static int nodes[NCLUSTERS];

/**
* @brief Dummy message.
*/
static char msg[KMAILBOX_MESSAGE_SIZE];

/**
* @brief Receives messages from worker.
*/
static void do_leader(void) 
{
	int inbox, outbox;
	uint64_t latency, volume;

	uassert((inbox = kmailbox_create(knode_get_num(), PORT_NUM)) >= 0);
	uassert((outbox = kmailbox_open(PROCESSOR_NODENUM_LEADER + 1, PORT_NUM)) >= 0);

	uassert(barrier_wait(barrier) == 0);

	uassert(kmailbox_read(inbox, msg, KMAILBOX_MESSAGE_SIZE) == KMAILBOX_MESSAGE_SIZE);
	uassert(kmailbox_write(outbox, msg, KMAILBOX_MESSAGE_SIZE) == KMAILBOX_MESSAGE_SIZE);

	uassert(kmailbox_ioctl(inbox, KMAILBOX_IOCTL_GET_LATENCY, &latency) == 0);
	uassert(kmailbox_ioctl(inbox, KMAILBOX_IOCTL_GET_VOLUME, &volume) == 0);

	uprintf("[mailbox][pingpong] latency=%l volume=%l", latency, volume);
	
	uassert(barrier_wait(barrier) == 0);

	uassert(kmailbox_close(outbox) == 0);
	uassert(kmailbox_unlink(inbox) == 0);
}

/**
 * @brief Sends menssages to leader.
 */
static void do_worker(void)
{
	int outbox, inbox;

	uassert((inbox = kmailbox_create(knode_get_num(), PORT_NUM)) >= 0);
	uassert((outbox = kmailbox_open(PROCESSOR_NODENUM_LEADER, PORT_NUM)) >= 0);

	uassert(barrier_wait(barrier) == 0);

	uassert(kmailbox_write(outbox, msg, KMAILBOX_MESSAGE_SIZE) == KMAILBOX_MESSAGE_SIZE);
	uassert(kmailbox_read(inbox, msg, KMAILBOX_MESSAGE_SIZE) == KMAILBOX_MESSAGE_SIZE);

	uassert(barrier_wait(barrier) == 0);

	uassert(kmailbox_close(outbox) == 0);
	uassert(kmailbox_unlink(inbox) == 0);
}
/**
* @brief Pingpong communication example with mailboxes.
*/
static void mail_pingpong(void)
{
	void (*fn)(void);

	fn = (knode_get_num() == PROCESSOR_NODENUM_LEADER) ?
	       do_leader : do_worker;

	for (int i = 0; i <2; i++)
		nodes[i] = PROCESSOR_NODENUM_LEADER + i;

	barrier = barrier_create(nodes, 2);
	uassert(BARRIER_IS_VALID(barrier));
	
		fn();

	uassert(barrier_destroy(barrier) == 0);
}

/**
* @brief Launches the examples.
*/
int __main3(int argc, const char *argv[])
{
	((void) argc);
	((void) argv);

	mail_pingpong();

	return(0);
}
