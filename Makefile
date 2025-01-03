CC = g++

all: firehose

firehose: libipu.a
	$(CC) -fopenmp firehose_main.cpp -L./device_libraries -lipu -lpoplar -lpoplin -lpoputil -lpopops -o firehose
libipu.a: mylib1.o mylib2.o
	ar rcs ./device_libraries/libipu.a ./device_libraries/mylib1.o
	ar rcs ./device_libraries/libipu.a ./device_libraries/mylib2.o
mylib1.o:
	popc -o ./device_libraries/io_codelet.gp ./device_libraries/io_codelet.cpp
	popc -o ./device_libraries/transpose.gp ./device_libraries/transpose.cpp
	$(CC) -c -fopenmp ./device_libraries/firehose_ipu.cpp -o ./device_libraries/mylib1.o
mylib2.o:
	$(CC) -c -fopenmp ./device_libraries/ipu_support.cpp -o ./device_libraries/mylib2.o

verify:
	$(CC) verify.cpp -o verify

clean_app:
	rm firehose

clean_lib:
	rm ./device_libraries/mylib.o

clean_logs:
	rm ft_*

clean_output:
	./reset.sh

clean: clean_app clean_lib clean_logs clean_output

refresh: clean_logs clean_output

get:
	git pull

run:
	sbatch demo.batch

super: clean get all run

log:
	export POPLAR_ENGINE_OPTIONS='{"autoReport.all":"true", "autoReport.directory":"./report"}'

help:
	printf "\nrm firehose\nrm ./device_libraries/mylib.o\nrm tensor_decomp_test_*\ngit pull\ng++ -fopenmp firehose_main.cpp -L/home/jomad21/myFiles/Tensor_Decomp_Scratch/device_libraries -lipu -lpoplar -lpoplin -lpoputil -lpopops -o firehose\nar rcs ./device_libraries/libipu.a ./device_libraries/mylib.o\npopc -o ./device_libraries/io_codelet.gp ./device_libraries/io_codelet.cpp'\ng++ -c -fopenmp ./device_libraries/firehose_ipu.cpp -o ./device_libraries/mylib.o\nsbatch demo.batch\n"

