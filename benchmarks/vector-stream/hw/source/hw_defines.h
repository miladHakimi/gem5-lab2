#include "../../../common/dma.h"

/***********************************************************
 * Computation Defines
 ***********************************************************/
#define VECTOR_SIZE		8


// StreamDMA
#define INPUT_SIZE		VECTOR_SIZE
#define OUTPUT_SIZE		VECTOR_SIZE

/***********************************************************
 * Cluster Base Address
 ***********************************************************/
#define BASE			0x2F000000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x0000
#define STREAM_DMA_MMR  0x2fe00000
#define CLUSTER_DMA_MMR 0x2ff00000
#define S1_MMR  		BASE + 0x0100
#define S2_MMR			BASE + 0x0200
#define S3_MMR			BASE + 0x0300

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn  BASE + 0x1000
#define StreamOut BASE + 0x1000

#define S1In 			StreamIn
#define S1Buffer        0x2F100000
#define S1Out			BASE + 0x3000

#define S2In 			S1Out
#define S2Out			BASE + 0x4000

#define S3In 			S2Out
#define S3Out			StreamOut