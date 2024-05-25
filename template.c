
#include "template.h"

template_t *example_template_init(template_t *dev, gpio_num_t dev_template_gpio, enum _template_work_state_t dev_mode)
{

  // In this function,Assigning values to structural variables of components
  dev->mode = dev_mode;
  dev->template_gpio = dev_template_gpio;

  return dev;
}

esp_err_t example_template_begin(template_t *dev, QueueHandle_t handle_queue_template, TaskHandle_t handle_task_template)
{

  // In this function, it is generally used to create and initialize pins, queues, and tasks.
  gpio_reset_pin(dev->template_gpio);

  // Select the working mode of the pin based on the control of the device
  gpio_set_direction(dev->template_gpio, GPIO_MODE_OUTPUT);

  // Set the initial pin high and low level states based on the characteristics of the device
  gpio_set_level(dev->template_gpio, 0);

  // Create message queues belonging to components
  /*
    You can set the second formal parameter based on the type of data you transfer.
    Here, we use int as an example to demonstrate creating
  */
  dev->handle_queue_template = handle_queue_template;
  handle_queue_template = xQueueCreate(1, sizeof(int));

  // Create message task belonging to components,this task you can user defined
  /*
      ------------NOTE------------
    1.The task must not have a return value
    2.The task function must contain while
    3.There must be a delay to prevent the watchdog from triggering a reset
    4.You can pass parameters to the task, and the formal parameters for passing parameters are in the fourth one
    5.To set the stack size of a task based on its actual task cost, it needs to be a multiple of 1024
  */
  dev->handle_task_template = handle_task_template;
  xTaskCreate(template_task, "template_task", 1024 * 2, NULL, 2, &handle_task_template);
}

esp_err_t example_template_set_val(int set_data)
{

  // In this function,Write data to a certain address of a component
}

esp_err_t example_template_set_mode(enum _template_mode_t template_mode)
{

  // In this function,Write mode to a certain address of a component
}

int example_template_get_data(template_t *dev_template)
{

  // In this function,Returns the data calculated by the component
  return dev_template->template_data;
}

enum _template_work_state_t example_template_get_state(template_t *dev_template)
{
  // In this function,Returns the work state calculated by the component
  return dev_template->state;
}

int read_data_form_template_queue(template_t *dev)
{

  // In this function,read data in the template queue
  int data;
  if (dev->handle_queue_template != NULL)
  {
    xQueuePeek(dev->handle_queue_template, &data, 1);
  }
  return data;
}

esp_err_t write_data_to_template_queue(template_t *dev, int template_set_val)
{
    // In this function,write data in the template queue
    if (dev->handle_queue_template != NULL)
    {
      xQueueOverwrite(dev->handle_queue_template, &template_set_val);
    }
}

void template_task(void *arg)
{
  while (1)
  {
    /* code */
    printf("test/n");

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}