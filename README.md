## Description
The code is written in benchmarks/cacheAx

The code for the host is in the /host/main.c. It contains reading from the input file and running the accelerator. 
The function defined in line 8 (START_ACC) is responsible for the dma copies and running the acclerator and moving the results from the accelerator's scratchpad.

The code for the accelerator is written in hw/cacheAx

Since the input is gmm, patching2 will be used. The rest is straigh forward.

## How to run (on 236 machine)
In the main project dir run:
    
    source req.sh
    cd benchmarks/cacheAx
    make clean; make; docker run -v /data:/data/ -v $HOME/gem5-lab2:/repo --env M5_PATH=/data/src/750-SALAM --env LAB_PATH=/repo --user $(id -u):$(id -g) -it gem5-salam:latest bash -c "cd /repo; /repo/runvector.sh -b cacheAx -p"

The results will be available after the run in /BM_ARM_OUT/cacheAx

The config for the cache can be set in /benchmarks/cacheAx/defines.h

The PROGRAM_INPUT_COUNT determines the number of times that we want to run the function and call the accelerator. The default is 100.

after the run is finished, in BM_ARM_OUT/system.terminal you can see the number of hits and misses.