VPATH= Kernel:../libs/Sockets

gcc -I"../../Kernel" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"kernel.c" -MT"kernel.c" -o "kernel.o" "kernel.c"

 

gcc -L"../../pruebaShared/Debug" -o "kernel"  kernel.o  "/libs/estructuras_kernel.h" -lparser-ansisop -lcommons -lpthread 


