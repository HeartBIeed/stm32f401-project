#include "modbus.h"


void modbus_init(modbus_rtu *modbus, uint8_t address){

    modbus->address = address;
    modbus->rx_length = 0U;
    modbus->last_byte_time = 0U;
    modbus->frame_timeout = 4U; // настроить таймаут одного кадра
    modbus->rx_state = MODBUS_RX_IDLE; // стартовое состояние

}


void modbus_poll(modbus_rtu *modbus){ // передаем экхемпляр структуры

    uint8_t byte;
    uint32_t current_time; // сюда пишем текушие мс по систику

    if (modbus->rx_state == MODBUS_RX_FRAME_READY) {
        return; // выходим при состоянии  готов
    }

    while (USART1_read_byte(&byte)) // пока буфер не пуст 
    {
        if (modbus->rx_length < MODBUS_MAX_FRAME_SIZE) // если буфер не переполнен
        {
            modbus->rx_frame[modbus->rx_length] = byte; // пишем в кадр байт
            modbus->rx_length++; //инкремент позиции байтов

            modbus->last_byte_time = get_mills(); // пишем в ЛБТ время
            modbus->rx_state = MODBUS_RX_RECEIVING;
        } else {
        // если буфер переполнен
        modbus->rx_length = 0U;
        modbus->rx_state = MODBUS_RX_IDLE;// Кадр не помещается — сбрасываем приём.
        }
    }

    if (modbus->rx_state == MODBUS_RX_RECEIVING) {// проверка конца кадра
        current_time = get_mills(); // обновляем вреня начала подсчета таймаута

        if ((current_time - modbus->last_byte_time) >= modbus->frame_timeout) {
        // если текщие мс - время последнего кадра >= таймауту примерно 3,5 мс 
            modbus->rx_state = MODBUS_RX_FRAME_READY; // готов кадр
        }
    }
}

int modbus_frame_check(modbus_rtu *modbus){ // проверка кадра по срс16

    uint16_t received_crc;
    uint16_t calculated_crc;

    if (modbus->rx_state != MODBUS_RX_FRAME_READY) {  //кадр не готов
        return 0;
    }

    if (modbus->rx_length < 4U) { // кадр маленький
        return 0;
    }

    if ((modbus->rx_frame[0] != modbus->address) &&
        (modbus->rx_frame[0] != 0U)) { //если адрес 0 — бродкаст
        return 0; 
    }


received_crc = // собираем срс байты 
    (uint16_t)modbus->rx_frame[modbus->rx_length - 2U] |
    ((uint16_t)modbus->rx_frame[modbus->rx_length - 1U] << 8U);


    calculated_crc = modbus_crc16(modbus->rx_frame,modbus->rx_length - 2U);
// вычисляем срс кадра без последних 2 байтов
    if (received_crc != calculated_crc) return 0; // если не совпали

    return 1;
}

void modbus_frame_reset(modbus_rtu *modbus){

    modbus->rx_length = 0U;
    modbus->rx_state = MODBUS_RX_IDLE;
}

int modbus_handle_read_holding(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count){
// экземпляр структуры, массив регистров, количество

    uint16_t start_address; // адресс первого регистра
    uint16_t quantity; //количество регистров
    uint16_t crc;
    uint16_t tx_index; 

    if (registers == NULL) return 0;

    if (modbus->rx_length != 8U) { //0x03 всегда содержит 8 байтов
        return 0;
    }

    if (modbus->rx_frame[1] != 0x03U) {
        return 0; // если функция не 3
    }

    if (modbus->rx_frame[0] == 0U) { // бродкаст
        return 0;
    }

    start_address =((uint16_t)modbus->rx_frame[2] << 8U) | (uint16_t)modbus->rx_frame[3];
    // берем адрес со 2 и 3 байта
    quantity =((uint16_t)modbus->rx_frame[4] << 8U) | (uint16_t)modbus->rx_frame[5];
    // берем количество запрашиваемых регистров

    if ((quantity == 0U) || (quantity > 125U)) {
        return 0; // в количестве от 1 до 125 иначе ошибка
    }

    if ((start_address >= registers_count) || (quantity > (registers_count - start_address))) {
        return 0; // проверка выхода за кол-во регистров
    }

    // формируем ответный эхо кадр
    modbus->tx_frame[0] = modbus->address;
    modbus->tx_frame[1] = 0x03U;
    modbus->tx_frame[2] = (uint8_t)(quantity * 2U); //количество байтов данных * 2 - стардарт ответа

    tx_index = 3U; // 3 позиции заполненны теперь след позиции

    for (uint16_t i = 0; i < quantity; i++) {

        uint16_t value = registers[start_address + i];

// по 2 байта записываем от текущего индекса со смещением 
// значения регистров от старт адреса

        modbus->tx_frame[tx_index++] =
            (uint8_t)(value >> 8U);

        modbus->tx_frame[tx_index++] =
            (uint8_t)(value & 0xFFU);
    }

    crc = modbus_crc16(modbus->tx_frame, tx_index);

 // срс передаётся младшим байтом вперёд.

    modbus->tx_frame[tx_index++] =
        (uint8_t)(crc & 0xFFU);

    modbus->tx_frame[tx_index++] =
        (uint8_t)(crc >> 8U);

    modbus->tx_length = tx_index;
// длина сформ кадра в  modbus->tx_length
    return 1;

}