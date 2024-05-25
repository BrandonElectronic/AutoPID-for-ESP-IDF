#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*Increase or decrease the included Header file according to the component function*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

/**********************
 *      TYPEDEFS
 **********************/

/** Template Work State.*/
enum _template_work_state_t {
    TEMPLATE_WORK_TEXT_LOWER,
    TEMPLATE_WORK_TEXT_UPPER,
    TEMPLATE_WORK_SPECIAL,
    TEMPLATE_WORK_NUMBER,
    TEMPLATE_WORK_USER_1,
    TEMPLATE_WORK_USER_2,
    TEMPLATE_WORK_USER_3,
    TEMPLATE_WORK_USER_4,
};

/** Template  State.*/
enum _template_mode_t {
    TEMPLATE_MODE_1,
    TEMPLATE_MODE_2,
    TEMPLATE_MODE_3,
  
};

/*Data of template*/
typedef struct {
    
    gpio_num_t template_gpio;           /*Pins required for component initialization.
                                        *Increase pin variables according to the required number of pins and name them in a standardized manner*/
    unsigned int template_data;        /*The data obtained or set by the component can be increased or decreased according to actual needs*/

    enum _template_work_state_t state;   /*The working mode or state of the component*/
    
    enum _template_mode_t mode;

    QueueHandle_t handle_queue_template;

    TaskHandle_t handle_task_template;
    
} template_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*
    If you want to modify some of the macro definitions you define through menuconfig instead of code editing. 
    You can read README.md to implement modifications.
    This part of the content is listed in the table of contents as "about kconfig"
*/

/*=====================
 * Init functions
 *====================*/

/**
 * Initialize the required pins, message queues, and tasks for the component
 * @param template_t *dev  Component structure pointer
 * @param gpio_num_t template_gpio Pin numbers for component settings   
 * @param enum _template_work_state_t mode
 * @return template_t *   
 */

template_t * example_template_init(template_t *dev,gpio_num_t dev_template_gpio,enum _template_work_state_t dev_mode);

/*=====================
 * Setter functions
 *====================*/

/**
 * Initialize the required pins, message queues, and tasks for the component
 * @param template_t *dev  Component structure pointer
 * @param QueueHandle_t handle_queue_template   Message queue handle for components
 * @param TaskHandle_t handle_task_template     Message task handle for components
 * @return  esp_err_t  
 */
esp_err_t example_template_begin(template_t * dev,QueueHandle_t handle_queue_template,TaskHandle_t handle_task_template);

/**
 * Write a data to an address or register in a component.
 * @param  Set the corresponding parameter data type according to the actual situation, such as int
           Pointer and supported data types
 * @return esp_err_t 
 */
esp_err_t example_template_set_val(int set_data);


/**
 * Set the component to what mode
 * @param Control the working mode of selected components and make choices based on actual application scenarios
 * @return esp_err_t 
 */
esp_err_t example_template_set_mode(enum _template_mode_t template_mode);


/*=====================
 * Getter functions
 *====================*/

/**
 * Read out the data obtained from the components.
 * @param template_t *  dev_template
 * @return Calculation data of components
 */
int example_template_get_data(template_t *  dev_template);

/**
 * Obtain the working status of components.
 * * @param template_t *  dev_template
 * @return enum _template_work_state_t  
 */
enum _template_work_state_t example_template_get_state(template_t *  dev_template);

/*=====================
 * Other functions
 *====================*/

/**
 * Obtaining data from the message queue of components.
 * @return Obtain the corresponding return value based on the message queue type you created, using int type as an example 
 */
int read_data_form_template_queue(template_t *dev);

/**
 * Write data to the message queue of components.
 * @return esp_err_t 
 */
esp_err_t write_data_to_template_queue(template_t *dev,int template_set_val);

/**
 * Create a task that belongs to this component.
 * !!!!The task must not have a return value!!!!
 */
void template_task(void *arg);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*TEMPLATE_H*/
