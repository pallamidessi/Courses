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
			
			addi $18,$18,-1
			lw $4,0($29)
			addi $29,$29,4

			sw $4,0($29)
			addi $29,$29,-4

			jal test 			
		
		j    Exit        # saut a la fin du programme
		
		strlen:
  		addi $29 ,$29 ,-4
			sw $31,4($29)			
			ori $15,$0,10
			or $18,$0,$0
			jal while
			lw $31,4($29)
			addi $29,$29,4
			
		while:		
			bne $15 $10 parc
			jr $31

		parc:
			addi $18,1
			addi $4,1
			lb $10 0($4)
			j while

		test:
			addi $29 ,$29 ,-4
			sw $31,4($29)
			or $13,$0,$0
			or $14,$0,$0
			jal while
			lw $31,4($29)
			addi $29,$29,4
					
			while:
			beq $14 $13 suivant
			beq 
						

Exit:                    # fin du programme
