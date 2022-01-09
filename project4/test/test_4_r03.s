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
  li $s0, 110 #### v0 = 110
  sw $s0, array + 0 #### store variable:v0
  li $s0, 97 #### v2 = 97
  sw $s0, array + 4 #### store variable:v2
  li $s0, 3 #### v4 = 3
  sw $s0, array + 8 #### store variable:v4
  lw $t0, array + 0 #### variable: v0, data offset: 0
  lw $t1, array + 4 #### variable: v2, data offset: 4
  sub $s0, $t0, $t1 ######## v8 = v0 - v2
  sw $s0, array + 12 #### store variable:v8
  li $s0, 2 #### v13 = 2
  sw $s0, array + 16 #### store variable:v13
  lw $t0, array + 8 #### variable: v4, data offset: 8
  lw $t1, array + 16 #### variable: v13, data offset: 16
  mul $s0, $t0, $t1
  sw $s0, array + 20 #### store variable:v11
  lw $t0, array + 12 #### variable: v8, data offset: 12
  lw $t1, array + 20 #### variable: v11, data offset: 20
  add $s0, $t0, $t1
  sw $s0, array + 24 #### store variable:v7
  lw $t0, array + 24 #### variable: v7, data offset: 24
  move $s0, $t0 #### v4 = v7
  sw $s0, array + 8 #### store variable:v4
  lw $t0, array + 8 #### variable: v4, data offset: 8
  move $s0, $t0 #### v15 = v4
  sw $s0, array + 28 #### store variable:v15
  move $t7,$a0
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $a0,$t7
  li $s0, 0 #### v16 = 0
  sw $s0, array + 32 #### store variable:v16
  lw $t0, array + 32 #### variable: v16, data offset: 32
  move $v0, $t0
  jr $ra