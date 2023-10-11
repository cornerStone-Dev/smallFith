
.cpu cortex-m0plus
.thumb
.syntax unified

;@ Section Constants
ATOMIC_XOR         = 0x1000
ATOMIC_SET         = 0x2000
ATOMIC_CLR         = 0x3000
PBB_BASE           = 0xE0000000
SYST_CSR           = PBB_BASE + 0xE010
SYST_RVR           = 0x04
SYST_CVR           = 0x08
SYST_CALIB         = 0x0C
EXTERNAL_INTS_E      = PBB_BASE + 0xE180
EXTERNAL_INTS      = PBB_BASE + 0xE280
ICSR_REG           = PBB_BASE + 0xED04
VTOR               = PBB_BASE + 0xED08
XOSC_BASE          = 0x40024000
XOSC_CTRL_RW       = XOSC_BASE + 0x00
XOSC_CTRL_SET      = XOSC_CTRL_RW + 0x2000
XOSC_STATUS        = XOSC_BASE + 0x04
XOSC_STARTUP       = XOSC_BASE + 0x0C
XOSC_CTRL_RW_OFF       =  0x00
XOSC_STATUS_OFF        =  0x04
XOSC_STARTUP_OFF       =  0x0C
CLOCKS_BASE        = 0x40008000
CLK_REF_CTRL       = CLOCKS_BASE + 0x30
CLK_REF_SELECTED   = CLOCKS_BASE + 0x38
CLK_SYS_CTRL       = CLOCKS_BASE + 0x3C
CLK_SYS_SELECTED   = CLOCKS_BASE + 0x44
CLK_PERI_CTRL      = CLOCKS_BASE + 0x48
CLK_SYS_RESUS_CTRL = CLOCKS_BASE + 0x78
CLK_REF_CTRL_OFF       = 0x30
CLK_REF_SELECTED_OFF   = 0x38
CLK_SYS_CTRL_OFF      = 0x3C
CLK_SYS_SELECTED_OFF   = 0x44
CLK_PERI_CTRL_OFF      = 0x48
CLK_SYS_RESUS_CTRL_OFF = 0x78
PSM_BASE           = 0x40010000
PSM_WDSEL          = PSM_BASE + 0x08
SYSCFG_BASE        = 0x40004000
PROC_0_NMI         = SYSCFG_BASE + 0x00

SIO_BASE               = 0xD0000000
SIO_GPIO_OUT_CLR       = SIO_BASE + 0x18
SIO_GPIO_OUT_XOR       = SIO_BASE + 0x1C 
SIO_GPIO_OE_SET        = SIO_BASE + 0x24
SIO_GPIO_OE_CLR        = SIO_BASE + 0x28
SIO_FIFO_ST            = 0x050
SIO_FIFO_WRITE         = 0x054
SIO_FIFO_READ          = 0x058
SIO_SIGNED_DIVIDEND    = 0x068
SIO_SIGNED_DIVISOR     = 0x06C
SIO_QUOTIENT           = 0x070
SIO_REMAINDER          = 0x074
SIO_DIV_CSR            = 0x078
SIO_SPINLOCK_0         = SIO_BASE + 0x16C

PPB_BASE               = 0xE0000000
PPB_INTERRUPT_PEND     = PPB_BASE + 0xE280


IO_BANK0_BASE          = 0x40014000
IO_GPIO00_CTRL         = IO_BANK0_BASE + 0x004
IO_GPIO01_CTRL         = IO_BANK0_BASE + 0x00C
IO_GPIO12_CTRL         = IO_BANK0_BASE + 0x064
IO_GPIO13_CTRL         = IO_BANK0_BASE + 0x06C


IO_GPIO25_CTRL_RW      = IO_BANK0_BASE + 0x0CC

RESETS_RESET_CLR       = 0x4000C000 + 0x3000
RESETS_RESET_DONE_RW   = 0x4000C000 + 0x8  

WATCHDOG_BASE      = 0x40058000
WATCHDOG_CNTRL     = WATCHDOG_BASE

PLL_SYS_BASE       = 0x40028000
PLL_SYS_CTRL       = PLL_SYS_BASE + 0x00
PLL_SYS_POWER      = PLL_SYS_BASE + 0x04
PLL_SYS_FBDIV      = PLL_SYS_BASE + 0x08
PLL_SYS_PRIM       = PLL_SYS_BASE + 0x0C

UART0_BASE         = 0x40034000
UART0_DR           = 0x000
UART0_FR           = 0x018
UART0_IBRD         = 0x024
UART0_FBRD         = 0x028
UART0_LCR          = 0x02C
UART0_CR           = 0x030
UART0_IRMASK       = 0x038
UART0_IRMASKSTATUS = 0x040
UART0_IRCLEAR      = 0x044
UART0_DMA_CR       = 0x048

DMA_BASE           = 0x50000000
DMA0_READ          = 0x000
DMA0_WRITE         = 0x004
DMA0_TRAN_CNT      = 0x008
DMA0_CTRL          = 0x00C
DMA1_READ          = 0x054
DMA1_WRITE         = 0x058
DMA1_TRAN_CNT      = 0x05C
DMA1_CTRL          = 0x050

DMA_2_BASE         = 0x50000090
DMA2_CTRL          = 0x0
DMA2_READ          = 0x4
DMA2_WRITE         = 0x8
DMA2_TRAN_CNT      = 0xc
DMA3_CTRL          = 0x40
DMA3_READ          = 0x44
DMA3_WRITE         = 0x48
DMA3_TRAN_CNT      = 0x4c

FIFO_BUFF_START  = 0x20040000
FIFO_WRITER_ADDR = 0x20040100

END_OF_RAM			= 0x20042000
CORE0_FITH_PARAM_STACK	= 0x20042000
CORE0_FITH_RETURN_STACK	= 0x20041C00
CORE0_FITH_GLOBALS1		= 0x20041000
CORE0_FITH_GLOBALS2		= 0x20041100
CORE0_FITH_BLOCKS		= 0x20041200
CORE0_HEAP_TOP		= 0x20040000
CORE1_STACK_START       = 0x20040C00

SIZE_OF_RAM_BUILD = (__bss_start__-0x20000000)


CMD_READ =  0x03

// Value is number of address bits divided by 4
ADDR_L =  6
SSI_CTRLR0_SPI_FRF_VALUE_STD =  0x0
SSI_CTRLR0_SPI_FRF_LSB  =      21
SSI_SPI_CTRLR0_XIP_CMD_LSB = 24
SSI_SPI_CTRLR0_ADDR_L_LSB = 2
SSI_SPI_CTRLR0_INST_L_LSB = 8
SSI_SPI_CTRLR0_TRANS_TYPE_VALUE_1C1A = 0
SSI_SPI_CTRLR0_TRANS_TYPE_LSB = 0
SSI_CTRLR0_DFS_32_LSB = 16
SSI_CTRLR0_TMOD_VALUE_EEPROM_READ = 3
SSI_CTRLR0_TMOD_LSB = 8
CTRLR0_XIP =  (SSI_CTRLR0_SPI_FRF_VALUE_STD << SSI_CTRLR0_SPI_FRF_LSB) + (31 << SSI_CTRLR0_DFS_32_LSB) + (SSI_CTRLR0_TMOD_VALUE_EEPROM_READ  << SSI_CTRLR0_TMOD_LSB)

SPI_CTRLR0_XIP = (CMD_READ << SSI_SPI_CTRLR0_XIP_CMD_LSB) + (ADDR_L << SSI_SPI_CTRLR0_ADDR_L_LSB) + (2 << SSI_SPI_CTRLR0_INST_L_LSB) + (SSI_SPI_CTRLR0_TRANS_TYPE_VALUE_1C1A << SSI_SPI_CTRLR0_TRANS_TYPE_LSB)


;@ Section Register Renaming
TOS .req r0
W   .req r1
CTX .req r4
RSP .req r5
BP  .req r6
TP  .req r7
G1  .req r6
G2  .req r7
PSP .req sp

;@ Section Macros
;@ Indirect Threading
.macro NEXT
	ldm  TP!, {W}
	ldm  W!, {r2}
	bx   r2
.endm

.macro POP_TOS
	pop	{TOS}
.endm

.macro PUSH_TOS
	push	{TOS}
.endm

.macro POP_W
	pop	{W}
.endm

.macro PUSH_W
	push	{W}
.endm

.macro POP_RSP src
	subs	RSP, 4
	ldr	\src, [RSP]
.endm

.macro PUSH_RSP src
	stm	RSP!, {\src}
.endm

.set link    , 0
.set tmpLink    , 0
.set F_IMMED , 0x80
.set F_HIDDEN, 0x40

WORD_FUNCTION =  0
WORD_FUNCTION_COMP =  1
WORD_GLOBAL = 2
WORD_CONSTANT = 3
WORD_LOCAL = 4
WORD_DIRECT = 5
WORD_COND_BEQ = 6
WORD_COND_BNE = 7
WORD_COND_BGE = 8
WORD_COND_BLT = 9
WORD_COND_BGT = 10
WORD_COND_BLE = 11
WORD_ELSE	= 12
WORD_RETURN	= 13
WORD_WHILE	= 14
WORD_CASE	= 15
WORD_AGAIN	= 16
WORD_LEAVE	= 17
WORD_REPEAT	= 18
WORD_CALL	= 19
WORD_COMPILE  = 20

@@ Word header macro

.macro define_builtIn_word name, type
	.balign 2
	.set link, 20b - 20f
20:	
	.hword link
	.set link, 20b
	.byte 0
	.byte 0
	.byte \type
	.byte 22f - 21f - 1
21:	.asciz "\name"
22:	.balign 2, 0
.endm

;@ Section Vector Table
.global vector_table
vector_table:
	b reset
	.balign 4
	.word reset ;@ has to be offset 4
	.word REBOOT ;@purgatory  ;@ 2 NMI
	.word whoisme ;@purgatory  ;@ 3 HardFault

	.word SIZE_OF_RAM_BUILD ;@ 4 Reserved
	.word flashEntry   ;@ 5 Reserved
	.word 0  ;@ 6 Reserved
	.word REBOOT  ;@ 7 Reserved
	
	.word REBOOT  ;@ 8 Reserved
	.word REBOOT  ;@ 9 Reserved
	.word REBOOT  ;@ 10 Reserved
	.word REBOOT  ;@ 11 SVCall
	
	.word REBOOT  ;@ 12 Reserved
	.word REBOOT  ;@ 13 Reserved
	.word REBOOT  ;@ 14 PendSV isrPendSvCall
	.word REBOOT  ;@ 15 SysTick
	
	.word alarm1ISR   ;@ 16 external interrupt 0
	.word alarm2ISR
	.word alarm3ISR
	.word alarm4ISR
	
	.word REBOOT   ;@ 4
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 8
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 12
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 16
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word isr_uart0_fith   ;@ 20
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 24
	.word REBOOT
	.word minorFrame;@runTasksP0	;@ 26 first software defined interrupt
	.word REBOOT;@runTasksP1	;@ 27
	
	.word Background;@runTasksP2   ;@ 28
	.word REBOOT
	.word REBOOT
	.word REBOOT

.balign 2
.code 16
.thumb_func
.global setZeroWait
.type setZeroWait, %function
setZeroWait:
	ldr  r2,=DMA_2_BASE ;@ base reg
1:
	ldr  r3,[r2, #DMA2_CTRL]
	lsls r3, 7
	bmi  1b
	bx lr

.balign 4
.global boot2Entry
;@~ .thumb_func
;@~ .type boot2Entry, %function
boot2Entry:
	bl   clocksSetup ;@ set up clocks
	bl   xipSetup ;@ set up xip to do flash reads
	;@ r0 = 0, r1 = 1 rest is garbage
	
	lsls r6,r1,#29 ;@ RAM addr
	ldr  r5, =0x10000000 + 4096;@ flash addr
	
	ldr  r1, [r5, #16] ;@ size of code
	;@ r0=0, r1=size of code;r5=flash base;r6=RAM base; 
1:
	ldr  r4, [r5, r0] ;@ load from flash
	str  r4, [r6, r0] ;@ store to ram
	adds r0, 4
	cmp  r0, r1
	bne  1b
	ldr  r4, [r6, #20] ;@ load flash entry vector
	blx  r4

.global clocksSetup
.thumb_func
.type clocksSetup, %function
clocksSetup:
    ldr r2,=XOSC_BASE
	movs r1, 47
	str r1,[r2, #XOSC_STARTUP_OFF] ;@ set timeout, from manual
	ldr r1,=0xFABAA0
	str r1,[r2, #XOSC_CTRL_RW_OFF] ;@ set enable and speed range

1:
	ldr r1,[r2, #XOSC_STATUS_OFF] ;@ read status to see if it is ready
	cmp r1, #0
	bge 1b

	ldr  r2,=CLOCKS_BASE
	movs r1, 0
	str  r1,[r2, #CLK_SYS_RESUS_CTRL_OFF] ;@ disable resus mis-feature
	movs r1, 2
	str  r1,[r2, #CLK_REF_CTRL_OFF] ;@ move reference clock to crystal
	movs r3, 4
1:
	ldr r1,[r2, #CLK_REF_SELECTED_OFF] ;@ read status to see if it is ready
	tst r1, r3
	beq 1b
	
	;@ bring up PLL
	movs r3, 1
	lsls r3,r3,#12
	ldr r0, =RESETS_RESET_CLR
	str r3, [r0]
	ldr r0, =RESETS_RESET_DONE_RW ;@ read status to see if it is ready
1:
	ldr r1,[r0]
	tst r1, r3
	beq 1b
	
	ldr  r2,=PLL_SYS_BASE ;@ base reg 
	movs r1, 80
	str  r1,[r2, #PLL_SYS_FBDIV - PLL_SYS_BASE] ;@ set up multiplier
	
	ldr r0,=PLL_SYS_POWER + ATOMIC_CLR ;@ turn on power
	movs r1, 0x21
	str r1,[r0]
	
	movs r3, 1
	lsls r3,r3,#31
1:
	ldr r1,[r2] ;@ wait for resonance cascade
	tst r1, r3
	beq 1b
	
	;@~ ldr r1,=(((4<<16)|(1<<12))>>12)
	movs r1, 0x42
	lsls r1, 12
	str r1,[r2, #PLL_SYS_PRIM - PLL_SYS_BASE] ;@ set post dividers
	
	;@ turn on post dividers
	movs r1, 0x08
	str r1,[r0]
	;@ PLL_SYS is now at 120 mhz, pico max is 133, this is 90% of that
	;@ so nice margin of safety
	
	ldr r2,=CLOCKS_BASE
	movs r1, 1
	str r1,[r2, #CLK_SYS_CTRL_OFF] ;@ set sys clock to PLL_SYS
	movs r3,2
1:
	ldr r1,[r2, #CLK_REF_SELECTED_OFF] ;@ read status to see if it is ready
	tst r1, r3
	bne 1b
	
	movs r1, 0x88
	lsls r1, 4
	str  r1,[r2, #CLK_PERI_CTRL_OFF] ;@ set peripheral clock to the ref clock and enable
	;@ we are done
	bx   lr

.global xipSetup
.thumb_func
.type xipSetup, %function
xipSetup: ;@ r0-r3 are garbage
	;@ try to set up xip
	movs  r3, 0x18                // Use as base address where possible
	lsls r3, 24

    // Disable SSI to allow further config
    movs r0, #0
    str  r0, [r3, #0x08]
    
    // NDF=0 (single 32b read)
    str  r0, [r3, #0x04]

    // Set baud rate
    movs r1, #4
    str  r1, [r3, #0x14]

    ldr  r1, =(CTRLR0_XIP)
    str  r1, [r3, #0x00]

    ldr  r1, =(SPI_CTRLR0_XIP)
    adds r3, #244
    str  r1, [r3]

    // Re-enable SSI
    subs r3, #244
    movs r1, #1
    str  r1, [r3, #0x08]
	
	bx   lr

.global dmaSetup
.thumb_func
dmaSetup:
;@ get DMA out of reset
	movs r3, 4 
	ldr r0, =RESETS_RESET_CLR
	str r3, [r0]
	ldr r0, =RESETS_RESET_DONE_RW ;@ read status to see if it is ready
1:
	ldr r1,[r0]
	tst r1, r3
	beq 1b
	
	ldr  r2,=DMA_2_BASE ;@ base reg
	ldr  r1, =(0x3F<<15)|(1<<21)|(2<<11)|(1<<5)|(2<<2)|(1<<0)
	str  r1,[r2, #DMA2_CTRL]
	adr  r1, ZERO_CONSTANT_LOCATION
	str  r1,[r2, #DMA2_READ] ;@ set as DMA read
	
	bx   lr

.balign 2
.code 16
.thumb_func
.global setZero
.type setZero, %function
setZero: ;@ r0 = dst r1 = size
	ldr  r2,=DMA_2_BASE ;@ base reg
	str  r0,[r2, #DMA2_WRITE]
	lsrs r1, 2
	str  r1,[r2, #DMA2_TRAN_CNT]
	bx lr

.balign 4
ZERO_CONSTANT_LOCATION:
.word 0
.balign 4
.ltorg

.global reset
.balign 2
.code 16
.thumb_func
.type reset, %function
reset:
	;@ config clocks
	bl clocksSetup
.global flashEntry
.balign 2
.code 16
.thumb_func
.type flashEntry, %function
flashEntry:
	;@ set up DMA to clear RAM
	bl   dmaSetup
	ldr  r0, = __bss_start__
	ldr  r1, = __bss_end__  ;@END_OF_RAM
	subs r1, r0
	bl   setZero
	;@ configure vector table
	ldr  r1, =VTOR
	ldr  r0, =vector_table
	str  r0,[r1]
	;@ set stack
	ldr  r0,=END_OF_RAM  ;@INITIAL_STACK
	msr  MSP, r0
	;@ take devices out of reset
	movs r2, 1
	bl resetIOBank
	bl configUART
	
	bl	setZeroWait ;@ until there is enough time to rip through both stacks
	bl	printHelloBanner
	;@~ bl   memSysInit
	;@~ bl   helper_unlock
	bl	picoInit
	ldr	r1, =s
	bl	smallFithInit
	;@ set up registers
.global resetAllRegs
resetAllRegs:
	ldr	r1,  =END_OF_RAM  ;@INITIAL_STACK
	mov	sp,   r1
	ldr	CTX, =s
	ldr	RSP, =CORE0_FITH_RETURN_STACK
	ldr	G1,  =CORE0_FITH_GLOBALS1
	ldr	G2,  =CORE0_FITH_GLOBALS2
	
	
	;@~ bl	fithInterpret
	b	check_uart0_fith
;@~ 1:  wfe
	
	;@~ b    1b
	
	;@~ b REBOOT

.balign 2
.code 16
.thumb_func
.global REBOOT
.type REBOOT, %function
REBOOT:
	;@~ ldr r0,=PSM_WDSEL ;@ ENABLE MASS RESET
	;@~ ldr r1,=0x1FFFC
	;@~ str r1,[r0]
	ldr  r0,=WATCHDOG_CNTRL ;@ reboot
	movs r1, 1
	lsls r1, 31
	str r1,[r0]
	b purgatory

.balign 2
.code 16
.thumb_func
.type whoisme, %function
whoisme:
	;@~ mrs r0, IPSR
	push	{r4,r5,r6,r7,lr}
	mov	r0, sp
	bl	printStackStrace
	pop	{r4,r5,r6,r7,pc}

.code 16
.thumb_func
.type purgatory, %function
purgatory:
	b purgatory

.balign 2
.code 16
.thumb_func
.type resetIOBank, %function
resetIOBank:
	lsls r3,r2,#5
	lsls r1,r2,#10
	adds r3, r1
	lsls r1,r2,#11
	adds r3, r1
	lsls r1,r2,#21
	adds r3, r1
	ldr  r0, =RESETS_RESET_CLR
	str  r3, [r0]
	ldr  r0, =RESETS_RESET_DONE_RW
	1:
	ldr  r1,[r0]
	tst  r1, r3
	beq  1b
	lsls r3,r2,25
	ldr  r0, =SIO_GPIO_OE_CLR
	str  r3, [r0]
	ldr  r0, =SIO_GPIO_OUT_CLR
	str  r3, [r0]
	ldr  r0, =SIO_GPIO_OE_SET
	str  r3, [r0]
	
	movs r1, 5  ;@ GPIO control
	;@~ movs r1, 7 ;@ PIO 1 control
	ldr  r0, =IO_GPIO25_CTRL_RW
	str  r1, [r0]
	bx   lr
	
.balign 2
.code 16
.thumb_func
.type configUART, %function
configUART:
	;@ Section UART 
	;@bring up UART
	lsls r3,r2,#22
	ldr r0, =RESETS_RESET_CLR
	str r3, [r0]
	ldr r0, =RESETS_RESET_DONE_RW ;@ read status to see if it is ready
1:
	ldr r1,[r0]
	tst r1, r3
	beq 1b
	
	ldr r4,=UART0_BASE ;@ base reg
	ldr r1,=6
	str r1,[r4, #UART0_IBRD] ;@ set up baud
	
	ldr r1,=33
	str r1,[r4, #UART0_FBRD] ;@ set up fractional baud
	
	ldr r1,=0x70
	str r1,[r4, #UART0_LCR] ;@ set up fractional baud
	
	ldr r1,=(1<<9)|(1<<8)|(1<<0)
	str r1,[r4, #UART0_CR] ;@ enable UART, TX and RX
	
	;@~ ldr r1,=(1<<1)
	;@~ str r1,[r4, #UART0_DMA_CR] ;@ enable DMA on RX and tx (1<<0)
	
	ldr r1,=(1<<6)|(1<<4) ;@ |(1<<5)
	str r1,[r4, #UART0_IRMASK]
	
	;@~ ldr r0,=IO_GPIO00_CTRL ;@ set gpio 0 to UART 0 TX
	;@~ ldr r1,=2
	;@~ str r1,[r0]
	
	;@~ ldr r0,=IO_GPIO01_CTRL ;@set gpio 1 to UART 0 RX
	;@~ ldr r1,=2
	;@~ str r1,[r0]
	
	ldr r0,=IO_GPIO12_CTRL ;@ set gpio 0 to UART 0 TX
	ldr r1,=2
	str r1,[r0]
	
	ldr r0,=IO_GPIO13_CTRL ;@set gpio 1 to UART 0 RX
	ldr r1,=2
	str r1,[r0]
	
	ldr r4,=DMA_BASE ;@ base reg
	ldr r1, =(0x3F<<15)|(1<<21)|(1<<11)|(1<<5)|(1<<4)|(2<<2)|(1<<0)
	str r1,[r4, #DMA1_CTRL]
	
	bx lr

.balign 2
.code 16
.thumb_func
.global dmaWordForwardCopy
.type dmaWordForwardCopy, %function
dmaWordForwardCopy: ;@ r0 = src r1 = dst r2 = size
	push {r4,lr}
	ldr  r4,=DMA_BASE ;@ base reg
1:
	ldr  r3,[r4, #DMA1_CTRL]
	lsls r3, 7
	bmi  1b
	str  r0,[r4, #DMA1_READ]
	str  r1,[r4, #DMA1_WRITE]
	lsrs r2, 2
	str  r2,[r4, #DMA1_TRAN_CNT]
	pop  {r4,pc}

.balign 2
.code 16
.thumb_func
.global readSysTimerVal
.type readSysTimerVal, %function
readSysTimerVal:
	ldr  r1, =SYST_CSR
	ldr  r1, [r1, #SYST_CVR]
	subs r0, r1, r0
	bx lr

.balign 2
.code 16
.thumb_func
.global startSysTimer
.type startSysTimer, %function
startSysTimer:
	ldr  r1, =SYST_CSR
	ldr  r0, =0xFFFFFF
	str  r0, [r1, #SYST_RVR]
	movs r0, 0
	str  r0, [r1, #SYST_CVR]
	movs r0, 5
	str  r0, [r1, #0]
	bx lr

.balign 2
.code 16
.thumb_func
.global endSysTimer
.type endSysTimer, %function
endSysTimer:
	ldr  r1, =SYST_CSR
	ldr  r2, [r1, #SYST_CVR]
	ldr  r0, =0xFFFFFF
	subs r0, r0, r2
	movs r2, 0
	str  r2, [r1, #0]
	bx lr

.thumb_func
.global uart0_txByte
uart0_txByte: ;@ r0 = data to print
	ldr r1,=UART0_BASE ;@ get address of UART
1:	ldr r2,[r1, #UART0_FR]
	lsls r2, 26 
	bmi 1b
	strb r0,[r1] ;@ write data out the serial port
	bx lr

.thumb_func
.global isr_uart0_fith
isr_uart0_fith:
	ldr	r1, =UART0_BASE ;@ get base address of UART
	ldr	r2, [r1, #UART0_IRMASKSTATUS]
	movs	r3, 0xFF
	str	r3, [r1, #UART0_IRCLEAR] ;@ clear interrupts
	movs	r3, 0x50
	tst	r2, r3
	beq	3f ;@ the interrupt was not on a recieve
	ldr	r2, [r1, #UART0_IRMASK]
	bics	r2, r3
	str	r2, [r1, #UART0_IRMASK] ;@ mask interrupts
3:	bx	lr

.thumb_func
.global check_uart0_fith
check_uart0_fith:
	wfe
1:	;@ continue processing if there are more characters
	ldr	r1, =UART0_BASE
	ldr	r2, [r1, #UART0_FR]
	movs	r3, 16
	tst	r2, r3
	bne	2f
	;@ call character processing function
	push	{TOS}
	movs	TOS, CTX
	ldr	r1, [r1, #UART0_DR]
	;@~ push	{r0,r1}
	;@~ bl	startSysTimer
	;@~ pop	{r0,r1}
	bl	combineToken
	;@~ push	{r0,r1,r2,r3}
	;@~ bl	endSysTimer
	;@~ bl	io_printin
	;@~ pop	{r0,r1,r2,r3}
	movs	W, TOS
	pop	{TOS}
	;@ check if the return value means we need to do something
	cmp	W, #0
	beq	1b
	blx	W
	b	1b
2:	movs	r3, 0x50
	ldr	r2, [r1, #UART0_IRMASK]
	orrs	r2, r3
	str	r2, [r1, #UART0_IRMASK] ;@ un-mask interrupts

	b	check_uart0_fith

.thumb_func
.global asmDivSM
asmDivSM:
	pop		{W}
.balign 2
.code 16
.thumb_func
.global asmDiv
.type asmDiv, %function
asmDiv: ;@ r0 = DIVISOR r1 = DIVIDEND
	ldr  r2, = SIO_BASE
	str  r1, [r2, #SIO_SIGNED_DIVIDEND]
	str  r0, [r2, #SIO_SIGNED_DIVISOR] ;@ now takes 8 cycles to finish
	b    1f
1:  b    1f
1:  b    1f
1:  b    1f
1:
	ldr  r0, [r2, #SIO_QUOTIENT]
	bx   lr

.thumb_func
.global asmModSM
asmModSM:
	pop		{W}
.balign 2
.code 16
.thumb_func
.global asmMod
.type asmMod, %function
asmMod: ;@ r0 = DIVISOR r1 = DIVIDEND
	ldr  r2, = SIO_BASE
	str  r1, [r2, #SIO_SIGNED_DIVIDEND]
	str  r0, [r2, #SIO_SIGNED_DIVISOR] ;@ now takes 8 cycles to finish
	b    1f
1:  b    1f
1:  b    1f
1:  b    1f
1:
	ldr  r0, [r2, #SIO_REMAINDER]
	ldr  r1, [r2, #SIO_QUOTIENT]
	bx   lr

.balign 2
.code 16
.thumb_func
.global asmGetDiv
.type asmGetDiv, %function
asmGetDiv: ;@ call right after asmMod
	movs r0, r1
	bx   lr

.balign 4
.ltorg

.balign 2
.code 16
.thumb_func
.global stringLen
.type stringLen, %function
stringLen: ;@ r0 has pointer to start of string, check 4 byte alignment
	movs r1, r0
	movs r0, 0
	movs r2, 3
	tst  r2, r1
	bne  stringLenByte
	movs r2, 0xFF
5:
	ldm  r1!, {r3}
	tst  r3, r2
	beq  1f
	lsrs r3, 8
	tst  r3, r2
	beq  2f
	lsrs r3, 8
	tst  r3, r2
	beq  3f
	lsrs r3, 8
	tst  r3, r2
	beq  4f
	adds r0, 4
	b    5b
4:
	adds r0, 3
	bx   lr
3:
	adds r0, 1
2:
	adds r0, 1
1:
	bx   lr

1:
	adds r0, 1
stringLenByte:
	ldrb r2, [r1, r0]
	cmp  r2, 0
	bne  1b
	bx   lr

;@ ARM implementations for the compiler
.global __gnu_thumb1_case_uqi
.thumb_func
__gnu_thumb1_case_uqi:
	mov     r12, r1
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r1, r1, #1
	ldrb    r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	mov     r1, r12
	bx      lr

.global __gnu_thumb1_case_sqi
.thumb_func
__gnu_thumb1_case_sqi:
	mov     r12, r1
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r1, r1, #1
	ldrsb   r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	mov     r1, r12
	bx      lr

.global __gnu_thumb1_case_uhi
.thumb_func
__gnu_thumb1_case_uhi:
	push    {r0, r1}
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r0, r0, #1
	lsls    r1, r1, #1
	ldrh    r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	pop     {r0, r1}
	bx      lr

.global __gnu_thumb1_case_shi
.thumb_func
__gnu_thumb1_case_shi:
	push    {r0, r1}
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r0, r0, #1
	lsls    r1, r1, #1
	ldrsh   r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	pop     {r0, r1}
	bx      lr

.global __gnu_thumb1_case_si
.thumb_func
__gnu_thumb1_case_si:
	push	{r0, r1}
	mov	r1, lr
	adds.n	r1, r1, #2
	lsrs	r1, r1, #2
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	ldr	r0, [r1, r0]
	adds	r0, r0, r1
	mov	lr, r0
	pop	{r0, r1}
	mov	pc, lr

	
.balign 4
.ltorg


20:
define_builtIn_word "drop", WORD_COMPILE + 1
	POP_TOS

define_builtIn_word "nip", WORD_COMPILE + 1
	POP_W

define_builtIn_word "over", WORD_COMPILE + 2
	PUSH_TOS
	ldr	TOS, [sp, #4]

define_builtIn_word "pick", WORD_COMPILE + 2
	PUSH_TOS
	ldr	TOS, [sp, #8]

define_builtIn_word "*", WORD_COMPILE + 2
	POP_W
	muls	TOS, W

define_builtIn_word "/", WORD_DIRECT
	.balign 4
	.word asmDivSM

define_builtIn_word "%", WORD_DIRECT
	.balign 4
	.word asmModSM

define_builtIn_word "|", WORD_COMPILE + 2
	POP_W
	orrs	TOS, W

define_builtIn_word "&", WORD_COMPILE + 2
	POP_W
	ands	TOS, W

define_builtIn_word "^", WORD_COMPILE + 2
	POP_W
	eors	TOS, W

define_builtIn_word "&~", WORD_COMPILE + 3
	movs	W, TOS
	POP_TOS
	bics	TOS, W

define_builtIn_word "~", WORD_COMPILE + 1
	mvns	TOS, TOS

define_builtIn_word "neg", WORD_COMPILE + 1
	negs	TOS, TOS

define_builtIn_word "not", WORD_COMPILE + 2
	negs	W, TOS
	adcs	TOS, W

define_builtIn_word "abs", WORD_COMPILE + 3
	asrs	W, TOS, 31
	adds	TOS, TOS, W
	eors	TOS, W

define_builtIn_word "or", WORD_COMPILE + 2
	POP_W
	orrs	TOS, W

define_builtIn_word "and", WORD_FUNCTION
	POP_W
	rsbs	TOS, TOS, 0
	sbcs	TOS, TOS
	ands	TOS, W
	bx	lr

define_builtIn_word "LED", WORD_FUNCTION
	push	{TOS,lr}
	bl	io_ledToggle
	pop	{TOS,pc}

define_builtIn_word "alloc", WORD_DIRECT
	.balign 4
	.word	zalloc

define_builtIn_word "free", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	free
	pop	{TOS,pc}

define_builtIn_word ".", WORD_FUNCTION
	push	{TOS,lr}
	add   r1, sp, #8
	add   r2, sp, #484
	lsrs  r2, 9
	lsls  r2, 9
	bl    printFithStackElements
	pop	{TOS,pc}

define_builtIn_word "swap", WORD_COMPILE + 3
	POP_W
	PUSH_TOS
	mov  TOS, W

define_builtIn_word "dup", WORD_COMPILE + 1
	PUSH_TOS

define_builtIn_word "c", WORD_FUNCTION
	add	r2, sp, 496
	lsrs	r2, 9
	lsls	r2, 9
	mov	sp, r2
	bx	lr

define_builtIn_word ">={",	WORD_COND_BLT
define_builtIn_word ">{",	WORD_COND_BLE
define_builtIn_word "<={",	WORD_COND_BGT
define_builtIn_word "<{",	WORD_COND_BGE
define_builtIn_word "!{",	WORD_COND_BEQ
define_builtIn_word "={",	WORD_COND_BNE
define_builtIn_word "}else{",	WORD_ELSE
define_builtIn_word "}{",		WORD_ELSE
define_builtIn_word "return",	WORD_RETURN
define_builtIn_word "while",	WORD_WHILE
define_builtIn_word "case{",	WORD_CASE
define_builtIn_word "again",	WORD_AGAIN
define_builtIn_word "leave",	WORD_LEAVE
define_builtIn_word "repeat{",	WORD_REPEAT
define_builtIn_word "call",		WORD_CALL

define_builtIn_word "phn", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	io_printhn
	pop	{TOS,pc}

define_builtIn_word "ph", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	io_printh
	pop	{TOS,pc}

define_builtIn_word "pi", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	io_printi
	pop	{TOS,pc}

define_builtIn_word "pin", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	io_printin
	pop	{TOS,pc}

define_builtIn_word "ps", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	io_prints
	pop	{TOS,pc}

define_builtIn_word "psn", WORD_FUNCTION
	pop	{W}
	push	{W,lr}
	bl	io_printsn
	pop	{TOS,pc}



.thumb_func
.global smallFithInit
smallFithInit:
	push	{lr}
	ldr	r0, =link
	ldr	r2,   =CORE0_HEAP_TOP
	bl	smallFithInitP2
	pop	{pc}
	
