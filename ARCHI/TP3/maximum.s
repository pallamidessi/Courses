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

		#comparaison
		bge  $10,$11,affiche_1
		blt  $10,$11,affiche_2
		



		
		j    Exit        # saut a la fin du programme

	#affichage d'un entier
	
	affiche_1:
		move $4 $10
		li $2, 1
		syscall
		j Exit
	
	#affichage d'un entier
	
	affiche_2:
		move $4 $11
		li $2, 1
		syscall
		j Exit
      
Exit:                    # fin du programme
