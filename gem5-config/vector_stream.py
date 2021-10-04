import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *

def buildcluster(options, system, clstr):
    # Specify the path to the mobilenet accelerator descriptions
    hw_path = options.accpath + "/" + options.accbench + "/hw"
    hw_config_path = hw_path + "/configs/"
    hw_ir_path = hw_path + "/ir/"
    local_low =  0x2F000000
    local_high = 0x2FFFFFFF
    local_range = AddrRange(local_low, local_high)
    external_range = [AddrRange(0x00000000, local_low-1),
                      AddrRange(local_high+1, 0xFFFFFFFF)]
    clstr._attach_bridges(system, local_range, external_range)
    clstr._connect_caches(system, options, l2coherent=True, cache_size = "32kB")
    gic = system.realview.gic

    # Add the top function
    acc = "top"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.top = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.top, config, ir)
    clstr._connect_hwacc(clstr.top)
    clstr.top.local = clstr.local_bus.slave
    clstr.top.enable_debug_msgs = True

    # Add the Stream DMAs
    addr = 0x2fe00000
    clstr.stream_dma0 = StreamDma(pio_addr=addr, pio_size=32, gic=gic, max_pending=32)
    clstr.stream_dma0.stream_addr= local_low + 0x1000
    clstr.stream_dma0.stream_size=8
    clstr.stream_dma0.pio_delay='1ns'
    clstr.stream_dma0.rd_int = 210
    clstr.stream_dma0.wr_int = 211
    clstr._connect_dma(system, clstr.stream_dma0)

    # Add the cluster DMA
    clstr.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32, int_num=95)
    clstr._connect_cluster_dma(system, clstr.dma)

    # Create Accelerators
    for acc_name in ["S1", "S2", "S3"]:
        # Add accelerators to the cluster
        acc_bench = options.accpath + "/" + options.accbench + "/" + "hw/ir/" + acc_name + ".ll"
        # Specify the path to the config file for an accelerator
        # acc_config = <Absolute path to the config file>
        acc_config = options.accpath + "/" + options.accbench + "/" + "hw/configs/" + acc_name + ".ini"

        print(acc_config)

        setattr(clstr, acc_name, CommInterface(devicename=acc_name))
        ACC = getattr(clstr,acc_name)
        AccConfig(ACC, acc_config, acc_bench)

        # Add an SPM attribute to the cluster
        setattr(clstr, acc_name+"_spm", ScratchpadMemory())
        ACC_SPM = getattr(clstr,acc_name + "_spm")
        AccSPMConfig(ACC, ACC_SPM, acc_config)
        clstr._connect_spm(ACC_SPM)


        # Connect the accelerator to the system's interrupt controller
        ACC.gic = system.realview.gic

        # Connect HWAcc to cluster buses
        clstr._connect_hwacc(ACC)
        ACC.local = clstr.local_bus.slave
        ACC.acp = clstr.coherency_bus.slave

        # Enable display of debug messages for the accelerator
        ACC.enable_debug_msgs = True


    # Wire up streams
    # DRAM->S1
    clstr.S1.stream = clstr.stream_dma0.stream_out
    
    # S1->S2
    addr = local_low + 0x3000
    clstr.S1Out = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.S1Out.stream_size = 8
    clstr.S1.stream = clstr.S1Out.stream_in
    clstr.S2.stream = clstr.S1Out.stream_out

    # S2->S3
    addr = local_low + 0x4000
    clstr.S2Out = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.S2Out.stream_size = 8
    clstr.S2.stream = clstr.S2Out.stream_in
    clstr.S3.stream = clstr.S2Out.stream_out

    # S3->DRAM
    clstr.S3.stream = clstr.stream_dma0.stream_in

def makeHWAcc(options, system):
    system.head = AccCluster()
    buildcluster(options, system, system.head)

