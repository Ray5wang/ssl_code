###########################################  
#Makefile for simple programs  
###########################################  
CFLAGS=-Idsplib 
CC=gcc
PRG=ssl_demo
OBJ=demo.o dsplib/fft.o dsplib/dynamicMemory.o

$(PRG):$(OBJ)
	$(CC) -o $(PRG) $(OBJ) -lm

$(OBJ):%.o:%.c 
	$(CC) -c $< -o $@ -lm
	  
clean:  
	@echo "Removing linked and compiled files......"  
	rm -f $(OBJ) $(PRG)  
						                                    
