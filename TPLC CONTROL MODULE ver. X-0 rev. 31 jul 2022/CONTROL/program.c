//this program is nowhere near done, consult paper notes for details

//analog inputs
//x = block (8 blocks)
//y = block sub-pin (16 per block)
int in[][] =
{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//digital outputs
//x = block (10 blocks)
//y = block sub-pin (8 per block)
bool out[][] =
{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

//pwm outputs
//x = block (10 blocks)
int pwm[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//slt states
//x = pin
//y = pin (4 pins)
bool slt[][] =
{
	{0, 0, 0, 0},
	{0, 0, 0, 1},
	{0, 0, 1, 0},
	{0, 0, 1, 1},
	{0, 1, 0, 0},
	{0, 1, 0, 1},
	{0, 1, 1, 0},
	{0, 1, 1, 1},
	{1, 0, 0, 0},
	{1, 0, 0, 1},
	{1, 0, 1, 0},
	{1, 0, 1, 1},
	{1, 1, 0, 0},
	{1, 1, 0, 1},
	{1, 1, 1, 0},
	{1, 1, 1, 1}
};




//read function (16 timepulses)
void read()
{
	//write to "in" matrix from demux
	for(y = 0; y <= 15; y++)	//advance to next sub-pin
	{
		//read inputs from the given sub-pins of each block and write it to "in" matrix 
		analogRead(pin_i_blk0, in[0][y]);
		analogRead(pin_i_blk1, in[1][y]);
		analogRead(pin_i_blk2, in[2][y]);
		analogRead(pin_i_blk3, in[3][y]);
		analogRead(pin_i_blk4, in[4][y]);
		analogRead(pin_i_blk5, in[5][y]);
		analogRead(pin_i_blk6, in[6][y]);
		analogRead(pin_i_blk7, in[7][y]);
		delay(timepulse);
	}
}

//write function (17 timepulses)
void write()
{
	//write to shift register from out[][] (16 timepulses)
	for(y = 0; y <= 7; y++)	//advance to next sub-pin
	{
		//read out[][] for the given sub-pins of each block and write it to shift register
		digitalWrite(pin_ser_blk0, out[0][y]);
		digitalWrite(pin_ser_blk1, out[1][y]);
		digitalWrite(pin_ser_blk2, out[2][y]);
		digitalWrite(pin_ser_blk3, out[3][y]);
		digitalWrite(pin_ser_blk4, out[4][y]);
		digitalWrite(pin_ser_blk5, out[5][y]);
		digitalWrite(pin_ser_blk6, out[6][y]);
		digitalWrite(pin_ser_blk7, out[7][y]);
		digitalWrite(pin_ser_blk8, out[8][y]);
		digitalWrite(pin_ser_blk9, out[9][y]);
		//pulse clk_shf to write serial inputs to shift register
		digitalWrite(pin_clk_shf, 0);
		delay(timepulse);
		digitalWrite(pin_clk_shf, 1);
		delay(timepulse);
	}
	//pulse clk_str to write shift register outputs to register (1 timepulse)
	//pin_clk_str is inverted because it's driving a ~clk_str pin on the 74HC595
	digitalWrite(pin_clk_str, 1);
	delay(timepulse);
	digitalWrite(pin_clk_str, 0);
	//read pwm[] for the given block and write to respective pwm pins
	//pwm is inverted because it's driving an O/D pin on the 74HC595
	//255 was used for atmega328p, new number needed for new MCU
	analogWrite(pin_pwm_blk0, 255 - pwm[0]);
	analogWrite(pin_pwm_blk1, 255 - pwm[1]);
	analogWrite(pin_pwm_blk2, 255 - pwm[2]);
	analogWrite(pin_pwm_blk3, 255 - pwm[3]);
	analogWrite(pin_pwm_blk4, 255 - pwm[4]);
	analogWrite(pin_pwm_blk5, 255 - pwm[5]);
	analogWrite(pin_pwm_blk6, 255 - pwm[6]);
	analogWrite(pin_pwm_blk7, 255 - pwm[7]);
	analogWrite(pin_pwm_blk8, 255 - pwm[8]);
	analogWrite(pin_pwm_blk9, 255 - pwm[9]);
}

//orders (calculates out[][] and pwm[] as a function of in[][])
//if input voltage is above ~2.5 V at an input pin, set corresponding output pin high
//pwm for each output block is determined by the voltage of the first pin of it's block
//needs to be reworked for new MCU
void orders()
{
	//calculate pwm
	pwm[0] = in[0][0] / 4;
	pwm[1] = in[0][7] / 4;
	pwm[2] = in[1][0] / 4;
	pwm[3] = in[1][7] / 4;
	

	//calculate out
	for(y = 0; y <= 7; y++)
	{
		if(in[0][y] < 127)
		{
			out[0][y];
		}
		if(in[0][y + 8] < 127)
		{
			out[1][y];
		}
		if(in[1][y] < 127)
		{
			out[2][y];
		}
		if(in[1][y + 8] < 127)
		{
			out[3][y];
		}
	}
}

//program
void loop()
{
	//wait time
	delay(950);

	//read inputs
	read();

	//execute orders
	orders();

	//write outputs
	write();
}