gcc -o pager pager.c && ./pager 4 2&
pid_pager=pgrep pager
gcc -o mmu mmu.c &&./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager