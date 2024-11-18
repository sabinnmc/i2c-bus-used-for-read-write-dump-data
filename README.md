# i2c-bus-used-for-read-write-dump-data
i2c AT24C02C serial EEPROM is used for this project


# WRITE, READ , AND DUMP data through I2C bus of EEPROM 
## argv[0] = "./bin/maain"    
    here command line argument ---> argv[0] always read final executable
## argv[1] = read character "R" "W" "D" 
***R = read a data***  
***W = erite single byte of data***  
***D = dump the data from respective register address***

## INPUT DATA LOOKLIKE
  while writing make sure to have four argument as below  
  ### ./bin/main w 00 A1  
  OR  
  ### ./bin/main W 00 a1  

  here  ./bin/main   -->> is a executable file after creating a object file  
        W/w   -->> it indicate file is at writing mode and it read one 8bit data at a time  
        00   -->> meaning is 0x00 which means the address of register where user gonna write data  (note:  00 < = address_register <= FF)  
        a1   -->> it is a 8 bit data that is going to be written inside 0x00 register address of this device    

 ### ./bin/main R 11    
  OR  
  ### ./bin/main r 11    

here  ./bin/main   -->> is a executable file after creating a object file  
        r/R   -->> it indicate file is at reading  mode and it read one 8bit data at a time  
        11   -->> meaning is 0x11 which means the address of register where user want t0 read  data  (note:  00 < = address_register <= FF)  
        (note: it take 3 argument only)  


### ./bin/main D 11 4   
  OR  
  ### ./bin/main d 11 4   

here  ./bin/main   -->> is a executable file after creating a object file  
        d/D   -->> it indicate file is at dumping  mode and it read one  8bit data starting from 0x11 register  to next 4 register in continous format that is 0x11 0x12 0x13 0x14 . All these register data will be read in dump case in this program   
        11   -->> meaning is 0x11 which means the address of register where user want to start reading upto  next 4 register  (note:  00 < = address_register <= FF)  
        (note: it take 3 argument only)
        4   --->>  here 4 is size of data user wish to register 
         



