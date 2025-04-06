### tests
1. t1 : single assembly file to start and print to qemu uart
2. t2 : asm file to initialize stack and jump to main in c
3. t3 : baremetal with support for interrupts and ...
4. t4 : asm file to init stack, fpu and jump to main in c
5. t5 : simba porting

### todo

- [ ] cleanup t3
- [ ] look into spike machine
- [ ] write functions to print floats and ints
- [ ] port simba in t5
