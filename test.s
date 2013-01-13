.data
w:          .space 10
hello:      .asciiz "Hello world!"

.text

print_int:
    li $v0, 1
    lw $a0, 0($sp)
    syscall
    jr $ra


main:
    li $5, 23

    sw $ra, -8($sp)
    la $sp, -8($sp)

    sw $5, -4($sp)
    la $sp, -4($sp)

    jal print_int

    la $sp, 4($sp)
    la $sp, 8($sp)
    lw $ra, -8($sp)

    mfhi $2    
    jr $ra
