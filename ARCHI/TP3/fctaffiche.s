.data
.text
.globl __start

	__start:
		

		#Met 6 dans le registre 4
		ori $4, $0, 6			
		jal affiche 
		
		#Met 7 dans le registre 4
		ori $4, $0, 7			
		jal affiche 
		
		#Met 8 dans le registre 4
		ori $4, $0, 8			
		jal affiche 
		
		#Met 9 dans le registre 4
		ori $4, $0, 9			
		jal affiche 
		

		
		j    Exit        # saut a la fin du programme
		
		#affichage
		affiche:
		li $2, 1
		syscall
		jr $ra
      
Exit:                    # fin du programme
