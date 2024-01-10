#include <iostream>
#include <pthread.h>

// A simplified version of the Task class
// Demonstrates how to use a pthread cleanup handler
class SimpleTask
{
public:
  SimpleTask();
  void svc();
  int activate();
  int wait();

private:
  static void *thread_entry_func(void *);

  // the cleanup function
  static void thread_cleanup_func(void *);

  // the thread reference
  pthread_t thread;

  // a boolean to tell whether the task is running
  bool running;
};

SimpleTask::SimpleTask()
{
  // Initially, we are not running
  this->running = false;
}

void *
SimpleTask::thread_entry_func(void *handle)
{
  auto obj = reinterpret_cast<SimpleTask*>(handle);

  // push the cleanup function onto the stack
  pthread_cleanup_push(SimpleTask::thread_cleanup_func, handle);
  obj->svc();
  // IMPORTANT: always pair a call to pthread_cleanup_push
  // with a call to pthread_cleanup_pop.
  // A non-zero parameter, as used below, means to call the handler
  pthread_cleanup_pop(1);

  return 0;
}

// This is the function that gets pushed onto the stack 
// of thread cleanup functions.
void
SimpleTask::thread_cleanup_func(void *handle)
{
  auto obj = reinterpret_cast<SimpleTask*>(handle);

  std::cout << "In cleanup function! Setting running to false." << std::endl;
  obj->running = false; // set running to false
}

int
SimpleTask::activate()
{
  if (this->running) {
    std::cout << "Task is already running!" << std::endl;
    return 0;
  }

  std::cout << "Activating task!" << std::endl;

  // create the thread, which also begins its execution
  if (pthread_create(&thread,
		     0,
		     SimpleTask::thread_entry_func,
		     reinterpret_cast<void*>(this)))
    {
      std::cerr << "Error: could not create thread!" << std::endl;
      return -1;
    }

  this->running = true;
  return 0;
}

int
SimpleTask::wait()
{
  int status = 0;

  if ((status = pthread_join(this->thread, 0)) != 0) {
      std::cout << "Error joining thread: " << status << std::endl;
      return -1;
  }

  return 0;
}

// just an empty svc method
void
SimpleTask::svc()
{
  std::cout << "Empty svc method." << std::endl;
}


int
main(int argc, char *argv[])
{
  // Create a task, call activate and then wait

  SimpleTask task;
  if (task.activate()) {
    std::cerr << "Error: couldn't activate task." << std::endl;
    exit(1);
  }

  if (task.wait()) {
    std::cerr << "Error: couldn't wait on task." << std::endl;
    exit(1);
  }
}
