#*****************************
# Makefile – Carolina Ujueta
# Servidor y Cliente FIFO   
#*****************************

SERVER = servidor
CLIENT = cliente

all: $(SERVER) $(CLIENT)

$(SERVER): servidor.c
	gcc servidor.c -o servidor

$(CLIENT): cliente.c
	gcc cliente.c -o cliente

clean:
	rm -f $(SERVER) $(CLIENT)
