# Reading-EMC2101-info-using-STM32
**Objective:** Create C++ class to print temperatures and product id of the EMC2101 IC which is controlled by a STM32 Microcontroller.

Points to note:

    1. STM32G484QETX microcontroller is selected.
    2. A class is created which has member functions to print internal temperature, external temperature and product id. All the temperature values are printed in decimal (Degree Celsius). The product id is also printed in decimal.
    3. It is assumed that the data from resistors is obtained in hexa format. 
    4. The EMC2101 is addressed in the SMBus as 100_1100b (according to data sheet). This is assumed to be 1001100 in binary. Corresponding hexa representation i.e. 0x4c, is used. The address is 7 bit, there it is left shifted to assign IC’s address in the code
    5. It is assumed that EMC2101 address and the corresponding resistor addresses stay constant for all the EMC2101 IC’s. In this case, all these values can be hard coded in the Class and are not required to be passed as parameters for the constructor. However, to showcase a constructor and destructor formation, the address values are not hard coded and passed as parameters when object of the class is created.
    6. The code was developed and compiled on STM32CubeIDE to check for compilation errors. Individual logic for 2’s complement and hexa to decimal conversion was validated separately using dummy inputs. 
