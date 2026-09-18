#include <serial/serial.h>
#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

class uartcomm
{
    private:
    serial::Serial serialPort_;
    uint8_t control_data[2] = {0x1c , 0xff};
    std::vector<uint8_t> rx_pending_;

    public:

    bool connect(std::string& portname,std::uint32_t baudrate)
    {
        try {
            serialPort_.setPort(portname);
            serialPort_.setBaudrate(baudrate);

            serial::Timeout timeout = serial::Timeout::simpleTimeout(1000);
            serialPort_.setTimeout(timeout);
            serialPort_.open();
            return serialPort_.isOpen();
        }catch (const std::exception& e) {
        std::cerr << "UART connect: " << e.what() << '\n';
        return false;
        }
        
    }

   

    bool parse_send_packagage(std::vector<double> & velocity,std::vector<double> & position)
    {   try
        {
             
        std::vector<float> floatData;
        floatData.reserve(velocity.size()+position.size());
        
        for(auto pos : velocity)
        {
            floatData.push_back(static_cast<float>(pos));
        }

        for(auto vel : position)
        {
            floatData.push_back(static_cast<float>(vel));
        }

        std::vector<uint8_t> tx_buffer;
        tx_buffer.push_back(control_data[0]);
        tx_buffer.push_back(control_data[1]);

        const uint8_t* float_bytes = reinterpret_cast<const uint8_t*>(floatData.data());
        size_t float_bytes_size = floatData.size() * sizeof(float);
        tx_buffer.insert(tx_buffer.end(), float_bytes, float_bytes + float_bytes_size);

        serialPort_.write(tx_buffer);
            
        return true;


        } catch (const std::exception& e) {
        std::cerr << "UART write: " << e.what() << '\n';
        return false;
}
    }

    bool receivePackage(std::vector<double> & hw_pos  ,std::vector<double> & hw_vel  ) {
      
            
            constexpr std::size_t FRAME_SIZE = 11;
            
        try{
            if(!serialPort_.isOpen())
            {
                return false;
            }
            //przyjmujemy max 256 bit
            std::array<uint8_t,256> data_chunk;

            const std::size_t to_read =
            std::min(serialPort_.available(), data_chunk.size());
        
            if (to_read > 0) {
            const std::size_t received =
                serialPort_.read(data_chunk.data(), to_read);

            
                rx_pending_.insert(
                rx_pending_.end(),
                data_chunk.begin(),
                data_chunk.begin() + received);
                }
        
        

            std::size_t offset = 0;
            bool updated = false;
            
            while(rx_pending_.size() - offset > 2)
            {
                if(rx_pending_[offset] != control_data[0] || rx_pending_[offset+1] != control_data[1])
                {
                    offset++;
                    continue;
                }
                if(rx_pending_.size() - offset < FRAME_SIZE)
                {
                    break;
                }
                //odczyt id
                const uint8_t joint_id = rx_pending_[offset+2];

                if(joint_id > hw_pos.size() || joint_id > hw_vel.size())
                {
                    offset++;
                    continue;
                }
                float vel;
                float pos;

                memcpy(&pos,rx_pending_.data()+offset+3,sizeof(pos));
                memcpy(&vel,rx_pending_.data() + 7 + offset,sizeof(float));
            
                hw_pos[joint_id] = static_cast<double>(pos);
                hw_vel[joint_id] = static_cast<double>(vel);

                offset += FRAME_SIZE;
                updated = true;

            }

              rx_pending_.erase(
                    rx_pending_.begin(),
                    rx_pending_.begin()+offset);
                return updated;
                
        } catch (const std::exception& e) {
        std::cerr << "UART receive: " << e.what() << '\n';
        return false;
        }

           
    }
    

    
};

//zapomanialem ale warto jeszcze dodac pozniej bity do weryfikacji bo jak zgubimy raz uart to sie wszystko rozjedzie
//troche mozna dopisac i weryfikowac co i jak 