
CC=gcc

all: main.o local_config.o fetch_from_url.o extract_server_list.o connect_cmd.o
	$(CC) $(CCFLAGS) $^ -lcurl -lyaml -o shadow_cli

main.o: main.c
	$(CC) $(CCFLAGS) -c $^ -o $@

local_config.o: local_config.c
	$(CC) $(CCFLAGS) -c $^ -o $@

fetch_from_url.o: fetch_from_url.c
	$(CC) $(CCFLAGS) -c $^ -o $@

extract_server_list.o: extract_server_list.c
	$(CC) $(CCFLAGS) -c $^ -o $@

connect_cmd.o: connect_cmd.c
	$(CC) $(CCFLAGS) -c $^ -o $@

clean:
	rm -rf *.o
