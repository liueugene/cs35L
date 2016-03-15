I modified main.c so that the work of generating the pixels is divided up equally among the threads. Each thread is tasked with
a column of pixels whose width = image width / # of threads. For example, if there were 4 threads and the picture was 101 pixels wide,
3 threads would get a 25-pixel wide column and the last thread would get 26 pixels.

One issue I had after adding multithreading was that my x and y axes were flipped, since I used a 1D array to store all my pixel
values and calculated the array position from the current px and py in the for loop. I swapped px and py, and width and height
in my element access code to fix the problem. I also had rounding errors because I was converting floats to ints in my array and
bitshifting the RGB color values into a single int. I fixed this by making a 2D array of floats instead where each color
gets its own column of values.

My implementation of SRT speeds up rendering significantly when multiple threads are used. 2 threads use nearly half the time of
a single thread, 4 threads use about a third of the time, and so on. Using even more threads gives diminishing returns because of the
increased overhead from thread creation, but there is still a noticeable speed improvement up until the point that the number of threads
equals the number of CPU cores.