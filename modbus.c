#include "modbus.h"

uint16_t holding_registers[HOLDING_REGISTERS_COUNT] = {0};

void modbus_init(modbus_rtu *modbus, uint8_t slave_address){

    modbus->address = slave_address;
    modbus->rx_length = 0;
    modbus->last_byte_time = 0;
    modbus->rx_state = MODBUS_RX_IDLE; // стартовое состояние

    if (USART1->BRR == 0x222E) modbus->frame_timeout = 4; // 9600
    if (USART1->BRR == 0x1117) modbus->frame_timeout = 3; // 19200
    if (USART1->BRR == 0x02D9) modbus->frame_timeout = 2; // 115200
}


void modbus_poll(modbus_rtu *modbus){ // передаем экземпляр структуры

    uint8_t byte;
    uint32_t current_time; // сюда пишем текушие мс по систику

    switch (modbus->rx_state)
    {
        case MODBUS_RX_IDLE:
            
            if (USART1_read_byte(&byte)){
                modbus->rx_frame[0] = byte; // пишем в структуру 1 байт
                modbus->rx_length = 1; 
                modbus->last_byte_time = get_mills(); // пишем в ЛБТ время
                modbus->rx_state = MODBUS_RX_RECEIVING;
            }

        break; 

        case MODBUS_RX_RECEIVING:

            while (USART1_read_byte(&byte)) // пока буфер не пуст 
            {
                if (modbus->rx_length < MODBUS_MAX_FRAME_SIZE) // если буфер не переполнен
                {
                    modbus->rx_frame[modbus->rx_length] = byte; // пишем в структуру байт
                    modbus->rx_length++; //инкремент позиции байтов

                    modbus->last_byte_time = get_mills(); // пишем в ЛБТ время (!!!)
                    modbus->rx_state = MODBUS_RX_RECEIVING;

                } else { // если буфер переполнен

                modbus->rx_length = 0;
                modbus->rx_state = MODBUS_RX_IDLE;// Кадр не помещается — сбрасываем приём.
                }
            } // end while

            current_time = get_mills(); //(!!!)

            if ((current_time - modbus->last_byte_time) >= modbus->frame_timeout) { //(!!!)
                modbus->rx_state = MODBUS_RX_FRAME_READY; // кадр приняли
            }

        break;

        case MODBUS_RX_FRAME_READY:

            // обработка функций 
            if (modbus_frame_check(modbus)) {
                    
                    switch (modbus->rx_frame[1]) // берем № функции
                    {

                    case MODBUS_FC_READ_HOLDING_REGISTERS:

                        if (modbus_handle_read_holding(modbus,holding_registers,HOLDING_REGISTERS_COUNT)) {

                            for (uint16_t i = 0; i < modbus->tx_length; i++) {

                                USART1_send_byte(modbus->tx_frame[i]); // отправляем сформированный фрейм
                            }
                        }

                    modbus->rx_length = 0;
                    modbus->rx_state = MODBUS_RX_IDLE;

                    break; //MODBUS_FC_READ_HOLDING_REGISTERS

                    case MODBUS_FC_WRITE_SINGLE_REGISTER:

                        if (modbus_handle_write_single_hldreg(modbus,holding_registers,HOLDING_REGISTERS_COUNT)) {

                            if (!(modbus->rx_frame[0] == 0)) { // если не бродкаст

                                for (uint16_t i = 0; i < modbus->tx_length; i++) {
                                    USART1_send_byte(modbus->tx_frame[i]); // отправляем сформированный эхо фрейм
                                }
                            } 
                        }

                    modbus->rx_length = 0;
                    modbus->rx_state = MODBUS_RX_IDLE;

                    break; // MODBUS_FC_WRITE_SINGLE_REGISTER                    

                    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:

                        if (modbus_handle_write_multiple_hldreg(modbus,holding_registers,HOLDING_REGISTERS_COUNT)) {
                            
                            if (!(modbus->rx_frame[0] == 0)) { // если не бродкаст

                                for (uint16_t i = 0; i < modbus->tx_length; i++) {
                                    USART1_send_byte(modbus->tx_frame[i]); // отправляем эхо фрейм
                                }
                            }
                        }

                    modbus->rx_length = 0;
                    modbus->rx_state = MODBUS_RX_IDLE;

                    break; // MODBUS_FC_WRITE_MULTIPLE_REGISTERS  


                    break; // MODBUS_FC_REPORT_SLAVE_ID                    

                    case MODBUS_FC_REPORT_SLAVE_ID:

                        if (modbus_handle_report_slave_id(modbus)) {

                            for (uint16_t i = 0; i < modbus->tx_length; i++) {
                                USART1_send_byte(modbus->tx_frame[i]); // отправляем эхо фрейм
                            }
                            
                        }

                    modbus->rx_length = 0;
                    modbus->rx_state = MODBUS_RX_IDLE;

                    break; // MODBUS_FC_REPORT_SLAVE_ID  

                    default:
                        break;
                    }
            }
        
        modbus->rx_length = 0;
        modbus->rx_state = MODBUS_RX_IDLE;
        break; // MODBUS_RX_FRAME_READY

    default:
        break;
    }
}


int modbus_frame_check(modbus_rtu *modbus){ // проверка кадра по срс16

    uint16_t received_crc;
    uint16_t calculated_crc;

    if (modbus->rx_state != MODBUS_RX_FRAME_READY) {  //кадр не готов
        return 0;
    }

    if (modbus->rx_length < 4) { // кадр маленький
        return 0;
    }

    if (modbus->rx_frame[0] != modbus->address &&
        modbus->rx_frame[0] != 0) {
        return 0; // Запрос другому слейву
    }

    received_crc = // собираем срс байты 
        (uint16_t)modbus->rx_frame[modbus->rx_length - 2] |
        ((uint16_t)modbus->rx_frame[modbus->rx_length - 1] << 8);


    calculated_crc = modbus_crc16(modbus->rx_frame,modbus->rx_length - 2);
// вычисляем срс кадра без последних 2 байтов
    if (received_crc != calculated_crc) return 0; // если не совпали

    return 1;
}

//3
int modbus_handle_read_holding(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count){
// экземпляр структуры, массив регистров, количество

    uint16_t start_address; // адрес первого регистра
    uint16_t quantity; //количество регистров
    uint16_t crc;
    uint16_t tx_index; 

    if (registers == NULL) return 0;

    if (modbus->rx_length != 8) { //0x03 всегда содержит 8 байтов
        return 0;
    }

    if (modbus->rx_frame[1] != MODBUS_FC_READ_HOLDING_REGISTERS) {
        return 0; // если функция не 3
    }

    if (modbus->rx_frame[0] == 0) { // бродкаст
        return 0;
    }

    start_address =((uint16_t)modbus->rx_frame[2] << 8) |
                    (uint16_t)modbus->rx_frame[3];
    // берем адрес со 2 и 3 байта

    quantity =((uint16_t)modbus->rx_frame[4] << 8) | 
               (uint16_t)modbus->rx_frame[5];
    // берем количество запрашиваемых регистров

    if ((quantity == 0) || (quantity > 125)) {
        return 0; // в количестве от 1 до 125 иначе ошибка
    }

    if ((start_address >= registers_count) || (quantity > (registers_count - start_address))) {
        return 0; // проверка выхода за кол-во регистров
    }

    // формируем ответный кадр
    modbus->tx_frame[0] = modbus->address;
    modbus->tx_frame[1] = MODBUS_FC_READ_HOLDING_REGISTERS;
    modbus->tx_frame[2] = (uint8_t)(quantity * 2); //количество байтов данных * 2 - стардарт ответа

    tx_index = 3; // 3 позиции заполненны теперь след позиции

    for (uint16_t i = 0; i < quantity; i++) {

        uint16_t value = registers[start_address + i];

// по 2 байта записываем от текущего индекса со смещением 
// значения регистров от старт адреса

        modbus->tx_frame[tx_index++] =
            (uint8_t)(value >> 8);

        modbus->tx_frame[tx_index++] =
            (uint8_t)(value & 0xFF);
    }

    crc = modbus_crc16(modbus->tx_frame, tx_index);

 // срс передаётся младшим байтом вперёд.
    modbus->tx_frame[tx_index++] = (uint8_t)(crc & 0xFF);
    modbus->tx_frame[tx_index++] = (uint8_t)(crc >> 8);

    modbus->tx_length = tx_index; // длина сформ кадра в  modbus->tx_length

    return 1;
}

//6
int modbus_handle_write_single_hldreg(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count){
    
    uint16_t register_address; // адресс первого регистра
    uint16_t data;
    uint16_t crc;

    if (registers == NULL) return 0;

    if (modbus->rx_length != 8) { //0x06 всегда содержит 8 байтов
        return 0;
    }

    if (modbus->rx_frame[1] != MODBUS_FC_WRITE_SINGLE_REGISTER) {
        return 0; // если функция не 6
    }

    register_address =((uint16_t)modbus->rx_frame[2] << 8) |
                       (uint16_t)modbus->rx_frame[3];
    // берем адрес 

    data =((uint16_t)modbus->rx_frame[4] << 8) |
           (uint16_t)modbus->rx_frame[5];

    holding_registers[register_address] = data;    

    crc = modbus_crc16(modbus->tx_frame, 8);

    // ответный кадр
    modbus->tx_frame[0] = modbus->rx_frame[0]; 
    modbus->tx_frame[1] = modbus->rx_frame[1];  
    modbus->tx_frame[2] = modbus->rx_frame[2];  
    modbus->tx_frame[3] = modbus->rx_frame[3]; 
    modbus->tx_frame[4] = modbus->rx_frame[4];
    modbus->tx_frame[5] = modbus->rx_frame[5];  
    modbus->tx_frame[6] = (uint8_t)(crc & 0xFF);
    modbus->tx_frame[7] = (uint8_t)(crc >> 8);

    modbus->tx_length = 8; // длина сформ кадра в  modbus->tx_length

    return 1;
}

//16
int modbus_handle_write_multiple_hldreg(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count){
    
    uint16_t register_address; // адрес первого регистра
    uint16_t data;
    uint16_t crc;
    uint16_t quantity_write_reg; //количество записываемых регистров
    uint16_t byte_count; //количество байт

    if (registers == NULL) return 0;

    if (modbus->rx_frame[1] != MODBUS_FC_WRITE_MULTIPLE_REGISTERS) {
        return 0; // если функция не 16
    }
  
    register_address =((uint16_t)modbus->rx_frame[2] << 8) |
                       (uint16_t)modbus->rx_frame[3];
    // берем адрес

    quantity_write_reg =((uint16_t)modbus->rx_frame[4] << 8) |
                       (uint16_t)modbus->rx_frame[5];
    // количество регистров

    byte_count = ((uint16_t)modbus->rx_frame[6]); 

    for (uint8_t i = 7; i < (quantity_write_reg + 7); i++)
    {
        data =((uint16_t)modbus->rx_frame[i] << 8) |
        (uint16_t)modbus->rx_frame[i];

        holding_registers[register_address] = data; 
        
        register_address ++;
    }
    
    quantity_write_reg = ((uint16_t)modbus->rx_frame[4] << 8) |
           (uint16_t)modbus->rx_frame[5];  // количество записанных регистров передаем в ответ

    crc = modbus_crc16(modbus->tx_frame, 8);

    // ответный кадр
    modbus->tx_frame[0] = modbus->rx_frame[0]; // слейв 
    modbus->tx_frame[1] = modbus->rx_frame[1]; // функция
    modbus->tx_frame[2] = modbus->rx_frame[2]; // адрес регистра
    modbus->tx_frame[3] = modbus->rx_frame[3]; // адрес регистра
    modbus->tx_frame[4] = modbus->rx_frame[4]; // количество регистров
    modbus->tx_frame[5] = modbus->rx_frame[5]; // количество регистров
    modbus->tx_frame[6] = (uint8_t)(crc & 0xFF);
    modbus->tx_frame[7] = (uint8_t)(crc >> 8);

    modbus->tx_length = 8; // длина сформ кадра в  modbus->tx_length

    return 1;
}


// 17
int modbus_handle_report_slave_id(modbus_rtu *modbus){
    
        uint16_t crc;
    
    
        if (modbus->rx_length != 4) { //0x03 всегда содержит 8 байтов
            return 0;
        }
    
        if (modbus->rx_frame[1] != MODBUS_FC_REPORT_SLAVE_ID) {
            return 0; // если функция не 17
        }
    
        if (modbus->rx_frame[0] == 0) { // бродкаст
            return 0;
        }
    
      // формируем ответный кадр
        modbus->tx_frame[0] = modbus->address;
        modbus->tx_frame[1] = MODBUS_FC_REPORT_SLAVE_ID;
        modbus->tx_frame[2] = 2;
        modbus->tx_frame[3] = modbus->address;
        modbus->tx_frame[4] = 0xFF;

        crc = modbus_crc16(modbus->tx_frame, 5);

        modbus->tx_frame[5] = (uint8_t)(crc & 0xFF);
        modbus->tx_frame[6] = (uint8_t)(crc >> 8);
    
        modbus->tx_length = 7; // длина сформ кадра в  modbus->tx_length
    
        return 1;
    }

/*

 Скорость t3.5 при 11 битах (8E1 / 8N2)
 4800         | 8,021 мс 
 9600         | 4,010 мс 
 19200        | 2,005 мс 
 38400 и выше | 1,750 мс  

*/