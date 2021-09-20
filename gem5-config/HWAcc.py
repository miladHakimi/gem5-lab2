import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *


def makeHWAcc(options, system):
    # Specify the path to the benchmark file for an accelerator
    # acc_bench = <Absolute path to benchmark LLVM file>
    acc_bench = options.accpath + "/" + options.accbench + "/hw/" + options.accbench + ".ll"

    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config = options.accpath + "/" + options.accbench + "/hw/config.ini"

    ############################# Creating the Accelerator Cluster #################################
    # Create a new Accelerator Cluster
    system.acctest  = AccCluster()
    local_low       = 0x2F000000
    local_high      = 0x2FFFFFFF
    local_range     = AddrRange(local_low, local_high)
    external_range  = [AddrRange(0x00000000, local_low-1),
                       AddrRange(local_high+1, 0xFFFFFFFF)]
    system.acctest._attach_bridges(system, local_range, external_range)
 #   system.acctest._connect_caches(system, options, l2coherent=True)
    system.acctest._connect_caches(system, options, l2coherent=True, cache_size = "32kB")

    ############################# Adding Accelerators to Cluster ##################################
    # Add an accelerator attribute to the cluster
    setattr(system.acctest, options.accbench, CommInterface(devicename=options.accbench))
    ACC = getattr(system.acctest,options.accbench)
    AccConfig(ACC, acc_config, acc_bench)

    # Add an SPM attribute to the cluster
    setattr(system.acctest, options.accbench+"_spm", ScratchpadMemory())
    ACC_SPM = getattr(system.acctest,options.accbench + "_spm")
    AccSPMConfig(ACC, ACC_SPM, acc_config)
    system.acctest._connect_spm(ACC_SPM)

    # Connect the accelerator to the system's interrupt controller
    ACC.gic = system.realview.gic

    # Connect HWAcc to cluster buses
    system.acctest._connect_hwacc(ACC)
    ACC.local = system.acctest.local_bus.slave
    ACC.acp = system.acctest.coherency_bus.slave

    # Enable display of debug messages for the accelerator
    ACC.enable_debug_msgs = True

    ################################## Adding DMAs to Cluster #####################################
    # Add DMA devices to the cluster and connect them
    system.acctest.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32, int_num=95)
    system.acctest._connect_cluster_dma(system, system.acctest.dma)
    # system.acctest.dma.dma = system.membus.slave
    # system.acctest.dma.pio = system.acctest.local_bus.master

    system.acctest.stream_dma_0 = StreamDma(pio_addr=0x2ff10000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acctest.stream_dma_0.stream_in = ACC.stream
    system.acctest.stream_dma_0.stream_out = ACC.stream
    system.acctest.stream_dma_0.stream_addr=0x2ff10020
    system.acctest.stream_dma_0.stream_size=8
    system.acctest.stream_dma_0.pio_delay = '1ns'
    system.acctest.stream_dma_0.rd_int = 210
    system.acctest.stream_dma_0.wr_int = 211
    system.acctest._connect_dma(system, system.acctest.stream_dma_0)

    system.acctest.stream_dma_1 = StreamDma(pio_addr=0x2ff20000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acctest.stream_dma_1.stream_in = ACC.stream
    system.acctest.stream_dma_1.stream_out = ACC.stream
    system.acctest.stream_dma_1.stream_addr=0x2ff20020
    system.acctest.stream_dma_1.stream_size=8
    system.acctest.stream_dma_1.pio_delay = '1ns'
    system.acctest.stream_dma_1.rd_int = 212
    system.acctest.stream_dma_1.wr_int = 213
    system.acctest._connect_dma(system, system.acctest.stream_dma_1)

