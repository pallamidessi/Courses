.data
	#Datas...
	hello:  .asciiz "hello world\n"   # hello pointe vers "hello world\n\0"
.text
.globl __start

	__start:

		
		la $4, hello
		ori $2, $0, 4 
		syscall

	
		j    Exit        # saut a la fin du programme

      
Exit:                    # fin du programme
