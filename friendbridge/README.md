How does friendbridge work?
---------------------------

1. Shared Memory (SHM) Setup:

friendbridge and Friend Core agree on a unique key (e.g., SHM_KEY) and size 
(e.g., SHM_SIZE) for the Shared Memory segment.
friendbridge uses shmget to create or access the Shared Memory segment using the
agreed key and size. The program then attaches to the Shared Memory segment 
using shmat, providing a pointer (shm_ptr) to the shared memory region.

The SHM_KEY is stored in cfg/cfg.ini and will be read by both Friend Core as 
well as friendbridge.

2. X11 Initialization:

friendbridge initializes an X11 display using XOpenDisplay(NULL) to interact 
with the X server. It selects the types of X11 events it wants to monitor using 
XSelectInput. In this case, SubstructureNotifyMask is used to receive 
notifications for window creation and manipulation.

3. Event Loop:

friendbridge enters a loop using XNextEvent to continuously listen for X11 
events. When an event occurs, friendbridge checks the event type. For example, 
if it's a CreateNotify event, it implies a new window has been created.

4. Sending Messages:

Upon detecting a relevant X11 event, such as the creation of a new window, 
friendbridge writes a message to the shared memory region (shm_ptr). For 
example, it might write "Window Created!" to the shared memory.

5. Detaching and Closing:

After completing its tasks, friendbridge detaches from the shared memory segment
using shmdt(shm_ptr). It closes the X11 display using XCloseDisplay(display).

6. Friend Core Integration:

Friend Core has its own mechanism to read and handle messages from the shared
memory segment. It monitors the shared memory region for incoming messages, 
interpreting and responding to them accordingly.

7. Synchronization:

To avoid race conditions, friendbridge and Friend Core might use synchronization
mechanisms like mutexes or semaphores if they access the shared memory 
concurrently.

8. Building and Running:

friendbridge is compiled and executed. It continuously monitors X11 events and 
communicates with Friend Core by writing messages to the shared memory region.

