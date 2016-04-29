.data
.text
.globl __start

	__start:
		
		#lecture d'un entier
		ori $2, $0, 5			
		syscall

		#stockage de l'entier saisie 
		or $10, $2, $0
		
		#lecture d'un entier
		ori $2, $0, 5
		syscall
		
		#stockage de l'entier saisie 
		or $11, $2, $0
		
		#allocation de la pile		
		addi $29, $29,-8
		
		#stockage des arguments dans la pile
		sw $10,0($29)
		sw $11,4($29)
		
		or $15,$0,$0
		ori $12,$0,1
		jal puissance 
		
		lw $10,4($29)
		lw $11,8($29)

		addi $29 ,$29 ,8

		#affichage du resulat
		move $4,$12
		li $2,1
		syscall		
		
		j    Exit        # saut a la fin du programme

		puissance:
	   	addi $29 ,$29 ,-4
		sw $31,8($29)
		jal while 
		lw $31,8($29)
		addi $29 ,$29 ,4
		jr $31

		while:
		blt $15, $11, calcul
		jr $31
    		
		calcul:
		mul $12, $12, $10
		addi $15,$15,1
		j while

Exit:                    # fin du programme
