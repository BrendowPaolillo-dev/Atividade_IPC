CC=gcc -Wall

PROGS=chat_cliente chatservidor

all: $(PROGS)

clean:
	rm -f $(PROGS)

chat_cliente:	Quemeu_sou_cliente.c
	$(CC) Quemeu_sou_cliente.c -o Quemeu_sou_cliente

chatservidor:	Quemeu_sou_servidor.c
	$(CC) Quemeu_sou_servidor.c -o Quemeu_sou_servidor
