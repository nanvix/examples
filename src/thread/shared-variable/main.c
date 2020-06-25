/*
 * MIT License
 *
 * Copyright(c) 2018 Pedro Henrique Penna <pedrohenriquepenna@gmail.com>
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
#include <nanvix/ulib.h>
#include <nanvix/sys/thread.h>
#include <nanvix/sys/semaphore.h>

/**
 * @brief Global variables.
 **/
/**@{*/
int global_counter = 0;
struct nanvix_semaphore semaphore;
/**@}*/

/**
 * @brief Increment a global variable 100 times.
 **/
void * increment(void * args)
{
	((void) args);

	for (int k = 0; k < 100; k++)
	{
		nanvix_semaphore_down(&semaphore);
		global_counter++;
		nanvix_semaphore_up(&semaphore);
	}

	return (NULL);
}

/**
 * @brief Main funtion.
 **/
int __main3(int argc, const char *argv[])
{
	kthread_t tids[THREAD_MAX - 2];

	((void) argc);
	((void) argv);

	nanvix_semaphore_init(&semaphore, 1);

	for (int i = 0; i < (THREAD_MAX - 2); i++)
		uassert(kthread_create(&tids[i], increment, NULL) >= 0);

	for (int i = 0; i < (THREAD_MAX - 2); i++)
		uassert(kthread_join(tids[i], NULL) >= 0);

	uprintf("Totals threads: %d", THREAD_MAX - 2);
	uprintf("Global counter: %d", global_counter);

	return (0);
}

