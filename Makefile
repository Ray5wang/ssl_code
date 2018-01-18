###########################################  
#Makefile for simple programs  
###########################################  
CFLAGS=-Idsplib 
CC=gcc
PRG=ssl_demo
OBJ=main.o dsplib/fft.o dsplib/process.o  dsplib/tdoa.o dsplib/dynamicMemory.o dsplib/VAD.o dsplib/window.o audio/wave.o 

$(PRG):$(OBJ)
	$(CC) -o $(PRG) $(OBJ) -lm

$(OBJ):%.o:%.c 
	$(CC) -c $< -o $@ -lm
	  
clean:  
	@echo "Removing linked and compiled files......"  
	rm -f $(OBJ) $(PRG)  
						                                    
