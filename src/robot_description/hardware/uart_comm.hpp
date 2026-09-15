#include <serial/serial.h>
#include <vector.hpp>
#include <cstdint>
#include <string>


class uartcomm
{
    private:
    serial::Serial serialport_

    public:

    bool connect(std::string& portname,std::uint32_t baudrate)
    {
        try {
            serialPort_.setPort(port);
            serialPort_.setBaudrate(baudrate);
            
            serialPort_.setTimeout(serial::Timeout::simpleTimeout(1000));
            serialPort_.open();
            return serialPort_.isOpen();
        } catch (const std::exception& e) {
            return false;
        }
    }

    bool parse_send_packagage(std::vector<double> & velocity,std::vector<double> & position)
    {   try
        {
             }
        std::vector<float> floatData;
        floatData.reserve(velocity.size()+position.size());
        
        for(auto pos : velocity)
        {
            floatData.push_back(static_cast<float>(pos));
        }

        for(auto vel : position)
        {
            floatData.push_back(static_cast<float>(vel))
        }

        size_t size = floatData.size() * sizeof(float);
        const uint8_t* byte_buff = (uint8_t *)floatData.data();

        write (byte_buff,size);
        return true;
        } catch (const std::exception& e) {
            return false;
        }
    }

    bool receivePackage(size_t expected_floats, std::vector<float>& results) {
        try {
            size_t expected_bytes = expected_floats * sizeof(float);

            
            if (serialPort_.available() >= expected_bytes) {
                std::vector<uint8_t> rx_buffer;
                
              
                serialPort_.read(rx_buffer, expected_bytes);

              
                const float* float_data = (const float*)rx_buffer.data();

                results.assign(float_data, float_data + expected_floats);
                
                return true;
            }
            
            return false;
            
        } catch (const std::exception& e) {
            return false;
        }
    }
    
};

//zapomanialem ale warto jeszcze dodac pozniej bity do weryfikacji bo jak zgubimy raz uart to sie wszystko rozjedzie
//troche mozna dopisac i weryfikowac co i jak 