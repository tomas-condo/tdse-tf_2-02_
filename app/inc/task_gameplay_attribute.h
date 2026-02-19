#ifndef TASK_INC_TASK_GAMEPLAY_ATTRIBUTE_H_
#define TASK_INC_TASK_GAMEPLAY_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdbool.h>
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/* Events to excite Task Gameplay */
// task_gameplay_attribute.h

/*typedef enum {
    ST_GAME_IDLE,

    // Ciclo de Juego
    ST_GAME_INIT_ROUND,     // Agrega color a la secuencia
    ST_GAME_PLAY_SEQ_ON,    // Enciende LED de la secuencia
    ST_GAME_PLAY_SEQ_OFF,   // Apaga LED (pausa)
    ST_GAME_WAIT_INPUT,     // Espera pulsación del jugador
    ST_GAME_VERIFY,         // Valida si la pulsación fue correcta
    ST_GAME_GAME_OVER       // Fin del juego
} task_gameplay_state_t;
*/

typedef enum {
	ST_GAME_IDLE,
	ST_GAME_INIT_ROUND,
	ST_GAME_PLAY_SEQ_ON,
	ST_GAME_PLAY_SEQ_OFF,
	ST_GAME_WAIT_INPUT,
	ST_GAME_VERIFY,
	ST_GAME_GAME_OVER,     // <--- Tu estado actual
    ST_GAME_SAVE_SCORE_1,  // <--- NUEVO
    ST_GAME_SAVE_SCORE_2,  // <--- NUEVO
    ST_GAME_SAVE_SCORE_3,  // <--- NUEVO
    ST_GAME_FINISH_OVER    // <--- NUEVO
} task_gameplay_state_t;

typedef enum task_gameplay_ev {
	EV_GAME_IDLE,
	EV_GAME_NONE = 0,

	/* Eventos asociados a task_sensor */
	EV_GAME_BTN_RO,
	EV_GAME_BTN_VE,
	EV_GAME_BTN_AZ,
	EV_GAME_BTN_AM,

	EV_GAME_START_NORMAL,
	EV_GAME_START_HARD
} task_gameplay_ev_t;


typedef struct
{
    uint32_t tick;
    task_gameplay_state_t state;
    task_gameplay_ev_t event;
    bool flag;

    // Configuración del Juego
    bool difficulty;        /* 0: Normal (repite tod), 1: Difícil (solo nuevo) */

    // Variables de la Lógica Simón Dice
    uint8_t  sequence[32];  /* Array que guarda la secuencia de colores (0-3) */
    uint8_t  seq_length;    /* Cuántos colores tiene la secuencia actual (Nivel) */

    uint8_t  play_index;    /* Índice usado por la Máquina para mostrar colores */
    uint8_t  user_index;    /* Índice usado para trackear qué color toca pulsar */

    uint16_t score;         /* Puntaje actual */

    // Auxiliares
    uint8_t  last_button_pressed; /* Para guardar qué botón se apretó en WAIT_INPUT */
    int      color;   /* Para control visual */

    bool     led_on_phase;

} task_gameplay_dta_t;


/********************** external data declaration ****************************/
extern task_gameplay_dta_t task_gameplay_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_GAMEPLAY_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
