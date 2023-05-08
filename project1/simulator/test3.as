        lw      0       1       neg     extreme number test
        lw      0       2       big
        lw      0       3       small
        beq     0       0       jmp 
done    halt
jmp     add     2       3       2
        beq     1       2       done
abnorm  halt
neg     .fill   -1
big     .fill   2147483647
small   .fill   -2147483648
dnAddr  .fill   done 
jpAddr  .fill   jmp
