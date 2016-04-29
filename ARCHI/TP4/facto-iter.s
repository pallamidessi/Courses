.data
.text
.globl __start

	__start:
		
		#lecture d'un entier
		ori $2, $0, 5			
		syscall

		#stockage de l'entier saisie dans le registr $10
		or $10, $2, $0

		#allocation de la pile		
		addi $29, $29,-4
		
		#stockage des arguments dans la pile
		sw $10,0($29)

		jal facto
		
		#restaure les arguments
		lw $10,0($29)

		#desalloue la pile 
		addi $29 $29 4
		
		

		#affichage d'un entier
		move $4 $15
		li $2, 1
		syscall

		j    Exit        # saut a la fin du programme
		
		facto:
	   	addi $29 ,$29 ,-4 	#allocation de la pile pour la valeur de retour	
		sw $31,4($29)		#stockage de la valeur de retour dans la pile
		move $15 $10		#on place la valeur de $10 dans $15
		jal iter		#on entre dans la boucle 
		lw $31,4($29)		#on restaure la valeur de retour
		addi $29 ,$29 ,4	#desallocation de la pile   
		jr $31
		
		iter:
		addi $10 $10 -1		#decrementente la valeur initial
		beq $10 $0 fin		#on test si la nouvelle valeur est egale a 0
		mul $15 $15 $10		#$15=$15*$10
		j iter

		fin:
		jr $31
Exit:                    # fin du programme
