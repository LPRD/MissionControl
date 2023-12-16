/** CppLinuxSerial Example 0B
 * Sends a single hello message from serial device B 
 * and reads a single hello mesage from serial device A.
 * Requires the user to input the device number 
 * and desired baud rate
 * 
 * build: (examples folder)
 mkdir -p output build
 g++ serialExample0B.cpp -lCppLinuxSerial -o ../output/example0B
 *
 * run: (output folder)
 ./example0B
 * 
 * 
*/

#include <CppLinuxSerial/SerialPort.hpp>
#include <thread>           //std::this_thread::sleep_for
#include <stdlib.h>         //exit()

using namespace mn::CppLinuxSerial;


int main(int argc, char* argv[])
{
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <usb-device-number> <baud-rate>" << std::endl;
        std::cout << "<usb-device-number> should be an int" << std::endl;
        std::cout << "<baud-rate> should be an int" << std::endl;
        exit(1);
    }

    std::string deviceNum(argv[1]);

    if(deviceNum.length() > 2){
        std::cout << "<usb-device-number> should not exceed 2 characters!" << std::endl;
        exit(1);
    }

    std::string baudRate(argv[2]);

    if(baudRate.length() > 6){
        std::cout << "<baud-rate> should not exceed 6 characters! " << std::endl;
        exit(1);
    }

    int rate = std::stoi(baudRate);

    BaudRate rateToUse;
    switch(rate) 
    {
        case 4800:
            rateToUse = BaudRate::B_4800;
            break;
        case 9600:
            rateToUse = BaudRate::B_9600;
            break;
        case 19200:
            rateToUse = BaudRate::B_19200;
            break;
        case 38400:
            rateToUse = BaudRate::B_38400;
            break;
        case 57600:
            rateToUse = BaudRate::B_57600;
            break;
        case 115200:
            rateToUse = BaudRate::B_115200;
            break;
        case 230400:
            rateToUse = BaudRate::B_230400;
            break;
        case 460800:
            rateToUse = BaudRate::B_460800;
            break;
        default:
            rateToUse = BaudRate::B_57600;
    }

	// Create serial port object and open serial port at 57600 baud, 8 data bits, no parity bit, one stop bit (8n1),
	// and no flow control
	SerialPort serialPort("/dev/ttyUSB" + deviceNum, rateToUse, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
	// Use SerialPort serialPort("/dev/ttyACM0", 13000); instead if you want to provide a custom baud rate
	serialPort.SetTimeout(100); // Block for up to 100ms to receive data
	serialPort.Open();

	// WARNING: If using the Arduino Uno or similar, you may want to delay here, as opening the serial port causes
	// the micro to reset!
    std::cout << "About to write" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Writing..." << std::endl;

	// Write some ASCII data
	serialPort.Write("Hello from B");

	// Read some data back (will block for up to 100ms due to the SetTimeout(100) call above)
	std::string readData;
	serialPort.Read(readData);
	std::cout << "Read data = \"" << readData << "\"" << std::endl;

	// Close the serial port
    std::cout << "Closing..." << std::endl;
	serialPort.Close();
}
