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

static barrier_t barrier;
static int nodes[NANVIX_PROC_MAX];

/**
* @brief Synchronizer used to barrier the barriers.
*/
static void synchronizer(void)
{
	/* Creates the barrier. */
	barrier = barrier_create(nodes, NANVIX_PROC_MAX);
	uassert(BARRIER_IS_VALID(barrier));

	uassert(barrier_wait(barrier) == 0);

	uprintf("[examples][sync][barrier] Waiting barrier");

	uassert(barrier_wait(barrier) == 0);

	uassert(barrier_destroy(barrier) == 0);
} 

/**
 * @brief Barrier examples using sync.
 */
static void sync_barrier(void)
{
	/* Build list of nodes. */
	for (int i = 0; i < NANVIX_PROC_MAX; i++)
		nodes[i] = PROCESSOR_NODENUM_LEADER + i;
	
	synchronizer();
}

/**
 * @brief Launches the example..
 */
int __main3(int argc, const char *argv[])
{
	((void) argc);
	((void) argv);

	sync_barrier();

	return (0);
}
