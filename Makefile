all:
	gcc controller.c network.c -o ./builds/controller
	gcc worker.c network.c -o ./builds/worker

worker: worker.c network.c
	gcc worker.c network.c -o ./builds/worker

controller: controller.c network.c
	gcc controller.c network.c -o ./builds/controller

clean:
	rm -f ./builds/worker ./builds/controller
