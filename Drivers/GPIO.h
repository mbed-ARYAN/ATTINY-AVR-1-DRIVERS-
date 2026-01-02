/*
 * GPIO.h
 *
 * Created: 17-12-2025 22:18:25
 *  Author: HP
 */ 


#ifndef GPIO_H_
#define GPIO_H_

typedef struct  
{
	PORT_t *port;
	uint8_t pin;
	
}gpio_pin_t;


uint8_t gpio_read_output_latch(gpio_pin_t pin);
uint8_t gpio_read_physical(gpio_pin_t pin);
void gpio_set_inverted(gpio_pin_t pin, uint8_t enable);

typedef enum{
	GPIO_INPUT;
	GPIO_INPUT_PULLUP;
	GPIO_OUTPUT;
	GPIO_ANALOG;
	}gpio_mode_t;

static inline void gpio_set_output(gpio_pin_t pin ){
	
	 pin.port->DIRSET = (1<<pin.pin);
	 
	 }
	 
static inline void gpio_set_input(gpio_pin_t pin){
	
	pin.port->DIRCLR = (1<<pin.pin);
}	 

static inline gpio_write_high(gpio_pin_t pin){
	
	pin.port->OUTSET = (1<<pin.pin);
}

static inline gpio_write_low(gpio_pin_t pin){
	
	pin.port->OUTCLR = (1<<pin.pin);
}

static inline gpio_toggle(gpio_pin_t pin){
	
	pin.port->OUTTGL = (1<<pin.pin);
}


static inline uint8_t gpio_read(gpio_pin_t pin){
	
	return(pin.port->IN & (1<< pin.pin))?1:0;
}

void gpio_config(gpio_pin_t pin , gpio_mode_t mode){
	
	//disable pullup , normal input sense 
	pin.port->PIN0CTRL &= ~(PORT_PULLUPEN_bm | PORT_INVEN_bm);
	
	switch(mode)
	{
		case GPIO_OUTPUT:
		gpio_set_output(pin);
		break;
		
		case GPIO_INPUT:
		gpio_set_input(pin);
		pin.port->PIN0CTRL |= PORT_ISC_BOTHEDGES_gc;
		break;
		
		case GPIO_INPUT_PULLUP:
		gpio_set_input(pin);
		pin.port->PIN0CTRL |= PORT_PULLUPEN_bm;
		pin.port->PIN0CTRL |= PORT_ISC_BOTHEDGES_gc;
		break;
		
		case GPIO_ANALOG:
		gpio_set_input(pin);
		pin.port->PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
		break;
					
	}
	
}











#endif /* GPIO_H_ */