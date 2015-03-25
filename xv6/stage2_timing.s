	.file	"stage2_timing.s"
.globl trial
	.bss
	.align 4
	.type	trial, @object
	.size	trial, 4
trial:
	.zero	4
.globl startTime
	.align 4
	.type	startTime, @object
	.size	startTime, 4
startTime:
	.zero	4
	.section	.rodata
.LC0:
	.string	"Traps Performed: %d\n"
.LC1:
	.string	"Total Elapsed Time: %dus\n"
.LC2:
	.string	"Average Time Per Trap: %dus\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$24, %esp
	movl	$handle_signal, 4(%esp)
	movl	$0, (%esp)			#SIGFPE=0
	call	signal
	movl	$0, 20(%esp)
	movl	startTime, %eax		#find start time
	testl	%eax, %eax			#if not zero, jump to print
	jne	.printResults
	call	uptime
	movl	%eax, startTime
.divByZero:
	movl	$0, %ecx			#Divide by zero
	movl	$1, %eax
	movl	$0, %edx
	idivl	%ecx
	jmp	.divByZero				#Should never be reached
.printResults:
	call	uptime				#find endtime
	movl	startTime, %edx
	subl	%edx, %eax			#find elapsed time
	movl	%eax, 20(%esp)
	movl	trial, %eax
	movl	%eax, 8(%esp)
	movl	$.LC0, 4(%esp)		#Traps Performed: %d
	movl	$1, (%esp)			#stdout=1
	call	printf
	movl	20(%esp), %eax
	imull	$10000, %eax		#convert to microseconds
	movl	%eax, 8(%esp)
	movl	$.LC1, 4(%esp)		#Total Elapsed Time: %d
	movl	$1, (%esp)			#stdout=1
	call	printf
	movl	20(%esp), %eax
	imull	$10000, %eax		#convert to microseconds
	movl	%eax, %edx			#this starts dividing for avg
	sarl	$31, %edx
	idivl	trial			
	movl	%eax, 8(%esp)
	movl	$.LC2, 4(%esp)		#Average Time Per Trap: %d
	movl	$1, (%esp)			#stdout=1
	call	printf
	call	exit
	.size	main, .-main
	.section	.rodata
.LC3:
	.string	"Caught wrong signal: %d Expecting 0\n"
	.text
.globl handle_signal
	.type	handle_signal, @function
handle_signal:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	jne	.badSignal
	movl	trial, %eax
	addl	$1, %eax
	movl	%eax, trial
	cmpl	$999999, %eax		#One million trials
	jle	.nextError
	movl	$0, 4(%esp)			
	movl	$0, (%esp)
	call	main				#Finished, return to main
	call	exit
.badSignal:						#Just in case, shouldn't be hit
	movl	8(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$.LC3, 4(%esp)		#Caught wrong signal %d exp 0
	movl	$2, (%esp)
	call	printf
	call	exit
	leave
	ret
.nextError:
	movl	%ebp, %esp			#Fix stack to avoid overflow
	movl	4(%esp), %eax
	movl	%eax, 8(%esp)
	addl	$4, %esp
	popl	%ebp
	ret
	.size	handle_signal, .-handle_signal
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-11)"
	.section	.note.GNU-stack,"",@progbits
