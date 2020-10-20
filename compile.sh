gcc -g -c event_dispatcher.c -o event_dispatcher.o
gcc -g -c main.c -o main.o
gcc -g -c gluethread/glthread.c -o gluethread/glthread.o
gcc -g main.o event_dispatcher.o gluethread/glthread.o -o exe -lpthread
