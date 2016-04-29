.data
entier_A:	.asciiz "Rentrer l'entier dont on veut les multiples :\n"   # hello pointe vers "hello world\n\0"
entier_N: 	.asciiz "Le nombre de multiple voulue :\n"   
erreur:  	.asciiz "erreur depassement de la taille des registre ou entier donnee negatifou egale a 0\n"   
newline: 	.asciiz "\n" #retour a la ligne 
.text
.globl __start

	__start:
		
		jal msg_entier_A
		
		#lecture de l'entier A
		li $2, 5
		syscall
		move $5 $2
		ble $5, $0, msg_erreur  
		
		jal msg_entier_N
		
		#lecture de l'entier N
		li $2,5
		syscall
		move $6 $2
		ble $6, $0, msg_erreur  
		
		li $10,0
		#Multiplication 
		test:
		jal retour
		bne $6,$0, multiplication	#le cas d'arret est quand N est egale a 0
		

		
		j    Exit        # saut a la fin du programme
		
		
		#affichage message d'erreur
		msg_erreur:
		la $4 erreur
		li $2, 4
		syscall
		j Exit
		
		#affichage message demande de A a l'utilisateur 
		msg_entier_A:
		la $4 entier_A
		ori $2, $0, 4
		syscall
		jr $31
				
		#affichage message demande de N a l'utilisateur 
		msg_entier_N:
		la $4 entier_N
		ori $2, $0, 4
		syscall
		jr $31
		
		#fait et affichage le resultat de la multiplication
		multiplication:
		addi $10,$10,1 				#on incremente le registre 10 qui est le multiplicateur
		mul $4, $5, $10				#on multiplie A ($5) par $10,est on stocke le resultat dans $4 pour l'afficher
		ble $4, $0, msg_erreur  	#si le resultat est inferieur a 0 il y a eu un de passement de la valeur du registre
		addi $6,$6,-1				#on soustrait 1 a N
		li $2, 1
		syscall
		j test
		
		#affiche un retour a la ligne 
		retour:
		li $2,4
		la $4 , newline 
		syscall
		jr $31
		
Exit:                    # fin du programme
