//******************************************************
// Program Title:   Write and Read data using I2C bus and Verdin iMX8M plus( a toradex SoM running s linux based OS)
// Author:          sabin bhandari
// Date:            November 13, 2024
// Purpose:         This program performs writing data , reading data and diumping data from serial EEPROM using I2C bus 
//******************************************************
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>         //comtain uin8_t
#include <unistd.h>         // write read close function are defined

#define SLAVE_ADDRESS           0x50
#define I2C_BUS_PATH            "/dev/i2c-2"
#define REGISTER_ADDRESS        0x00
#define  ROW_DATA_SIZE          16

bool intialization_i2c();
bool write_data_eeprom(uint8_t *data, size_t data_length);
bool read_data_eeprom(void);
bool dump_data_eeprom(uint8_t *data , size_t dump_data_size);

int g_fd = 0;
long g_registeraddress = 0;

int main(int argc, char *argv[])
{
    if(!intialization_i2c())
    {
        return -1;
    }

    //listing out all the data
    char *which_mode = argv[1];  //data input is  either R, W or D (read,write or dump) in respective order 
    g_registeraddress  = strtol(argv[2], NULL, 16);
   
    // reading which mode is called in program

    switch(which_mode[0])
    {
        case 'w':
        case 'W':
            //writing data in memory
            //bool write_data_eeprom(uint8_t *data)
            //adding data on the same pipeline for data manipulation
            if(argc == 4){
                //reading data and then converting that into array
                long data_mode = strtol(argv[3], NULL, 16);
                uint8_t data[] = { static_cast<uint8_t>(data_mode) };
                size_t data_length = sizeof(data);
                if(write_data_eeprom(data, data_length))
                {
                    printf("data written succefully \n");
                }
                else{
                    perror("failed to write ");
                    return -1;
                }
            }       
            close(g_fd);
        break;

        case 'r':
        case 'R':
            printf("I you reading a read _eepron function \n");
            if( argc == 3){
                
                if(read_data_eeprom()){
                    printf("data read successful !\n");
                    //printf("data read:: 0x%02x 0x%02x 0x%02x \n", read_buffer[0], read_buffer[1], read_buffer[2]); 
                    
                }
                else{
                    printf("failed to read \n");
                    return -1;
                }
                close(g_fd);
            }
        break;

        case 'd':
        case 'D':
            {
                

                if(argc == 4)
                {
                    unsigned int counter = 0;
                    
                    //reading data and then converting that into array
                    long dump_data = strtol(argv[3], NULL, 16);
                    //uint8_t data[] = { static_cast<uint8_t>(data_mode) };

                    unsigned int dump_data_size = static_cast<unsigned int>(dump_data);
                    uint8_t dump_buffer[dump_data_size];
                    printf(" dump data size %ud \n", dump_data_size);

                    //size_t data_length = sizeof(data);
                    //bool dump_data_eeprom(uin8_t *data , size_t dump_data_size)
                    if(dump_data_eeprom(dump_buffer, dump_data_size)){
                        printf("data dump successful !\n");
                        printf("\n--------------------------------DUMPING DATA-------------------------------------------------\n");
                        printf("---------------------------------------------------------------------------------------------\n");
                        printf("0x%08x \t ",static_cast<unsigned int>(g_registeraddress));
                        // bufffer is created because global varaible may be need iin future mofification of program. buffer is used inside the loop only
                        unsigned int register_address_buffer = static_cast<unsigned int>(g_registeraddress);
                        for(size_t i = 0; i < dump_data_size; i++){
                              if(counter <  ROW_DATA_SIZE)
                            {
                                printf("0x%02x \t ", dump_buffer[i]);
                                counter++;
                            }
                            else
                            {
                                counter = 0;
                                register_address_buffer = register_address_buffer + 16;
                                printf("\n0x%08x \t ",static_cast<unsigned int>(register_address_buffer));
                                printf("0x%02x \t ", dump_buffer[i]);

                            }
                            
                        }
                        printf("\n---------------------------------------------------------------------------------------------\n");
                    }
                    else{
                        printf("failed to read \n");
                        return -1;
                    }   
                }
                close(g_fd);
                break;
            }
        default:
        {
            printf("command line argument is wrong! TRY AGAIN!! \n");
            break;
        }
    }
    return 0;  
}

//intilaizing I2C communication 
bool intialization_i2c()
{
    g_fd = open(I2C_BUS_PATH, O_RDWR);
    
    if (g_fd < -1)
    {
        printf("open('%s') cannot be opened ! check  command --->>> ls /dev/i2c-1", I2C_BUS_PATH);
        close(g_fd);
        return false;
    }
    
    if(ioctl(g_fd, I2C_SLAVE, SLAVE_ADDRESS) < 0)
    {
        perror("failed to acquire i2c bus or failed to talk to slave");
        close(g_fd);
        return false;
    }
    return true;
}

bool write_data_eeprom(uint8_t *data, size_t data_length)
{
  
    int length = static_cast<int>(data_length);
    uint8_t buffer[length + 1];  // as data_lenth is unsigned long integer which cause problem 
    buffer[0] = g_registeraddress;
    int i;
    for (i = 0; i < length ; i++){
        buffer[i+1] = data[i];
        // this is done to include both registeraddress and data on a single array line
    }
    printf("total memory used for writing is %ld \n", data_length);
    printf("value of file descriptor :: %d  \n", g_fd);

    if(ioctl(g_fd, I2C_SLAVE, SLAVE_ADDRESS) < 0)
    {
        perror("failed to acquire i2c bus or failed to talk to slave \n");
        close(g_fd);
        return false;
    }

    //ssize_t write(int fd, const void *buf, size_t count);
    ssize_t wr = write(g_fd, buffer, data_length + 1);
    printf("return value of write function pointer %zd \n ", wr);

    if(wr == static_cast<ssize_t>(data_length + 1))
    {
        printf("successflly written to the I2C bus \n");
        //waiting write cycle to complete
        usleep(5000);
        return true;
    }
    else{
        perror("coulnot write to the I2C bus \n");
        return false;
    }
}

//reading data from a eeprom 
bool read_data_eeprom(void)
{
    if(ioctl(g_fd, I2C_SLAVE, SLAVE_ADDRESS) < 0)
    {
        perror("failed to acquire i2c bus or failed to talk to slave \n");
        close(g_fd);
        return false;
    }

    //convertibg long int ---> uint8_t
    uint8_t register_add = static_cast<uint8_t> (g_registeraddress);
    uint8_t reg_add[] = {register_add};

    //　①　writing register address before 
    //　②　reading data from that register address
    if(write(g_fd, &reg_add, 1) != 1){
        perror("failed to write to the I2C bus");
        return false;
    }

    uint8_t data[1];
   // if(read(g_fd, data, sizeof(data)) != sizeof(data)){
    if(read(g_fd, data, 1) != 1 ){
        perror("failed to read from I2C bus");
        return false;
    }
    else{
        printf("\n--------------------------------READING DATA-------------------------------------------------\n");
        printf("---------------------------------------------------------------------------------------------\n");
        printf("0x%08x  :: 0x%02x\n", static_cast<unsigned int>(g_registeraddress) ,  data[0]);
        printf("---------------------------------------------------------------------------------------------\n");
    }
    return true;
}


/**! bool dump_data_eeprom(uint8_t *data)  --->NOTICE
 *  "data" argument used here defined the amount of data it want to dispaly not the actual data of memory
 */
bool dump_data_eeprom(uint8_t *data , size_t dump_data_size)
{
    
    ssize_t dump_data_size_convert = static_cast<ssize_t> (dump_data_size);
    //uint8_t dump_buffer[dump_data_size];


    if(ioctl(g_fd, I2C_SLAVE, SLAVE_ADDRESS) < 0)
    {
        perror("failed to acquire i2c bus or failed to talk to slave \n");
        close(g_fd);
        return false;
    }

    //ssize_t write(int fd, const void *buf, size_t count);
    if(write(g_fd, &g_registeraddress, 1) != 1)
    {
        perror("failed to write register address for purpose of dumping data \n");
        close(g_fd);
        return false;
    }

    //printf("dump data size inside call function %zd \n", dump_data_size);
    if(read(g_fd, data, dump_data_size) != dump_data_size_convert)
    {
        perror("failed to dump data , check the size of dumping data size \n ");
        close(g_fd);
        return false;
    }
   /* else
    {
        printf("data dump:: ");
        for(size_t i = 0; i < dump_data_size; i++){
        printf("0x%02x , ", dump_buffer[i]);
        }
        printf("\n");          
    }
    */
    return true;
}