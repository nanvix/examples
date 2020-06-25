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
#include <nanvix/sys/perf.h>
#include <nanvix/ulib.h>
#include <nanvix/sys/thread.h>
#include <nanvix/sys/semaphore.h>

/**
 * @name Number of each type of threads.
 */
/**@{*/
#define NPRODUCERS ((THREAD_MAX - 1) / 2)
#define NCONSUMERS ((THREAD_MAX - 1) / 2)
/**@}*/

/**
 * @brief Size of the buffer
 */
#define SIZE 10

/**
 * @name Global variables.
 */
/**@{*/
int f, i = 0;
int buffer[SIZE];
struct nanvix_semaphore cheio;
struct nanvix_semaphore vazio;
struct nanvix_semaphore lock_prod;
struct nanvix_semaphore lock_cons;
/**@}*/

/**
 * @brief Produces 1000 int's to global buffer.
 *
 * @param args Pointer to logic thread id.
 *
 * @returns NULL value.
 **/
void *producer(void * args)
{
	int tid = *((int *) args);
	uprintf("Produtor %d criado", tid);

	for (int t = 0; t < 1000; t++)
	{
		nanvix_semaphore_down(&vazio);
		nanvix_semaphore_down(&lock_prod);

		f = (f+1) % SIZE;
		buffer[f] = (tid * 1000) + t;
		uprintf("produzi no indice %d: %d", f, buffer[f]);

		nanvix_semaphore_up(&lock_prod);
		nanvix_semaphore_up(&cheio);
	}

	return (NULL);
}

/**
 * @brief Consumes 1000 int's from global buffer.
 *
 * @param args Pointer to logic thread id.
 *
 * @retuns NULL value.
 **/
void *consumer(void * args)
{

	int tid = *((int *) args);
	uprintf("Consumidor %d criado", tid);

	for (int t = 0; t < 1000; t++)
	{
		nanvix_semaphore_down(&cheio);
		nanvix_semaphore_down(&lock_cons);

		i = (i+1) % SIZE;
		uprintf("consumi no indice %d: %d", i, buffer[i]);
		buffer[i] = 0;

		nanvix_semaphore_up(&lock_cons);
		nanvix_semaphore_up(&vazio);
	}

	return (NULL);
}

/**
 * @brief Main function. 
 **/
int __main3(int argc, const char *argv[])
{
	int n = 0;
	kthread_t tid[THREAD_MAX - 1];
	int virtual_tid[THREAD_MAX - 1];

	((void) argc);
	((void) argv);

	nanvix_semaphore_init(&cheio, 0);
	nanvix_semaphore_init(&vazio, SIZE);
	nanvix_semaphore_init(&lock_prod, 1);
	nanvix_semaphore_init(&lock_cons, 1);


	for (int k = 0; k < NCONSUMERS; ++k)
	{
		virtual_tid[n] = k;
		uassert(kthread_create(&tid[n], consumer, ((void * ) &virtual_tid[n])) == 0);
		n++;
	}

	for (int k; k < (NPRODUCERS - 1); ++k)
	{
		virtual_tid[n] = k;
		uassert(kthread_create(&tid[n], producer, ((void * ) &virtual_tid[n])) == 0);
		n++;
	}

	virtual_tid[n] = (NPRODUCERS - 1);
	producer(((void * ) &virtual_tid[n]));

	for (int k = 0; k < (NCONSUMERS + (NPRODUCERS - 1)); ++k)
		kthread_join(tid[k], NULL);

	return (0);
}

