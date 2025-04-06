target remote :1234
# break on start of program
break *0x80000000
break main
continue
