all:
	gcc ./Slaves/slave1.c -o slave1 -lm
	gcc ./Slaves/slave2.c -o slave2 -lm
	gcc ./Slaves/slave3.c -o slave3 -lm
	gcc ./Slaves/slave4.c -o slave4 -lm
	gcc ./Slaves/slave5.c -o slave5 -lm
	gcc ./Slaves/slave6.c -o slave6 -lm
	gcc ./Slaves/slave7.c -o slave7 -lm
	gcc ./Slaves/slave8.c -o slave8 -lm
	gcc ./Slaves/slave9.c -o slave9 -lm
	gcc ./Slaves/slave10.c -o slave10 -lm
	gcc ./Master1/master.c -o master

run:
	./slave1 &
	./slave2 &
	./slave3 &
	./slave4 &
	./slave5 &
	./slave6 &
	./slave7 &
	./slave8 &
	./slave9 &
	./slave10 &
	./master