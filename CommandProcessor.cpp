// Test the removal of this #if #else code it may not be necessary
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

// Assuming the RGB order for LEDs

class Command_Processor {

public:
	Command_Processor(uint8_t n, uint8_t dpin, uint8_t cpin, uint8_t order=WS2801_RGB);
	~Command_Processor();
	
	void process_command(String command);	// Takes string and selects correct command
	

	
	// direct translation of commands received over UDP
	
	void draw_linear_point(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue);
	void draw_grid_point(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
	void fill_color(uint8_t r, uint8_t g, uint8_t);		// 
	void fill_white();
	void fill_white_variable(uint8_t brightness);
	void fill_off();

private:
	
	// Specific draw variables
	uint8_t pixel_count;
	uint8_t *pixels;
	uint8_t rgb_order;
	uint8_t clk_pin, data_pin;
	uint8_t clk_pin_mask, data_pin_mask;
	bool started;
	
	volatile uint8_t *clk_port, *data_port;
		
	// Specific draw commands
	
	uint8_t translate_grid_to_linear(uint8_t x, uint8_t y);
	
      void start_grid(void);
      void update_grid(void);
      void set_pixel_color(uint8_t n, uint8_t red, uint8_t green, uint8_t blue);
      uint8_t pixel_count(void);
      uint8_t get_pixel_color_red(uint8_t n);
      uint8_t get_pixel_color_green(uint8_t n);
      uint8_t get_pixel_color_blue(uint8_t n);
	
};


Command_Processor::Command_Processor(uint8_t n, uint8_t data_pin, uint8_t clk_pin, uint8_t order=WS2801_RGB){
	rgb_order = order;
	started = false;
	pixels = (uint8_t *)calloc(n, 3); 		// Allocate 3 bytes per each pixel. There are n pixels
	pixel_count = pixels != NULL ? n : 0;	// Update pixel count to reflect n
	
      if(started == true) { // If start_grid() was already called...
        // Enable output on 'soft' SPI pins:
        pinMode(dpin, OUTPUT);
        pinMode(cpin, OUTPUT);
      } // Otherwise, pins are not set to outputs until start_grid() is called.
	
      this.data_pin     = data_pin;			// Saves current data pin setting to Command object
      this.clk_pin      = clk_pin;			// Saves current clock pin setting to Command object
      clk_port     	= portOutputRegister(digitalPinToPort(clk_pin));
      clk_pin_mask  	= digitalPinToBitMask(clk_pin);
      data_port    	= portOutputRegister(digitalPinToPort(data_pin));
      data_pin_mask 	= digitalPinToBitMask(data_pin);
}

Command_Processor::~Command_Processor(){
	
}

void Command_Processor::process_command(String command){
	
}

// direct translation of commands received over UDP

void Command_Processor::draw_linear_point(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue){
	
}

void Command_Processor::draw_grid_point(uint8_t index, uint8_t red, uint8_t green, uint8_t blue){
	
}

void Command_Processor::fill_color(uint8_t r, uint8_t g, uint8_t){
	
} 

void Command_Processor::fill_white(){
	
}

void Command_Processor::fill_white_variable(uint8_t brightness){
	
}

void Command_Processor::fill_off(){
	
}

volatile uint8_t *clk_port, *data_port;


// Specific draw commands

uint8_t Command_Processor::translate_grid_to_linear(uint8_t x, uint8_t y){
	
}

void Command_Processor::start_grid(void){
	pinMode(data_pin, OUTPUT);
	pinMode(clk_pin , OUTPUT);
	
	started = true;
}

void Command_Processor::update_grid(void){
	uint8_t i;
	uint8_t num_color_bytes = pixel_count * 3;
	uint8_t bit;
	
	for(i = 0; i < num_color_bytes; i++ ){
		for(bit=0x80; bit; bit >>= 1) {
			if(pixels[i] & bit)
				*data_port |= data_pin_mask;
			else
				*data_port &= ~data_pin_mask;
			
			// Manually sets clock pin low and then high (or opposite)
			*clk_port |= clk_pin_mask;
			*clk_port &= ~clk_pin_mask;
		}
	}
}

void Command_Processor::set_pixel_color(uint8_t n, uint8_t red, uint8_t green, uint8_t blue){
	uint8_t *p = &pixels[n * 3]; // This loads the first par of the nth pixel (red part)
	if(n < pixel_count) {
		*p = r;
		p++;
		*p = g;
		p++;
		*p = b;
		p++;
	}
}

uint8_t Command_Processor::num_pixels(void){
	
}

uint8_t Command_Processor::get_pixel_color_red(uint8_t n){
	if (n < pixel_count) {
		uint8_t pixel_byte = n * 3;
		return pixels[pixel_byte];
	} 
	
	return 0; // Pixel # is invalid
}

uint8_t Command_Processor::get_pixel_color_green(uint8_t n){
	if (n < pixel_count) {
		uint8_t pixel_byte = n * 3;
		return pixels[pixel_byte + 1];
	} 
	
	return 0; // Pixel # is invalid
}

uint8_t Command_Processor::get_pixel_color_blue(uint8_t n){
	if (n < pixel_count) {
		uint8_t pixel_byte = n * 3;
		return pixels[pixel_byte + 2];
	} 
	
	return 0; // Pixel # is invalid
}
