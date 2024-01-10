This example shows the system calls invoked when we run a simple program like

```
#include <iostream>
#include <unistd.h>
int main() {


    int pid=fork();
    int mypid=getpid();
    std::cout << "mypid is " <<  mypid<<std::endl;
    return 0;
}

```

Follow the instructions in the tests folder.



