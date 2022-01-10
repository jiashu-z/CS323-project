# SPL compiler generated assembly
.data
array: .space 400
_prmpt: .asciiz "Enter an integer: "
_eol: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  move $s6, $a0
  la $a0, _prmpt
  syscall
  move $a0, $s6
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  move $s6, $a0
  la $a0, _eol
  syscall
  move $a0, $s6
  move $v0, $0
  jr $ra
main:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s0, $v0
  sw $s0, array + 0
  lw $t0, array + 0 #### variable: v2, data offset: 0
  move $s0, $t0 #### v0 = v2 
  sw $s0, array + 4 #### store variable:v0
  lw $t0, array + 4 #### variable: v0, data offset: 4
  move $s0, $t0 #### v3 = v0 
  sw $s0, array + 8 #### store variable:v3
  li $s0, 0 #### v4 = 0
  sw $s0, array + 12 #### store variable:v4
  lw $t0, array + 8 #### variable: v3, data offset: 8
  lw $t1, array + 12 #### variable: v4, data offset: 12
  bgt $t0, $t1, label0
  j label1
label0:
  li $s0, 1 #### v6 = 1
  sw $s0, array + 16 #### store variable:v6
  move $t7,$a0 
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $a0,$t7 
  j label2
label1:
  lw $t0, array + 4 #### variable: v0, data offset: 4
  move $s0, $t0 #### v7 = v0 
  sw $s0, array + 20 #### store variable:v7
  li $s0, 0 #### v8 = 0
  sw $s0, array + 24 #### store variable:v8
  lw $t0, array + 20 #### variable: v7, data offset: 20
  lw $t1, array + 24 #### variable: v8, data offset: 24
  blt $t0, $t1, label3
  j label4
label3:
  li $s0, 1 #### v11 = 1
  sw $s0, array + 28 #### store variable:v11
  lw $t0, array + 28 #### variable: v11, data offset: 28
  neg $t0, $t0
  addi $s0, $t0, 0 ######## v10 = 0 - 
  sw $s0, array + 32 #### store variable:v10
  move $t7,$a0 
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $a0,$t7 
  j label5
label4:
  li $s0, 0 #### v13 = 0
  sw $s0, array + 36 #### store variable:v13
  move $t7,$a0 
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $a0,$t7 
label5:
label2:
  li $s0, 0 #### v14 = 0
  sw $s0, array + 40 #### store variable:v14
  lw $t0, array + 40 #### variable: v14, data offset: 40
  move $v0, $t0
  jr $ra
