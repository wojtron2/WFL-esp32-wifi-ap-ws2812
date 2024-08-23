#include "globalvars.h"


int zmienna2 = 0;

//digitalWrite(STROBO_PIN, LOW);
//extern int strobo_loops;

void strobo1(int delay_on, int delay_off, int strobo_loops){

  for (int i = 0; i < strobo_loops; i++) {

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 1); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 0); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 1); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 0); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 1); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 0); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 1); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 0); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 1); //strobo gpio25 ON
  delay(delay_off);

  delay(delay_on);
  gpio_set_level(GPIO_NUM_25, 0); //strobo gpio25 ON
  delay(delay_off);

  }

}