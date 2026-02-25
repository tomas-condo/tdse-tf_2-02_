#ifndef I2C_H_
#define I2C_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "stdint.h"
#include "main.h"

/********************** external functions declaration ***********************/
extern void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
extern void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
extern void EEPROM_PageErase (uint16_t page);

void eeprom_write_score(uint16_t page, uint16_t offset, uint16_t score);
uint16_t eeprom_read_score(uint16_t page, uint16_t offset);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */

/********************** end of file ******************************************/
