	.cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	2
	.global	initSprite
	.arch armv4t
	.syntax unified
	.arm
	.fpu softvfp
	.type	initSprite, %function
initSprite:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #0
	mov	r1, #16
	mov	ip, #1
	ldr	r3, .L3
	ldr	r0, .L3+4
	str	r1, [r3]
	str	r1, [r3, #4]
	str	ip, [r3, #40]
	str	r0, [r3, #44]
	str	r2, [r3, #8]
	str	r2, [r3, #12]
	str	r2, [r3, #16]
	str	r2, [r3, #20]
	str	r2, [r3, #24]
	str	r2, [r3, #28]
	str	r2, [r3, #32]
	str	r2, [r3, #36]
	str	r2, [r3, #48]
	str	r2, [r3, #52]
	bx	lr
.L4:
	.align	2
.L3:
	.word	sprite
	.word	shadowOAM
	.size	initSprite, .-initSprite
	.align	2
	.global	initMatrices
	.syntax unified
	.arm
	.fpu softvfp
	.type	initMatrices, %function
initMatrices:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #0
	mov	ip, #256
	mov	r0, #512
	mov	r1, #128
	ldr	r3, .L6
	ldr	r3, [r3]
	strh	ip, [r3, #6]	@ movhi
	strh	ip, [r3, #30]	@ movhi
	strh	r2, [r3, #14]	@ movhi
	strh	r2, [r3, #22]	@ movhi
	strh	r2, [r3, #46]	@ movhi
	strh	r2, [r3, #54]	@ movhi
	strh	r2, [r3, #78]	@ movhi
	strh	r2, [r3, #86]	@ movhi
	strh	r0, [r3, #38]	@ movhi
	strh	r0, [r3, #62]	@ movhi
	strh	r1, [r3, #70]	@ movhi
	strh	r1, [r3, #94]	@ movhi
	bx	lr
.L7:
	.align	2
.L6:
	.word	affine
	.size	initMatrices, .-initMatrices
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"Start new save...\000"
	.align	2
.LC1:
	.ascii	"You saved! Good job!\000"
	.text
	.align	2
	.global	getSave
	.syntax unified
	.arm
	.fpu softvfp
	.type	getSave, %function
getSave:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L16
	mov	r0, #60
	mov	lr, pc
	bx	r3
	ldr	r4, .L16+4
	ldr	r1, .L16+8
	mov	r3, #0
	str	r0, [r4]
	str	r0, [r1]
	b	.L10
.L15:
	ldr	r0, [r1]
.L10:
	add	r2, r3, #234881024
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r0, r3]
	add	r3, r3, #1
	cmp	r3, #60
	bne	.L15
	ldr	r3, [r4]
	ldrb	r3, [r3, #56]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L11
	ldr	r0, .L16+12
	ldr	r3, .L16+16
	mov	lr, pc
	bx	r3
	mov	r2, #0
	mov	r1, #16
	mov	ip, #1
	ldr	r3, .L16+20
	ldr	r0, .L16+24
	ldr	r4, [r4]
	str	r1, [r3]
	str	r1, [r3, #4]
	str	ip, [r3, #40]
	str	r0, [r3, #44]
	str	r2, [r3, #8]
	str	r2, [r3, #12]
	str	r2, [r3, #16]
	str	r2, [r3, #20]
	str	r2, [r3, #24]
	str	r2, [r3, #28]
	str	r2, [r3, #32]
	str	r2, [r3, #36]
	str	r2, [r3, #48]
	str	r2, [r3, #52]
.L12:
	mov	r0, r4
	ldr	r3, .L16+28
	mov	lr, pc
	bx	r3
	pop	{r4, lr}
	bx	lr
.L11:
	ldr	r0, .L16+32
	ldr	r3, .L16+16
	mov	lr, pc
	bx	r3
	ldr	r4, [r4]
	mov	lr, r4
	ldr	ip, .L16+20
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldm	lr, {r0, r1}
	stm	ip, {r0, r1}
	b	.L12
.L17:
	.align	2
.L16:
	.word	malloc
	.word	saveFile
	.word	transfer
	.word	.LC0
	.word	mgba_printf
	.word	sprite
	.word	shadowOAM
	.word	free
	.word	.LC1
	.size	getSave, .-getSave
	.align	2
	.global	initialize
	.syntax unified
	.arm
	.fpu softvfp
	.type	initialize, %function
initialize:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L20
	mov	lr, pc
	bx	r3
	mov	r2, #67108864
	mov	r1, #5120
	ldr	r4, .L20+4
	strh	r1, [r2]	@ movhi
	mov	r3, #256
	mov	r0, #3
	ldr	r2, .L20+8
	ldr	r1, .L20+12
	mov	lr, pc
	bx	r4
	mov	r0, #3
	ldr	r2, .L20+16
	ldr	r1, .L20+20
	mov	r3, #16384
	mov	lr, pc
	bx	r4
	ldr	r3, .L20+24
	mov	lr, pc
	bx	r3
	mov	ip, #256
	mov	r0, #512
	mov	r2, #0
	mov	r1, #128
	ldr	r3, .L20+28
	ldr	lr, .L20+32
	ldr	r3, [r3]
	ldrh	lr, [lr, #48]
	strh	ip, [r3, #6]	@ movhi
	strh	ip, [r3, #30]	@ movhi
	ldr	ip, .L20+36
	strh	r0, [r3, #38]	@ movhi
	strh	r0, [r3, #62]	@ movhi
	ldr	r0, .L20+40
	strh	lr, [ip]	@ movhi
	strh	r2, [r3, #14]	@ movhi
	strh	r2, [r3, #22]	@ movhi
	strh	r2, [r3, #46]	@ movhi
	strh	r2, [r3, #54]	@ movhi
	strh	r2, [r3, #78]	@ movhi
	strh	r2, [r3, #86]	@ movhi
	strh	r1, [r3, #70]	@ movhi
	strh	r1, [r3, #94]	@ movhi
	pop	{r4, lr}
	strh	r2, [r0]	@ movhi
	b	getSave
.L21:
	.align	2
.L20:
	.word	mgba_open
	.word	DMANow
	.word	83886592
	.word	ssPal
	.word	100728832
	.word	ssTiles
	.word	hideSprites
	.word	affine
	.word	67109120
	.word	buttons
	.word	oldButtons
	.size	initialize, .-initialize
	.align	2
	.global	updateGame
	.syntax unified
	.arm
	.fpu softvfp
	.type	updateGame, %function
updateGame:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #0
	ldr	r3, .L29
	ldrh	r1, [r3, #48]
	ldr	r3, .L29+4
	tst	r1, #64
	str	r2, [r3, #12]
	str	r2, [r3, #20]
	mvneq	r2, #0
	ldr	r1, .L29
	ldrh	r1, [r1, #48]
	streq	r2, [r3, #20]
	tst	r1, #128
	ldr	r1, .L29
	ldrh	r1, [r1, #48]
	addeq	r2, r2, #1
	streq	r2, [r3, #20]
	tst	r1, #32
	mvneq	r1, #0
	movne	r1, #0
	ldr	r0, .L29
	ldrh	r0, [r0, #48]
	streq	r1, [r3, #12]
	ldr	ip, [r3, #8]
	tst	r0, #16
	ldr	r0, [r3, #16]
	addeq	r1, r1, #1
	streq	r1, [r3, #12]
	add	r2, r0, r2
	add	r1, ip, r1
	str	r1, [r3, #8]
	str	r2, [r3, #16]
	bx	lr
.L30:
	.align	2
.L29:
	.word	67109120
	.word	sprite
	.size	updateGame, .-updateGame
	.align	2
	.global	drawGame
	.syntax unified
	.arm
	.fpu softvfp
	.type	drawGame, %function
drawGame:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L34
	ldr	r3, [r2, #8]
	add	r1, r2, #28
	push	{r4, lr}
	ldm	r1, {r1, lr}
	lsl	r3, r3, #23
	ldr	r0, [r2, #44]
	ldrb	ip, [r2, #16]	@ zero_extendqisi2
	lsr	r3, r3, #23
	add	r1, r1, lr, lsl #5
	lsl	r1, r1, #1
	orr	r3, r3, #16384
	strh	r1, [r0, #4]	@ movhi
	strh	r3, [r0, #2]	@ movhi
	strh	ip, [r0]	@ movhi
	ldr	r1, .L34+4
	ldrh	r1, [r1, #48]
	tst	r1, #1
	bne	.L32
	ldr	r2, [r2, #52]
	lsl	r2, r2, #9
	and	r2, r2, #15872
	orr	r3, r3, r2
	orr	ip, ip, #768
	strh	r3, [r0, #2]	@ movhi
	strh	ip, [r0]	@ movhi
.L32:
	ldr	r3, .L34+8
	mov	lr, pc
	bx	r3
	ldr	r3, .L34+12
	mov	lr, pc
	bx	r3
	pop	{r4, lr}
	bx	lr
.L35:
	.align	2
.L34:
	.word	sprite
	.word	67109120
	.word	waitUntilVBlank
	.word	updateOAM
	.size	drawGame, .-drawGame
	.section	.rodata.str1.4
	.align	2
.LC2:
	.ascii	"(%d, %d)\000"
	.align	2
.LC3:
	.ascii	"Game saved! File size: %u bytes\000"
	.text
	.align	2
	.global	saveGame
	.syntax unified
	.arm
	.fpu softvfp
	.type	saveGame, %function
saveGame:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	ldr	r3, .L42
	mov	r0, #60
	mov	lr, pc
	bx	r3
	ldr	r7, .L42+4
	mov	lr, r7
	mov	ip, r0
	mov	r5, r0
	mov	r8, #0
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldm	lr, {r0, r1}
	ldr	r6, .L42+8
	stm	ip, {r0, r1}
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	ldr	r0, .L42+12
	ldr	r7, .L42+16
	strb	r8, [r5, #56]
	str	r5, [r6]
	mov	lr, pc
	bx	r7
	ldr	r3, [r6]
	ldr	r1, .L42+20
	mov	r4, r8
	str	r3, [r1]
	b	.L38
.L41:
	ldr	r3, [r1]
.L38:
	ldrb	r2, [r3, r4]	@ zero_extendqisi2
	add	r3, r4, #234881024
	add	r4, r4, #1
	cmp	r4, #60
	strb	r2, [r3]
	bne	.L41
	ldr	r0, [r6]
	ldr	r3, .L42+24
	mov	lr, pc
	bx	r3
	mov	r1, r4
	ldr	r0, .L42+28
	mov	lr, pc
	bx	r7
	pop	{r4, r5, r6, r7, r8, lr}
	bx	lr
.L43:
	.align	2
.L42:
	.word	malloc
	.word	sprite
	.word	saveFile
	.word	.LC2
	.word	mgba_printf
	.word	transfer
	.word	free
	.word	.LC3
	.size	saveGame, .-saveGame
	.section	.rodata.str1.4
	.align	2
.LC4:
	.ascii	"%x\000"
	.align	2
.LC5:
	.ascii	"%u\000"
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu softvfp
	.type	main, %function
main:
	@ Function supports interworking.
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r7, fp, lr}
	ldr	r3, .L56
	mov	lr, pc
	bx	r3
	ldr	r6, .L56+4
	ldr	r5, .L56+8
	ldr	r10, .L56+12
	ldr	r9, .L56+16
	ldr	r8, .L56+20
	ldr	r7, .L56+24
	ldr	r4, .L56+28
	ldr	fp, .L56+32
.L45:
	ldrh	r3, [r6]
	strh	r3, [r5]	@ movhi
	ldrh	r2, [r4, #48]
	tst	r3, #8
	strh	r2, [r6]	@ movhi
	beq	.L46
	ldrh	r3, [r4, #48]
	tst	r3, #8
	beq	.L55
.L46:
	mov	lr, pc
	bx	r8
	mov	lr, pc
	bx	r7
	ldrh	r3, [r5]
	tst	r3, #4
	beq	.L45
	ldrh	r3, [r4, #48]
	tst	r3, #4
	bne	.L45
	ldr	r3, .L56+36
	mov	lr, pc
	bx	r3
	b	.L45
.L55:
	ldr	r3, [r10, #44]
	mov	r0, fp
	ldrh	r1, [r3]
	mov	lr, pc
	bx	r9
	ldr	r3, [r10, #44]
	mov	r0, fp
	ldrh	r1, [r3, #2]
	mov	lr, pc
	bx	r9
	ldr	r3, [r10, #44]
	mov	r0, fp
	ldrh	r1, [r3, #4]
	mov	lr, pc
	bx	r9
	ldr	r3, [r10, #44]
	ldr	r0, .L56+40
	ldrh	r1, [r3, #6]
	mov	lr, pc
	bx	r9
	b	.L46
.L57:
	.align	2
.L56:
	.word	initialize
	.word	buttons
	.word	oldButtons
	.word	sprite
	.word	mgba_printf
	.word	updateGame
	.word	drawGame
	.word	67109120
	.word	.LC4
	.word	saveGame
	.word	.LC5
	.size	main, .-main
	.comm	transfer,4,4
	.comm	saveFile,4,4
	.comm	sprite,56,4
	.comm	affine,4,4
	.comm	shadowOAM,1024,4
	.comm	oldButtons,2,2
	.comm	buttons,2,2
	.ident	"GCC: (devkitARM release 53) 9.1.0"
