all:
	gcc controller.c network.c -o ./builds/controller
	gcc worker.c network.c -o ./builds/worker

worker: worker.c network.c
	gcc worker.c network.c -o ./builds/worker

controller: controller.c network.c
	gcc controller.c network.c -o ./builds/controller

clean:
	rm -f ./builds/worker ./builds/controller

run1:
	./builds/controller --port 9001 --password hello123 --charset-file charset.txt --timeout-ms 5000

run2:
	./builds/worker --host 127.0.0.1 --port 9001 --id 1 --heartbeat-ms 5000
