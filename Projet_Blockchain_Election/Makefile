CC=gcc
CFLAGS= -g -Wall 

all : main

main: main.o primalite.o proto_rsa.o manip_struct.o stockage_dans_les_listes.o simulation_ex4.o gagnant.o structure_du_block.o structure_arbre_arborescente.o simulation_process-vote.o
	$(CC) $(CFLAGS) -o $@ $^ -lssl -lcrypto -lm

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^ -c

primalite.o: primalite.c 
	$(CC) $(CFLAGS) -o $@ $^ -c

proto_rsa.o: proto_rsa.c 
	$(CC) $(CFLAGS) -o $@ $^ -c

manip_struct.o: manip_struct.c 
	$(CC) $(CFLAGS) -o $@ $^ -c

simulation_ex4.o: simulation_ex4.c
	$(CC) $(CFLAGS) -o $@ $^ -c

stockage_dans_les_listes.o: stockage_dans_les_listes.c 
	$(CC) $(CFLAGS) -o $@ $^ -c

gagnant.o: gagnant.c 
	$(CC) $(CFLAGS) -o $@ $^ -c

structure_du_block.o: structure_du_block.c 
	$(CC) $(CFLAGS) -o $@ $^ -c -lssl -lcrypto

structure_arbre_arborescente.o: structure_arbre_arborescente.c
	$(CC) $(CFLAGS) -o $@ $^ -c 

simulation_process-vote.o: simulation_process-vote.c
	$(CC) $(CFLAGS) -o $@ $^ -c 

valgrind: main
	valgrind --leak-check=full ./main

clean:
	rm -rf *.o


