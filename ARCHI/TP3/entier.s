.data

.text
.globl __start

	__start:
		
		#lecture d'un entier
		ori $2, $0, 5		
		syscall

		#affichage de l'entier saisie 
		or $4, $2, $0
		li $2, 1
		syscall
		

		j    Exit        # saut a la fin du programme

	procedure:

      
Exit:                    # fin du programme
