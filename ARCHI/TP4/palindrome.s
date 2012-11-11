.data
read: .space 256
vrai:.asciiz " la chaine est un palindrome\n"
faux:.asciiz " la chaine n'est pas un palindrome\n"
.text
.globl __start

	__start:
		
		
			la $4, read # charge le pointeur read dans $4
			ori $5, $0, 256 # charge la longueur maximale dans $5
			ori $2, $0, 8 # charge 8 (code de l’appel système) dans $2
			syscall # Exécute l’appel système
			
			la $4,read
			ori $2,$0,4
			syscall
			
			addi $29,$29,-4
			sw $4,0($29)

			jal strlen 
			
			lw $4,0($29)
			addi $29,$29,4
			
			la $4,read
			addi $18,$18,-1
			jal test 			
		
		j    Exit        # saut a la fin du programme
		
		strlen:
  		addi $29 ,$29 ,-4
			sw $31,4($29)			
			ori $15,$0,10
			or $18,$0,$0
			lb $10 0($4)
			jal rechercheFin
			lw $31,4($29)
			addi $29,$29,4
			
		rechercheFin:		
			bne $15 $10 parc
			jr $31

		parc:
			addi $18 $18 1
			addi $4 $4 1
			lb $10 0($4)
			j rechercheFin

		test:
			addi $29 ,$29 ,-4 
			sw $31,4($29)
			or $13,$0,$0
			or $14,$0,$0
			add $20,$4,$18
			jal while
			lw $31,4($29)
			addi $29,$29,4
			jr $31
					
		while:
			beq $14 $13 suivant		#si la lettre de debut est egale a la lettre de fin on continue 
			j nonPalindrome			#on affiche le message d'echec dans le cas contraire
			
		suivant:
			beq $4 $20 estPalindrome		#si l'adresse de debut est egale a l'adresse de la fin ,alors c'est un palindrome
			lb $13 0($4)				#on charge une lettre au debut
			lb $14 0($20)				#on charge une lettre a la fin 
			addi $4 $4 1				#on incremente l'adresse de debut
			addi $20 $20 -1			#on decremente l'adresse de fin 
			bgt $4 $20 estPalindrome		#si l'adresse de fin et de debut se depasse alors l'entree est un palindrome
			j while
			
		#affichage du message si l'entree est un palindrome
		estPalindrome:
			la $4 vrai
			ori $2, $0, 4
			syscall
			j Exit
		
		#affichage du message si l'entree n'est pas un palindrome
		nonPalindrome:
			la $4 faux
			ori $2, $0, 4
			syscall
			j Exit

Exit:                    # fin du programme
