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
hanoi:
  move $s0, $a0 #### v4 = v0 
  sw $s0, array + 0 #### store variable:v4
  li $s0, 1 #### v5 = 1
  sw $s0, array + 4 #### store variable:v5
  lw $t0, array + 0 #### variable: v4, data offset: 0
  lw $t1, array + 4 #### variable: v5, data offset: 4
  beq $t0, $t1, label0
  j label1
label0:
  move $s0, $a1 #### v9 = v1 
  sw $s0, array + 8 #### store variable:v9
  li $s0, 10000 #### v10 = 10000
  sw $s0, array + 12 #### store variable:v10
  lw $t0, array + 8 #### variable: v9, data offset: 8
  lw $t1, array + 12 #### variable: v10, data offset: 12
  mul $s0, $t0, $t1
  sw $s0, array + 16 #### store variable:v8
  move $s0, $a3 #### v11 = v3 
  sw $s0, array + 20 #### store variable:v11
  lw $t0, array + 16 #### variable: v8, data offset: 16
  lw $t1, array + 20 #### variable: v11, data offset: 20
  add $s0, $t0, $t1
  sw $s0, array + 24 #### store variable:v7
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
  move $s0, $a0 #### v14 = v0 
  sw $s0, array + 28 #### store variable:v14
  li $s0, 1 #### v15 = 1
  sw $s0, array + 32 #### store variable:v15
  lw $t0, array + 28 #### variable: v14, data offset: 28
  lw $t1, array + 32 #### variable: v15, data offset: 32
  sub $s0, $t0, $t1 ######## v13 = v14 - v15
  sw $s0, array + 36 #### store variable:v13
  move $s0, $a1 #### v16 = v1 
  sw $s0, array + 40 #### store variable:v16
  move $s0, $a3 #### v17 = v3 
  sw $s0, array + 44 #### store variable:v17
  move $s0, $a2 #### v18 = v2 
  sw $s0, array + 48 #### store variable:v18
  addi $sp, $sp, -20 #### fixed
  sw $ra, 0($sp)
  sw $a0, 4($sp) #### fixed
  sw $a1, 8($sp) #### fixed
  sw $a2, 12($sp) #### fixed
  sw $a3, 16($sp) #### fixed
  lw $a0, array + 36
  lw $a1, array + 40
  lw $a2, array + 44
  lw $a3, array + 48
  jal hanoi
  lw $ra, 0($sp)
  lw $a0, 4($sp)
  lw $a1, 8($sp)
  lw $a2, 12($sp)
  lw $a3, 16($sp)
  addi $sp, $sp, 20
  move $s0, $v0
  sw $s0, array + 52
  move $s0, $a1 #### v22 = v1 
  sw $s0, array + 56 #### store variable:v22
  li $s0, 10000 #### v23 = 10000
  sw $s0, array + 60 #### store variable:v23
  lw $t0, array + 56 #### variable: v22, data offset: 56
  lw $t1, array + 60 #### variable: v23, data offset: 60
  mul $s0, $t0, $t1
  sw $s0, array + 64 #### store variable:v21
  move $s0, $a3 #### v24 = v3 
  sw $s0, array + 68 #### store variable:v24
  lw $t0, array + 64 #### variable: v21, data offset: 64
  lw $t1, array + 68 #### variable: v24, data offset: 68
  add $s0, $t0, $t1
  sw $s0, array + 72 #### store variable:v20
  move $t7,$a0 
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $a0,$t7 
  move $s0, $a0 #### v27 = v0 
  sw $s0, array + 76 #### store variable:v27
  li $s0, 1 #### v28 = 1
  sw $s0, array + 80 #### store variable:v28
  lw $t0, array + 76 #### variable: v27, data offset: 76
  lw $t1, array + 80 #### variable: v28, data offset: 80
  sub $s0, $t0, $t1 ######## v26 = v27 - v28
  sw $s0, array + 84 #### store variable:v26
  move $s0, $a2 #### v29 = v2 
  sw $s0, array + 88 #### store variable:v29
  move $s0, $a1 #### v30 = v1 
  sw $s0, array + 92 #### store variable:v30
  move $s0, $a3 #### v31 = v3 
  sw $s0, array + 96 #### store variable:v31
  addi $sp, $sp, -20 #### fixed
  sw $ra, 0($sp)
  sw $a0, 4($sp) #### fixed
  sw $a1, 8($sp) #### fixed
  sw $a2, 12($sp) #### fixed
  sw $a3, 16($sp) #### fixed
  lw $a0, array + 84
  lw $a1, array + 88
  lw $a2, array + 92
  lw $a3, array + 96
  jal hanoi
  lw $ra, 0($sp)
  lw $a0, 4($sp)
  lw $a1, 8($sp)
  lw $a2, 12($sp)
  lw $a3, 16($sp)
  addi $sp, $sp, 20
  move $s0, $v0
  sw $s0, array + 100
label2:
  li $s0, 0 #### v32 = 0
  sw $s0, array + 104 #### store variable:v32
  lw $t0, array + 104 #### variable: v32, data offset: 104
  move $v0, $t0
  jr $ra
main:
  li $s0, 3 #### v34 = 3
  sw $s0, array + 108 #### store variable:v34
  lw $t0, array + 108 #### variable: v34, data offset: 108
  move $s0, $t0 #### v33 = v34 
  sw $s0, array + 112 #### store variable:v33
  lw $t0, array + 112 #### variable: v33, data offset: 112
  move $s0, $t0 #### v36 = v33 
  sw $s0, array + 116 #### store variable:v36
  li $s0, 1 #### v37 = 1
  sw $s0, array + 120 #### store variable:v37
  li $s0, 2 #### v38 = 2
  sw $s0, array + 124 #### store variable:v38
  li $s0, 3 #### v39 = 3
  sw $s0, array + 128 #### store variable:v39
  addi $sp, $sp, -20 #### fixed
  sw $ra, 0($sp)
  sw $a0, 4($sp) #### fixed
  sw $a1, 8($sp) #### fixed
  sw $a2, 12($sp) #### fixed
  sw $a3, 16($sp) #### fixed
  lw $a0, array + 116
  lw $a1, array + 120
  lw $a2, array + 124
  lw $a3, array + 128
  jal hanoi
  lw $ra, 0($sp)
  lw $a0, 4($sp)
  lw $a1, 8($sp)
  lw $a2, 12($sp)
  lw $a3, 16($sp)
  addi $sp, $sp, 20
  move $s0, $v0
  sw $s0, array + 132
  li $s0, 0 #### v40 = 0
  sw $s0, array + 136 #### store variable:v40
  lw $t0, array + 136 #### variable: v40, data offset: 136
  move $v0, $t0
  jr $ra
